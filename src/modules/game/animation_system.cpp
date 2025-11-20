#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>

namespace bas {

// Animation frame data
struct AnimationFrame {
    int frameIndex{0};
    double duration{0.1};  // Duration in seconds
};

// Animation object storage
struct AnimationData {
    std::string name;
    std::vector<AnimationFrame> frames;
    int currentFrame{0};
    double currentTime{0.0};
    bool isPlaying{false};
    bool isLooping{true};
    double speed{1.0};  // Playback speed multiplier
    int frameWidth{0};
    int frameHeight{0};
    int totalFrames{0};
};

static std::unordered_map<int, AnimationData> g_animations;
static int g_next_animation_id = 1;

// Animation constructor: Animation(name, frameCount, frameWidth, frameHeight, frameDuration)
static Value animation_constructor(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::nil();
    }
    
    std::string name = args[0].is_string() ? args[0].as_string() : "Animation";
    int frameCount = args[1].as_int();
    int frameWidth = args[2].as_int();
    int frameHeight = args[3].as_int();
    double frameDuration = args.size() > 4 ? args[4].as_number() : 0.1;
    
    if (frameCount < 1 || frameWidth < 1 || frameHeight < 1) {
        return Value::nil();
    }
    
    int id = g_next_animation_id++;
    AnimationData& anim = g_animations[id];
    anim.name = name;
    anim.frameWidth = frameWidth;
    anim.frameHeight = frameHeight;
    anim.totalFrames = frameCount;
    anim.speed = 1.0;
    anim.isLooping = true;
    
    // Create frames
    anim.frames.resize(frameCount);
    for (int i = 0; i < frameCount; ++i) {
        anim.frames[i].frameIndex = i;
        anim.frames[i].duration = frameDuration;
    }
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Animation");
    obj["_id"] = Value::from_int(id);
    obj["name"] = Value::from_string(name);
    obj["frameCount"] = Value::from_int(frameCount);
    obj["frameWidth"] = Value::from_int(frameWidth);
    obj["frameHeight"] = Value::from_int(frameHeight);
    obj["currentFrame"] = Value::from_int(0);
    obj["isPlaying"] = Value::from_bool(false);
    obj["isLooping"] = Value::from_bool(true);
    obj["speed"] = Value::from_number(1.0);
    
    return Value::from_map(std::move(obj));
}

// Animation.play()
static Value animation_play(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int id = idIt->second.as_int();
    auto animIt = g_animations.find(id);
    if (animIt == g_animations.end()) {
        return args[0];
    }
    
    AnimationData& anim = animIt->second;
    anim.isPlaying = true;
    anim.currentTime = 0.0;
    
    Value::Map updated = map;
    updated["isPlaying"] = Value::from_bool(true);
    updated["currentFrame"] = Value::from_int(0);
    return Value::from_map(std::move(updated));
}

// Animation.stop()
static Value animation_stop(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto animIt = g_animations.find(id);
        if (animIt != g_animations.end()) {
            animIt->second.isPlaying = false;
            animIt->second.currentFrame = 0;
            animIt->second.currentTime = 0.0;
        }
    }
    
    Value::Map updated = map;
    updated["isPlaying"] = Value::from_bool(false);
    updated["currentFrame"] = Value::from_int(0);
    return Value::from_map(std::move(updated));
}

// Animation.pause()
static Value animation_pause(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto animIt = g_animations.find(id);
        if (animIt != g_animations.end()) {
            animIt->second.isPlaying = false;
        }
    }
    
    Value::Map updated = map;
    updated["isPlaying"] = Value::from_bool(false);
    return Value::from_map(std::move(updated));
}

// Animation.setSpeed(speed)
static Value animation_setSpeed(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    double speed = args[1].as_number();
    if (speed < 0.0) speed = 0.0;
    
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto animIt = g_animations.find(id);
        if (animIt != g_animations.end()) {
            animIt->second.speed = speed;
        }
    }
    
    Value::Map updated = map;
    updated["speed"] = Value::from_number(speed);
    return Value::from_map(std::move(updated));
}

