#include "bas/tween_system.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>

namespace bas {

// Easing functions
namespace easing {
    // Linear
    static double linear(double t) { return t; }
    
    // Ease In
    static double easeInQuad(double t) { return t * t; }
    static double easeInCubic(double t) { return t * t * t; }
    static double easeInQuart(double t) { return t * t * t * t; }
    static double easeInQuint(double t) { return t * t * t * t * t; }
    static double easeInSine(double t) { return 1.0 - std::cos((t * 3.14159265359) / 2.0); }
    static double easeInExpo(double t) { return t == 0.0 ? 0.0 : std::pow(2.0, 10.0 * (t - 1.0)); }
    static double easeInCirc(double t) { return 1.0 - std::sqrt(1.0 - t * t); }
    static double easeInBack(double t) {
        const double c1 = 1.70158;
        const double c3 = c1 + 1.0;
        return c3 * t * t * t - c1 * t * t;
    }
    static double easeInElastic(double t) {
        if (t == 0.0) return 0.0;
        if (t == 1.0) return 1.0;
        const double c4 = (2.0 * 3.14159265359) / 3.0;
        return -std::pow(2.0, 10.0 * t - 10.0) * std::sin((t * 10.0 - 10.75) * c4);
    }
    static double easeOutBounce(double t);
    // Ease Out
    static double easeOutQuad(double t) { return 1.0 - (1.0 - t) * (1.0 - t); }
    static double easeOutCubic(double t) { return 1.0 - std::pow(1.0 - t, 3.0); }
    static double easeOutQuart(double t) { return 1.0 - std::pow(1.0 - t, 4.0); }
    static double easeOutQuint(double t) { return 1.0 - std::pow(1.0 - t, 5.0); }
    static double easeOutSine(double t) { return std::sin((t * 3.14159265359) / 2.0); }
    static double easeOutExpo(double t) { return t == 1.0 ? 1.0 : 1.0 - std::pow(2.0, -10.0 * t); }
    static double easeOutCirc(double t) { return std::sqrt(1.0 - std::pow(t - 1.0, 2.0)); }
    static double easeOutBack(double t) {
        const double c1 = 1.70158;
        const double c3 = c1 + 1.0;
        return 1.0 + c3 * std::pow(t - 1.0, 3.0) + c1 * std::pow(t - 1.0, 2.0);
    }
    static double easeOutElastic(double t) {
        if (t == 0.0) return 0.0;
        if (t == 1.0) return 1.0;
        const double c4 = (2.0 * 3.14159265359) / 3.0;
        return std::pow(2.0, -10.0 * t) * std::sin((t * 10.0 - 0.75) * c4) + 1.0;
    }
    static double easeOutBounce(double t) {
        const double n1 = 7.5625;
        const double d1 = 2.75;
        if (t < 1.0 / d1) {
            return n1 * t * t;
        } else if (t < 2.0 / d1) {
            t = t - (1.5 / d1);
            return n1 * t * t + 0.75;
        } else if (t < 2.5 / d1) {
            t = t - (2.25 / d1);
            return n1 * t * t + 0.9375;
        } else {
            t = t - (2.625 / d1);
            return n1 * t * t + 0.984375;
        }
    }
    
    static double easeInBounce(double t) {
        return 1.0 - easeOutBounce(1.0 - t);
    }
    
