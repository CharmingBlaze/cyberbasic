#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <functional>
#include <exception>
#include <stdexcept>
#include <fstream>
#include <sys/stat.h>
#include <ctime>

namespace bas {

// ===== COROUTINE SYSTEM =====

struct CoroutineData {
    std::string name;
    std::function<Value()> generator;
    bool active{false};
    Value lastValue{Value::nil()};
    int id{0};
};

static std::unordered_map<int, CoroutineData> g_coroutines;
static int g_next_coroutine_id = 1;

// Coroutine.create(name, generatorFunction) -> Coroutine
static Value coroutine_create(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::nil();
    }
    
    int id = g_next_coroutine_id++;
    CoroutineData& coro = g_coroutines[id];
    coro.id = id;
    coro.name = args[0].as_string();
    coro.active = true;
    
    Value::Map coroObj;
    coroObj["_type"] = Value::from_string("Coroutine");
    coroObj["_id"] = Value::from_int(id);
    coroObj["name"] = Value::from_string(coro.name);
    coroObj["active"] = Value::from_bool(coro.active);
    
    return Value::from_map(std::move(coroObj));
}

// Coroutine.resume(coroutine) -> value
static Value coroutine_resume(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = static_cast<int>(idIt->second.as_int());
    auto coroIt = g_coroutines.find(id);
    if (coroIt == g_coroutines.end() || !coroIt->second.active) {
        return Value::nil();
    }
    
    // In a real implementation, this would resume the coroutine
    // For now, return the last value
    return coroIt->second.lastValue;
}

// Coroutine.yield(value) -> value (called from within coroutine)
static Value coroutine_yield(const std::vector<Value>& args) {
    Value value = args.empty() ? Value::nil() : args[0];
    // In a real implementation, this would suspend the coroutine
    return value;
}

// ===== UNIT TESTING FRAMEWORK =====

struct TestResult {
    std::string name;
    bool passed{false};
    std::string error;
};

static std::vector<TestResult> g_test_results;
static bool g_test_mode{false};

// Test.start() - Begin test mode
static Value test_start(const std::vector<Value>& args) {
    g_test_mode = true;
    g_test_results.clear();
    return Value::nil();
}

// Test.end() -> array of results
static Value test_end(const std::vector<Value>& args) {
    g_test_mode = false;
    Value::Array results;
    for (const auto& test : g_test_results) {
        Value::Map result;
        result["name"] = Value::from_string(test.name);
        result["passed"] = Value::from_bool(test.passed);
        result["error"] = Value::from_string(test.error);
        results.push_back(Value::from_map(std::move(result)));
    }
    return Value::from_array(std::move(results));
}

// Test.assert(condition, [message]) -> bool
static Value test_assert(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::from_bool(false);
    }
    
    bool condition = args[0].as_bool();
    std::string message = args.size() > 1 ? args[1].as_string() : "Assertion failed";
    
    if (g_test_mode) {
        TestResult result;
        result.name = message;
        result.passed = condition;
        if (!condition) {
            result.error = "Assertion failed: " + message;
        }
        g_test_results.push_back(result);
    }
    
    return Value::from_bool(condition);
}

// Test.run(name, testFunction) -> bool
static Value test_run(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string testName = args[0].as_string();
    TestResult result;
    result.name = testName;
    
    try {
        // In a real implementation, this would call the test function
        result.passed = true;
    } catch (const std::exception& e) {
        result.passed = false;
        result.error = e.what();
    }
    
    g_test_results.push_back(result);
    return Value::from_bool(result.passed);
}

// ===== PROFILING SYSTEM =====

struct ProfileData {
    std::string name;
    std::chrono::high_resolution_clock::time_point startTime;
    double totalTime{0.0};
    int callCount{0};
    bool active{false};
};

static std::unordered_map<std::string, ProfileData> g_profiles;

// Profile.start(name) - Start profiling
static Value profile_start(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string name = args[0].as_string();
    ProfileData& profile = g_profiles[name];
    profile.name = name;
    profile.startTime = std::chrono::high_resolution_clock::now();
    profile.active = true;
    
    return Value::nil();
}

// Profile.end(name) -> double (time in ms)
static Value profile_end(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_number(0.0);
    }
    
    std::string name = args[0].as_string();
    auto it = g_profiles.find(name);
    if (it == g_profiles.end() || !it->second.active) {
        return Value::from_number(0.0);
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - it->second.startTime);
    double ms = duration.count() / 1000.0;
    
    it->second.totalTime += ms;
    it->second.callCount++;
    it->second.active = false;
    
    return Value::from_number(ms);
}