// Animation.setLooping(looping)
static Value animation_setLooping(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    bool looping = args[1].as_bool();
    
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto animIt = g_animations.find(id);
        if (animIt != g_animations.end()) {
            animIt->second.isLooping = looping;
        }
    }
    
    Value::Map updated = map;
    updated["isLooping"] = Value::from_bool(looping);
    return Value::from_map(std::move(updated));
}

// Animation.getCurrentFrame() - returns current frame index
static Value animation_getCurrentFrame(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_int(0);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_int(0);
    }
    
    int id = idIt->second.as_int();
    auto animIt = g_animations.find(id);
    if (animIt == g_animations.end()) {
        return Value::from_int(0);
    }
    
    return Value::from_int(animIt->second.currentFrame);
}

// Animation.update(deltaTime) - updates animation frame (should be called each frame)
static Value animation_update(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    double deltaTime = args[1].as_number();
    
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int id = idIt->second.as_int();
    auto animIt = g_animations.find(id);
    if (animIt == g_animations.end()) {
        return args[0];
    }
    
    AnimationData& anim = animIt->second;
    if (!anim.isPlaying || anim.frames.empty()) {
        return args[0];
    }
    
    // Update animation time
    anim.currentTime += deltaTime * anim.speed;
    
    // Find current frame based on time
    double accumulatedTime = 0.0;
    int newFrame = 0;
    for (size_t i = 0; i < anim.frames.size(); ++i) {
        accumulatedTime += anim.frames[i].duration;
        if (anim.currentTime < accumulatedTime) {
            newFrame = static_cast<int>(i);
            break;
        }
        newFrame = static_cast<int>(i);
    }
    
    // Handle looping
    if (anim.currentTime >= accumulatedTime) {
        if (anim.isLooping) {
            anim.currentTime = 0.0;
            newFrame = 0;
        } else {
            anim.isPlaying = false;
            newFrame = static_cast<int>(anim.frames.size() - 1);
        }
    }
    
    anim.currentFrame = newFrame;
    
    Value::Map updated = map;
    updated["currentFrame"] = Value::from_int(newFrame);
    updated["isPlaying"] = Value::from_bool(anim.isPlaying);
    return Value::from_map(std::move(updated));
}

// Animation.getFrameRect() - returns Rectangle for current frame (for sprite sheet)
static Value animation_getFrameRect(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = idIt->second.as_int();
    auto animIt = g_animations.find(id);
    if (animIt == g_animations.end()) {
        return Value::nil();
    }
    
    AnimationData& anim = animIt->second;
    int frameX = anim.currentFrame * anim.frameWidth;
    int frameY = 0;  // Assuming single row sprite sheet
    
    Value::Map rect;
    rect["_type"] = Value::from_string("Rectangle");
    rect["x"] = Value::from_number(frameX);
    rect["y"] = Value::from_number(frameY);
    rect["width"] = Value::from_int(anim.frameWidth);
    rect["height"] = Value::from_int(anim.frameHeight);
    
    return Value::from_map(std::move(rect));
}

// Register animation system functions
void register_animation_system(FunctionRegistry& R) {
    R.add("ANIMATION", NativeFn{"ANIMATION", -1, animation_constructor});
    R.add("ANIMATION_PLAY", NativeFn{"ANIMATION_PLAY", 1, animation_play});
    R.add("ANIMATION_STOP", NativeFn{"ANIMATION_STOP", 1, animation_stop});
    R.add("ANIMATION_PAUSE", NativeFn{"ANIMATION_PAUSE", 1, animation_pause});
    R.add("ANIMATION_SETSPEED", NativeFn{"ANIMATION_SETSPEED", 2, animation_setSpeed});
    R.add("ANIMATION_SETLOOPING", NativeFn{"ANIMATION_SETLOOPING", 2, animation_setLooping});
    R.add("ANIMATION_GETCURRENTFRAME", NativeFn{"ANIMATION_GETCURRENTFRAME", 1, animation_getCurrentFrame});
    R.add("ANIMATION_UPDATE", NativeFn{"ANIMATION_UPDATE", 2, animation_update});
    R.add("ANIMATION_GETFRAMERECT", NativeFn{"ANIMATION_GETFRAMERECT", 1, animation_getFrameRect});
}

} // namespace bas