    // Ease In-Out
    static double easeInOutQuad(double t) {
        return t < 0.5 ? 2.0 * t * t : 1.0 - std::pow(-2.0 * t + 2.0, 2.0) / 2.0;
    }
    static double easeInOutCubic(double t) {
        return t < 0.5 ? 4.0 * t * t * t : 1.0 - std::pow(-2.0 * t + 2.0, 3.0) / 2.0;
    }
    static double easeInOutQuart(double t) {
        return t < 0.5 ? 8.0 * t * t * t * t : 1.0 - std::pow(-2.0 * t + 2.0, 4.0) / 2.0;
    }
    static double easeInOutQuint(double t) {
        return t < 0.5 ? 16.0 * t * t * t * t * t : 1.0 - std::pow(-2.0 * t + 2.0, 5.0) / 2.0;
    }
    static double easeInOutSine(double t) {
        return -(std::cos(3.14159265359 * t) - 1.0) / 2.0;
    }
    static double easeInOutExpo(double t) {
        return t == 0.0 ? 0.0 : t == 1.0 ? 1.0 : t < 0.5 
            ? std::pow(2.0, 20.0 * t - 10.0) / 2.0
            : (2.0 - std::pow(2.0, -20.0 * t + 10.0)) / 2.0;
    }
    static double easeInOutCirc(double t) {
        return t < 0.5
            ? (1.0 - std::sqrt(1.0 - std::pow(2.0 * t, 2.0))) / 2.0
            : (std::sqrt(1.0 - std::pow(-2.0 * t + 2.0, 2.0)) + 1.0) / 2.0;
    }
    static double easeInOutBack(double t) {
        const double c1 = 1.70158;
        const double c2 = c1 * 1.525;
        return t < 0.5
            ? (std::pow(2.0 * t, 2.0) * ((c2 + 1.0) * 2.0 * t - c2)) / 2.0
            : (std::pow(2.0 * t - 2.0, 2.0) * ((c2 + 1.0) * (t * 2.0 - 2.0) + c2) + 2.0) / 2.0;
    }
    static double easeInOutElastic(double t) {
        if (t == 0.0) return 0.0;
        if (t == 1.0) return 1.0;
        const double c5 = (2.0 * 3.14159265359) / 4.5;
        return t < 0.5
            ? -(std::pow(2.0, 20.0 * t - 10.0) * std::sin((20.0 * t - 11.125) * c5)) / 2.0
            : (std::pow(2.0, -20.0 * t + 10.0) * std::sin((20.0 * t - 11.125) * c5)) / 2.0 + 1.0;
    }
    static double easeInOutBounce(double t) {
        return t < 0.5
            ? (1.0 - easeOutBounce(1.0 - 2.0 * t)) / 2.0
            : (1.0 + easeOutBounce(2.0 * t - 1.0)) / 2.0;
    }
}

// Tween data structure
struct TweenData {
    std::string targetId;  // Object ID or property path
    std::string property;
    double startValue;
    double endValue;
    double currentValue;
    double duration;
    double elapsed;
    std::string easingType;
    bool isActive;
    bool isPaused;
    bool isComplete;
    
    TweenData() : startValue(0.0), endValue(0.0), currentValue(0.0), 
                  duration(1.0), elapsed(0.0), easingType("linear"),
                  isActive(false), isPaused(false), isComplete(false) {}
};

static std::unordered_map<int, TweenData> g_tweens;
static int g_next_tween_id = 1;

// Get easing function by name
static double (*get_easing_func(const std::string& name))(double) {
    if (name == "linear") return easing::linear;
    if (name == "easeInQuad") return easing::easeInQuad;
    if (name == "easeInCubic") return easing::easeInCubic;
    if (name == "easeInQuart") return easing::easeInQuart;
    if (name == "easeInQuint") return easing::easeInQuint;
    if (name == "easeInSine") return easing::easeInSine;
    if (name == "easeInExpo") return easing::easeInExpo;
    if (name == "easeInCirc") return easing::easeInCirc;
    if (name == "easeInBack") return easing::easeInBack;
    if (name == "easeInElastic") return easing::easeInElastic;
    if (name == "easeInBounce") return easing::easeInBounce;
    if (name == "easeOutQuad") return easing::easeOutQuad;
    if (name == "easeOutCubic") return easing::easeOutCubic;
    if (name == "easeOutQuart") return easing::easeOutQuart;
    if (name == "easeOutQuint") return easing::easeOutQuint;
    if (name == "easeOutSine") return easing::easeOutSine;
    if (name == "easeOutExpo") return easing::easeOutExpo;
    if (name == "easeOutCirc") return easing::easeOutCirc;
    if (name == "easeOutBack") return easing::easeOutBack;
    if (name == "easeOutElastic") return easing::easeOutElastic;
    if (name == "easeOutBounce") return easing::easeOutBounce;
    if (name == "easeInOutQuad") return easing::easeInOutQuad;
    if (name == "easeInOutCubic") return easing::easeInOutCubic;
    if (name == "easeInOutQuart") return easing::easeInOutQuart;
    if (name == "easeInOutQuint") return easing::easeInOutQuint;
    if (name == "easeInOutSine") return easing::easeInOutSine;
    if (name == "easeInOutExpo") return easing::easeInOutExpo;
    if (name == "easeInOutCirc") return easing::easeInOutCirc;
    if (name == "easeInOutBack") return easing::easeInOutBack;
    if (name == "easeInOutElastic") return easing::easeInOutElastic;
    if (name == "easeInOutBounce") return easing::easeInOutBounce;
    return easing::linear;
}

// Tween.to(target, property, endValue, duration, [easing])
static Value tween_to(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::from_int(-1);
    }
    
    int id = g_next_tween_id++;
    TweenData& tween = g_tweens[id];
    