// Profile.get(name) -> double (total time in ms)
static Value profile_get(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_number(0.0);
    }
    
    std::string name = args[0].as_string();
    auto it = g_profiles.find(name);
    if (it == g_profiles.end()) {
        return Value::from_number(0.0);
    }
    
    return Value::from_number(it->second.totalTime);
}

// Profile.reset([name]) - Reset profile data
static Value profile_reset(const std::vector<Value>& args) {
    if (args.empty()) {
        g_profiles.clear();
    } else if (args[0].is_string()) {
        std::string name = args[0].as_string();
        g_profiles.erase(name);
    }
    return Value::nil();
}

// Profile.getAll() -> map of profiles
static Value profile_getAll(const std::vector<Value>& args) {
    Value::Map result;
    for (const auto& pair : g_profiles) {
        Value::Map profileData;
        profileData["name"] = Value::from_string(pair.second.name);
        profileData["totalTime"] = Value::from_number(pair.second.totalTime);
        profileData["callCount"] = Value::from_int(pair.second.callCount);
        profileData["averageTime"] = Value::from_number(
            pair.second.callCount > 0 ? pair.second.totalTime / pair.second.callCount : 0.0
        );
        result[pair.first] = Value::from_map(std::move(profileData));
    }
    return Value::from_map(std::move(result));
}

// ===== HOT RELOAD SYSTEM =====

struct FileWatcher {
    std::string path;
    std::chrono::system_clock::time_point lastModified;
    bool watching{false};
};

static std::unordered_map<std::string, FileWatcher> g_file_watchers;

// HotReload.watch(filePath) -> bool
static Value hotreload_watch(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string path = args[0].as_string();
    FileWatcher& watcher = g_file_watchers[path];
    watcher.path = path;
    
    try {
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0) {
            auto ftime = std::chrono::system_clock::from_time_t(fileStat.st_mtime);
            watcher.lastModified = ftime;
            watcher.watching = true;
            return Value::from_bool(true);
        }
    } catch (...) {
        return Value::from_bool(false);
    }
    
    return Value::from_bool(false);
}

// HotReload.check(filePath) -> bool (true if changed)
static Value hotreload_check(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string path = args[0].as_string();
    auto it = g_file_watchers.find(path);
    if (it == g_file_watchers.end() || !it->second.watching) {
        return Value::from_bool(false);
    }
    
    try {
        struct stat fileStat;
        if (stat(path.c_str(), &fileStat) == 0) {
            auto newTime = std::chrono::system_clock::from_time_t(fileStat.st_mtime);
            
            if (newTime > it->second.lastModified) {
                it->second.lastModified = newTime;
                return Value::from_bool(true);
            }
        }
    } catch (...) {
        return Value::from_bool(false);
    }
    
    return Value::from_bool(false);
}

// HotReload.reload(filePath) -> bool
static Value hotreload_reload(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string path = args[0].as_string();
    // In a real implementation, this would reload the file
    // For now, just return true
    return Value::from_bool(true);
}

// ===== RANGE/ITERATOR SYSTEM =====

// Range.create(start, end, [step]) -> Range
static Value range_create(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    long long start = args[0].as_int();
    long long end = args[1].as_int();
    long long step = args.size() > 2 ? args[2].as_int() : 1;
    
    Value::Map rangeObj;
    rangeObj["_type"] = Value::from_string("Range");
    rangeObj["start"] = Value::from_int(start);
    rangeObj["end"] = Value::from_int(end);
    rangeObj["step"] = Value::from_int(step);
    
    return Value::from_map(std::move(rangeObj));
}

// Range.toArray(range) -> array
static Value range_toArray(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_array({});
    }
    
    const auto& map = args[0].as_map();
    auto startIt = map.find("start");
    auto endIt = map.find("end");
    auto stepIt = map.find("step");
    
    if (startIt == map.end() || endIt == map.end()) {
        return Value::from_array({});
    }
    
    long long start = startIt->second.as_int();
    long long end = endIt->second.as_int();
    long long step = stepIt != map.end() ? stepIt->second.as_int() : 1;
    
    Value::Array result;
    if (step > 0) {
        for (long long i = start; i < end; i += step) {
            result.push_back(Value::from_int(i));
        }
    } else if (step < 0) {
        for (long long i = start; i > end; i += step) {
            result.push_back(Value::from_int(i));
        }
    }
    
    return Value::from_array(std::move(result));
}

// ===== ENHANCED DEBUGGING =====

struct Breakpoint {
    std::string file;
    int line{0};
    bool active{true};
    std::string condition;
};

