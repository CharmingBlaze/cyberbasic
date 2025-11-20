#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <chrono>

namespace bas {

// Timer object storage
struct TimerData {
    double startTime{0.0};
    double pausedTime{0.0};
    bool isPaused{false};
    bool isRunning{false};
};

static std::unordered_map<int, TimerData> g_timers;
static int g_next_timer_id = 1;
static double g_delta_time = 0.0;
static double g_last_frame_time = 0.0;

// Update delta time (should be called each frame)
void update_delta_time() {
    double currentTime = GetTime();
    g_delta_time = currentTime - g_last_frame_time;
    g_last_frame_time = currentTime;
}

// Get current delta time
double get_delta_time() {
    return g_delta_time;
}

// Timer constructor: Timer()
static Value timer_constructor(const std::vector<Value>& args) {
    int id = g_next_timer_id++;
    TimerData& timer = g_timers[id];
    timer.startTime = GetTime();
    timer.isRunning = false;
    timer.isPaused = false;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Timer");
    obj["_id"] = Value::from_int(id);
    obj["elapsed"] = Value::from_number(0.0);
    obj["isRunning"] = Value::from_bool(false);
    obj["isPaused"] = Value::from_bool(false);
    
    return Value::from_map(std::move(obj));
}

// Timer.start()
static Value timer_start(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = idIt->second.as_int();
    auto timerIt = g_timers.find(id);
    if (timerIt == g_timers.end()) {
        return Value::nil();
    }
    
    TimerData& timer = timerIt->second;
    if (timer.isPaused) {
        timer.startTime = GetTime() - timer.pausedTime;
        timer.isPaused = false;
    } else {
        timer.startTime = GetTime();
    }
    timer.isRunning = true;
    
    Value::Map updated = map;
    updated["isRunning"] = Value::from_bool(true);
    updated["isPaused"] = Value::from_bool(false);
    return Value::from_map(std::move(updated));
}

// Timer.stop()
static Value timer_stop(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto timerIt = g_timers.find(id);
        if (timerIt != g_timers.end()) {
            timerIt->second.isRunning = false;
            timerIt->second.isPaused = false;
        }
    }
    
    Value::Map updated = map;
    updated["isRunning"] = Value::from_bool(false);
    updated["isPaused"] = Value::from_bool(false);
    return Value::from_map(std::move(updated));
}

// Timer.pause()
static Value timer_pause(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = idIt->second.as_int();
    auto timerIt = g_timers.find(id);
    if (timerIt == g_timers.end()) {
        return Value::nil();
    }
    
    TimerData& timer = timerIt->second;
    if (timer.isRunning && !timer.isPaused) {
        timer.pausedTime = GetTime() - timer.startTime;
        timer.isPaused = true;
    }
    
    Value::Map updated = map;
    updated["isPaused"] = Value::from_bool(true);
    return Value::from_map(std::move(updated));
}

// Timer.elapsed() - returns elapsed time in seconds
static Value timer_elapsed(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_number(0.0);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_number(0.0);
    }
    
    int id = idIt->second.as_int();
    auto timerIt = g_timers.find(id);
    if (timerIt == g_timers.end()) {
        return Value::from_number(0.0);
    }
    
    TimerData& timer = timerIt->second;
    double elapsed = 0.0;
    
    if (timer.isPaused) {
        elapsed = timer.pausedTime;
    } else if (timer.isRunning) {
        elapsed = GetTime() - timer.startTime;
    }
    
    return Value::from_number(elapsed);
}

// Clock.getTime() - returns current time as string
static Value clock_getTime(const std::vector<Value>& args) {
    (void)args;
    double time = GetTime();
    int hours = static_cast<int>(time) / 3600;
    int minutes = (static_cast<int>(time) % 3600) / 60;
    int seconds = static_cast<int>(time) % 60;
    
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", hours, minutes, seconds);
    return Value::from_string(buffer);
}

// Clock.getDelta() - returns frame delta time
static Value clock_getDelta(const std::vector<Value>& args) {
    (void)args;
    return Value::from_number(g_delta_time);
}

// Clock.getFPS() - returns current FPS
static Value clock_getFPS(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetFPS());
}

// Register timer and clock functions
void register_timer_system(FunctionRegistry& R) {
    R.add("TIMER", NativeFn{"TIMER", 0, timer_constructor});
    R.add("TIMER_START", NativeFn{"TIMER_START", 1, timer_start});
    R.add("TIMER_STOP", NativeFn{"TIMER_STOP", 1, timer_stop});
    R.add("TIMER_PAUSE", NativeFn{"TIMER_PAUSE", 1, timer_pause});
    R.add("TIMER_ELAPSED", NativeFn{"TIMER_ELAPSED", 1, timer_elapsed});
    R.add("CLOCK_GETTIME", NativeFn{"CLOCK_GETTIME", 0, clock_getTime});
    R.add("CLOCK_GETDELTA", NativeFn{"CLOCK_GETDELTA", 0, clock_getDelta});
    R.add("CLOCK_GETFPS", NativeFn{"CLOCK_GETFPS", 0, clock_getFPS});
}

} // namespace bas