    // Get target object
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end()) {
            tween.targetId = std::to_string(idIt->second.as_int());
        }
    } else {
        tween.targetId = args[0].as_string();
    }
    
    tween.property = args[1].as_string();
    tween.endValue = args[2].as_number();
    tween.duration = args[3].as_number();
    tween.easingType = args.size() > 4 ? args[4].as_string() : "linear";
    
    // Get current value from target (simplified - would need object lookup)
    tween.startValue = 0.0;  // Would get from target object
    tween.currentValue = tween.startValue;
    tween.elapsed = 0.0;
    tween.isActive = true;
    tween.isPaused = false;
    tween.isComplete = false;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Tween");
    obj["_id"] = Value::from_int(id);
    obj["isActive"] = Value::from_bool(true);
    obj["isComplete"] = Value::from_bool(false);
    
    return Value::from_map(std::move(obj));
}

// Tween.from(target, property, startValue, endValue, duration, [easing])
static Value tween_from(const std::vector<Value>& args) {
    if (args.size() < 5) {
        return Value::from_int(-1);
    }
    
    int id = g_next_tween_id++;
    TweenData& tween = g_tweens[id];
    
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end()) {
            tween.targetId = std::to_string(idIt->second.as_int());
        }
    } else {
        tween.targetId = args[0].as_string();
    }
    
    tween.property = args[1].as_string();
    tween.startValue = args[2].as_number();
    tween.endValue = args[3].as_number();
    tween.duration = args[4].as_number();
    tween.easingType = args.size() > 5 ? args[5].as_string() : "linear";
    
    tween.currentValue = tween.startValue;
    tween.elapsed = 0.0;
    tween.isActive = true;
    tween.isPaused = false;
    tween.isComplete = false;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Tween");
    obj["_id"] = Value::from_int(id);
    obj["isActive"] = Value::from_bool(true);
    obj["isComplete"] = Value::from_bool(false);
    
    return Value::from_map(std::move(obj));
}

// Tween.update(deltaTime) - Update all active tweens
static Value tween_update(const std::vector<Value>& args) {
    double deltaTime = args.size() > 0 ? args[0].as_number() : 0.016;  // Default 60fps
    
    for (auto& pair : g_tweens) {
        TweenData& tween = pair.second;
        if (!tween.isActive || tween.isPaused || tween.isComplete) {
            continue;
        }
        
        tween.elapsed += deltaTime;
        if (tween.elapsed >= tween.duration) {
            tween.elapsed = tween.duration;
            tween.currentValue = tween.endValue;
            tween.isComplete = true;
            tween.isActive = false;
        } else {
            double t = tween.elapsed / tween.duration;
            double (*ease_func)(double) = get_easing_func(tween.easingType);
            double eased = ease_func(t);
            tween.currentValue = tween.startValue + (tween.endValue - tween.startValue) * eased;
        }
    }
    
    return Value::nil();
}

// Tween.stop(tweenId)
static Value tween_stop(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tweens.find(id);
    if (it != g_tweens.end()) {
        it->second.isActive = false;
        it->second.isComplete = true;
    }
    
    return Value::nil();
}

// Tween.pause(tweenId)
static Value tween_pause(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tweens.find(id);
    if (it != g_tweens.end()) {
        it->second.isPaused = true;
    }
    
    return Value::nil();
}

// Tween.resume(tweenId)
static Value tween_resume(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tweens.find(id);
    if (it != g_tweens.end()) {
        it->second.isPaused = false;
    }
    
    return Value::nil();
}

// Tween.getValue(tweenId) - Get current tween value
static Value tween_getValue(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_number(0.0);
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tweens.find(id);
    if (it != g_tweens.end()) {
        return Value::from_number(it->second.currentValue);
    }
    
    return Value::from_number(0.0);
}

// Tween.isComplete(tweenId)
static Value tween_isComplete(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_tweens.find(id);
    if (it != g_tweens.end()) {
        return Value::from_bool(it->second.isComplete);
    }
    
    return Value::from_bool(false);
}

void register_tween_system(FunctionRegistry& registry) {
    registry.add("TWEEN_TO", NativeFn{"TWEEN_TO", 5, tween_to});
    registry.add("TWEEN_FROM", NativeFn{"TWEEN_FROM", 6, tween_from});
    registry.add("TWEEN_UPDATE", NativeFn{"TWEEN_UPDATE", 1, tween_update});
    registry.add("TWEEN_STOP", NativeFn{"TWEEN_STOP", 1, tween_stop});
    registry.add("TWEEN_PAUSE", NativeFn{"TWEEN_PAUSE", 1, tween_pause});
    registry.add("TWEEN_RESUME", NativeFn{"TWEEN_RESUME", 1, tween_resume});
    registry.add("TWEEN_GETVALUE", NativeFn{"TWEEN_GETVALUE", 1, tween_getValue});
    registry.add("TWEEN_ISCOMPLETE", NativeFn{"TWEEN_ISCOMPLETE", 1, tween_isComplete});
}

}