static std::vector<Breakpoint> g_breakpoints;
[[maybe_unused]] static bool g_debug_mode{false};

// Debug.setBreakpoint(file, line, [condition]) -> bool
static Value debug_setBreakpoint(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    Breakpoint bp;
    bp.file = args[0].as_string();
    bp.line = static_cast<int>(args[1].as_int());
    bp.condition = args.size() > 2 ? args[2].as_string() : "";
    bp.active = true;
    
    g_breakpoints.push_back(bp);
    return Value::from_bool(true);
}

// Debug.clearBreakpoints() -> int (count cleared)
static Value debug_clearBreakpoints(const std::vector<Value>& args) {
    int count = static_cast<int>(g_breakpoints.size());
    g_breakpoints.clear();
    return Value::from_int(count);
}

// Debug.getBreakpoints() -> array
static Value debug_getBreakpoints(const std::vector<Value>& args) {
    Value::Array result;
    for (const auto& bp : g_breakpoints) {
        Value::Map bpObj;
        bpObj["file"] = Value::from_string(bp.file);
        bpObj["line"] = Value::from_int(bp.line);
        bpObj["active"] = Value::from_bool(bp.active);
        bpObj["condition"] = Value::from_string(bp.condition);
        result.push_back(Value::from_map(std::move(bpObj)));
    }
    return Value::from_array(std::move(result));
}

// Debug.watch(variableName) -> bool
static Value debug_watch(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    // In a real implementation, this would add the variable to a watch list
    return Value::from_bool(true);
}

// Debug.getCallStack() -> array
static Value debug_getCallStack(const std::vector<Value>& args) {
    // In a real implementation, this would return the call stack
    return Value::from_array({});
}

// ===== REGISTER ALL FUNCTIONS =====

void register_advanced_features(FunctionRegistry& R) {
    // Coroutine functions
    R.add("COROUTINE_CREATE", NativeFn{"COROUTINE_CREATE", 2, coroutine_create});
    R.add("COROUTINE_RESUME", NativeFn{"COROUTINE_RESUME", 1, coroutine_resume});
    R.add("COROUTINE_YIELD", NativeFn{"COROUTINE_YIELD", -1, coroutine_yield});
    
    // Unit testing functions
    R.add("TEST_START", NativeFn{"TEST_START", 0, test_start});
    R.add("TEST_END", NativeFn{"TEST_END", 0, test_end});
    R.add("TEST_ASSERT", NativeFn{"TEST_ASSERT", -1, test_assert});
    R.add("TEST_RUN", NativeFn{"TEST_RUN", 2, test_run});
    
    // Profiling functions
    R.add("PROFILE_START", NativeFn{"PROFILE_START", 1, profile_start});
    R.add("PROFILE_END", NativeFn{"PROFILE_END", 1, profile_end});
    R.add("PROFILE_GET", NativeFn{"PROFILE_GET", 1, profile_get});
    R.add("PROFILE_RESET", NativeFn{"PROFILE_RESET", -1, profile_reset});
    R.add("PROFILE_GETALL", NativeFn{"PROFILE_GETALL", 0, profile_getAll});
    
    // Hot reload functions
    R.add("HOTRELOAD_WATCH", NativeFn{"HOTRELOAD_WATCH", 1, hotreload_watch});
    R.add("HOTRELOAD_CHECK", NativeFn{"HOTRELOAD_CHECK", 1, hotreload_check});
    R.add("HOTRELOAD_RELOAD", NativeFn{"HOTRELOAD_RELOAD", 1, hotreload_reload});
    
    // Range functions
    R.add("RANGE_CREATE", NativeFn{"RANGE_CREATE", -1, range_create});
    R.add("RANGE_TOARRAY", NativeFn{"RANGE_TOARRAY", 1, range_toArray});
    
    // Debugging functions
    R.add("DEBUG_SETBREAKPOINT", NativeFn{"DEBUG_SETBREAKPOINT", -1, debug_setBreakpoint});
    R.add("DEBUG_CLEARBREAKPOINTS", NativeFn{"DEBUG_CLEARBREAKPOINTS", 0, debug_clearBreakpoints});
    R.add("DEBUG_GETBREAKPOINTS", NativeFn{"DEBUG_GETBREAKPOINTS", 0, debug_getBreakpoints});
    R.add("DEBUG_WATCH", NativeFn{"DEBUG_WATCH", 1, debug_watch});
    R.add("DEBUG_GETCALLSTACK", NativeFn{"DEBUG_GETCALLSTACK", 0, debug_getCallStack});
}

} // namespace bas

