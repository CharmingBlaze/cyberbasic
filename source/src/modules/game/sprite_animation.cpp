#include "bas/sprite_animation.hpp"
#include <algorithm>

namespace bas {

// Static member definitions
int SpriteAnimationSystem::next_sprite_id = 1;
int SpriteAnimationSystem::next_animation_id = 1;
std::unordered_map<int, std::shared_ptr<Sprite>> SpriteAnimationSystem::sprites;
std::unordered_map<int, std::shared_ptr<Animation>> SpriteAnimationSystem::animations;

void Animation::add_frame(int frame_index, double x, double y, double width, double height, double duration) {
    frames.emplace_back(frame_index, x, y, width, height, duration);
}

void Animation::update(double delta_time) {
    if (!playing || frames.empty()) return;
    
    current_time += delta_time;
    
    while (current_time >= frames[current_frame].duration) {
        current_time -= frames[current_frame].duration;
        current_frame++;
        
        if (current_frame >= static_cast<int>(frames.size())) {
            if (loop) {
                current_frame = 0;
            } else {
                current_frame = static_cast<int>(frames.size()) - 1;
                playing = false;
                break;
            }
        }
    }
}

AnimationFrame Animation::get_current_frame_data() const {
    if (frames.empty()) {
        return AnimationFrame(0, 0, 0, 0, 0, 0);
    }
    return frames[current_frame];
}

// SpriteAnimationSystem implementation
int SpriteAnimationSystem::create_sprite(const std::string& name, int texture_id, double width, double height) {
    int id = next_sprite_id++;
    auto sprite = std::make_shared<Sprite>(id, name, texture_id, width, height);
    sprites[id] = sprite;
    return id;
}

std::shared_ptr<Sprite> SpriteAnimationSystem::get_sprite(int sprite_id) {
    auto it = sprites.find(sprite_id);
    return (it != sprites.end()) ? it->second : nullptr;
}

void SpriteAnimationSystem::destroy_sprite(int sprite_id) {
    sprites.erase(sprite_id);
}

int SpriteAnimationSystem::create_animation(const std::string& name, int sprite_id) {
    int id = next_animation_id++;
    auto animation = std::make_shared<Animation>(id, name, sprite_id);
    animations[id] = animation;
    return id;
}

std::shared_ptr<Animation> SpriteAnimationSystem::get_animation(int animation_id) {
    auto it = animations.find(animation_id);
    return (it != animations.end()) ? it->second : nullptr;
}

void SpriteAnimationSystem::destroy_animation(int animation_id) {
    animations.erase(animation_id);
}

void SpriteAnimationSystem::play_animation(int sprite_id, int animation_id) {
    auto sprite = get_sprite(sprite_id);
    auto animation = get_animation(animation_id);
    
    if (sprite && animation && animation->get_sprite_id() == sprite_id) {
        sprite->current_animation_id = animation_id;
        animation->play();
    }
}

void SpriteAnimationSystem::stop_animation(int sprite_id) {
    auto sprite = get_sprite(sprite_id);
    if (sprite && sprite->current_animation_id != -1) {
        auto animation = get_animation(sprite->current_animation_id);
        if (animation) {
            animation->stop();
        }
        sprite->current_animation_id = -1;
    }
}

void SpriteAnimationSystem::pause_animation(int sprite_id) {
    auto sprite = get_sprite(sprite_id);
    if (sprite && sprite->current_animation_id != -1) {
        auto animation = get_animation(sprite->current_animation_id);
        if (animation) {
            animation->pause();
        }
    }
}

void SpriteAnimationSystem::resume_animation(int sprite_id) {
    auto sprite = get_sprite(sprite_id);
    if (sprite && sprite->current_animation_id != -1) {
        auto animation = get_animation(sprite->current_animation_id);
        if (animation) {
            animation->resume();
        }
    }
}

void SpriteAnimationSystem::update_all_animations(double delta_time) {
    for (auto& pair : animations) {
        pair.second->update(delta_time);
    }
}

void SpriteAnimationSystem::shutdown() {
    sprites.clear();
    animations.clear();
}

} // namespace bas
