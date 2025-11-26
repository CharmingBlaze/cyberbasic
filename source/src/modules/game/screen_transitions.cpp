#include "bas/screen_transitions.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <cmath>
#include <algorithm>

namespace bas {

// Transition data
enum class TransitionType {
    FADE,
    SLIDE_LEFT,
    SLIDE_RIGHT,
    SLIDE_UP,
    SLIDE_DOWN,
    WIPE_LEFT,
    WIPE_RIGHT,
    CIRCLE,
    SQUARE
};

struct TransitionData {
    TransitionType type;
    float duration;
    float elapsed;
    bool isActive;
    bool isIn;  // true = fade in, false = fade out
    Color color;
    int id;
    
    TransitionData() : type(TransitionType::FADE), duration(1.0f), elapsed(0.0f), 
                      isActive(false), isIn(true), 
                      color({0, 0, 0, 255}), id(0) {}
};

static std::unordered_map<int, TransitionData> g_transitions;
static int g_next_transition_id = 1;

// TRANSITION.fade(duration, [color], [isIn]) -> transitionId
static Value transition_fade(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_int(-1);
    }
    
    int id = g_next_transition_id++;
    TransitionData& trans = g_transitions[id];
    trans.type = TransitionType::FADE;
    trans.duration = static_cast<float>(args[0].as_number());
    trans.elapsed = 0.0f;
    trans.isActive = true;
    trans.isIn = args.size() > 2 ? args[2].as_bool() : true;
    
    if (args.size() > 1 && args[1].is_map()) {
        // Color object
        const auto& colorMap = args[1].as_map();
        trans.color.r = static_cast<unsigned char>(colorMap.at("r").as_number());
        trans.color.g = static_cast<unsigned char>(colorMap.at("g").as_number());
        trans.color.b = static_cast<unsigned char>(colorMap.at("b").as_number());
        trans.color.a = static_cast<unsigned char>(colorMap.at("a").as_number());
    } else {
        trans.color = BLACK;
    }
    
    return Value::from_int(id);
}

// TRANSITION.slide(direction, duration, [isIn]) -> transitionId
static Value transition_slide(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_int(-1);
    }
    
    int id = g_next_transition_id++;
    TransitionData& trans = g_transitions[id];
    trans.duration = static_cast<float>(args[1].as_number());
    trans.elapsed = 0.0f;
    trans.isActive = true;
    trans.isIn = args.size() > 2 ? args[2].as_bool() : true;
    trans.color = BLACK;
    
    std::string direction = args[0].as_string();
    if (direction == "left") {
        trans.type = TransitionType::SLIDE_LEFT;
    } else if (direction == "right") {
        trans.type = TransitionType::SLIDE_RIGHT;
    } else if (direction == "up") {
        trans.type = TransitionType::SLIDE_UP;
    } else if (direction == "down") {
        trans.type = TransitionType::SLIDE_DOWN;
    } else {
        trans.type = TransitionType::SLIDE_LEFT;
    }
    
    return Value::from_int(id);
}

// TRANSITION.wipe(direction, duration, [isIn]) -> transitionId
static Value transition_wipe(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_int(-1);
    }
    
    int id = g_next_transition_id++;
    TransitionData& trans = g_transitions[id];
    trans.duration = static_cast<float>(args[1].as_number());
    trans.elapsed = 0.0f;
    trans.isActive = true;
    trans.isIn = args.size() > 2 ? args[2].as_bool() : true;
    trans.color = BLACK;
    
    std::string direction = args[0].as_string();
    if (direction == "left") {
        trans.type = TransitionType::WIPE_LEFT;
    } else {
        trans.type = TransitionType::WIPE_RIGHT;
    }
    
    return Value::from_int(id);
}

