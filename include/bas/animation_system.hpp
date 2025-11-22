#pragma once
#include "runtime.hpp"
#include "value.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace bas {

struct AnimationFrame {
    int frameIndex{0};
    double duration{0.1};
};

struct AnimationData {
    std::string name;
    std::vector<AnimationFrame> frames;
    int currentFrame{0};
    double currentTime{0.0};
    bool isPlaying{false};
    bool isLooping{true};
    double speed{1.0};
    int frameWidth{0};
    int frameHeight{0};
    int totalFrames{0};
};

extern std::unordered_map<int, AnimationData> g_animations;

void register_animation_system(FunctionRegistry& R);

} // namespace bas