// TRANSITION.update(transitionId, deltaTime) -> bool (isComplete)
static Value transition_update(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_bool(false);
    }
    
    int id = static_cast<int>(args[0].as_int());
    float deltaTime = static_cast<float>(args[1].as_number());
    
    auto it = g_transitions.find(id);
    if (it == g_transitions.end() || !it->second.isActive) {
        return Value::from_bool(true);
    }
    
    TransitionData& trans = it->second;
    trans.elapsed += deltaTime;
    
    if (trans.elapsed >= trans.duration) {
        trans.elapsed = trans.duration;
        trans.isActive = false;
        return Value::from_bool(true);
    }
    
    return Value::from_bool(false);
}

// TRANSITION.draw(transitionId) - Draw transition effect
static Value transition_draw(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_transitions.find(id);
    if (it == g_transitions.end() || !it->second.isActive) {
        return Value::nil();
    }
    
    TransitionData& trans = it->second;
    float t = trans.elapsed / trans.duration;
    if (!trans.isIn) {
        t = 1.0f - t;
    }
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    Color drawColor = trans.color;
    drawColor.a = static_cast<unsigned char>(255.0f * t);
    
    switch (trans.type) {
        case TransitionType::FADE: {
            DrawRectangle(0, 0, screenWidth, screenHeight, drawColor);
            break;
        }
        case TransitionType::SLIDE_LEFT: {
            int offset = static_cast<int>((1.0f - t) * screenWidth);
            DrawRectangle(-offset, 0, screenWidth, screenHeight, trans.color);
            break;
        }
        case TransitionType::SLIDE_RIGHT: {
            int offset = static_cast<int>((1.0f - t) * screenWidth);
            DrawRectangle(offset, 0, screenWidth, screenHeight, trans.color);
            break;
        }
        case TransitionType::SLIDE_UP: {
            int offset = static_cast<int>((1.0f - t) * screenHeight);
            DrawRectangle(0, -offset, screenWidth, screenHeight, trans.color);
            break;
        }
        case TransitionType::SLIDE_DOWN: {
            int offset = static_cast<int>((1.0f - t) * screenHeight);
            DrawRectangle(0, offset, screenWidth, screenHeight, trans.color);
            break;
        }
        case TransitionType::WIPE_LEFT: {
            int width = static_cast<int>(t * screenWidth);
            DrawRectangle(0, 0, width, screenHeight, trans.color);
            break;
        }
        case TransitionType::WIPE_RIGHT: {
            int width = static_cast<int>(t * screenWidth);
            DrawRectangle(screenWidth - width, 0, width, screenHeight, trans.color);
            break;
        }
        default:
            break;
    }
    
    return Value::nil();
}

// TRANSITION.isComplete(transitionId) -> bool
static Value transition_isComplete(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_transitions.find(id);
    if (it == g_transitions.end()) {
        return Value::from_bool(true);
    }
    
    return Value::from_bool(!it->second.isActive);
}

// TRANSITION.stop(transitionId)
static Value transition_stop(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    int id = static_cast<int>(args[0].as_int());
    auto it = g_transitions.find(id);
    if (it != g_transitions.end()) {
        it->second.isActive = false;
    }
    
    return Value::nil();
}

void register_screen_transitions(FunctionRegistry& registry) {
    registry.add("TRANSITION_FADE", NativeFn{"TRANSITION_FADE", 3, transition_fade});
    registry.add("TRANSITION_SLIDE", NativeFn{"TRANSITION_SLIDE", 3, transition_slide});
    registry.add("TRANSITION_WIPE", NativeFn{"TRANSITION_WIPE", 3, transition_wipe});
    registry.add("TRANSITION_UPDATE", NativeFn{"TRANSITION_UPDATE", 2, transition_update});
    registry.add("TRANSITION_DRAW", NativeFn{"TRANSITION_DRAW", 1, transition_draw});
    registry.add("TRANSITION_ISCOMPLETE", NativeFn{"TRANSITION_ISCOMPLETE", 1, transition_isComplete});
    registry.add("TRANSITION_STOP", NativeFn{"TRANSITION_STOP", 1, transition_stop});
}

}

