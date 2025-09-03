#include "bas/sprite_animation.hpp"
#include "bas/runtime.hpp"

namespace bas {

void register_sprite_animation_functions(FunctionRegistry& R) {
    
    // === SPRITE MANAGEMENT FUNCTIONS ===
    
    R.add("CREATESPRITEANIMATION", NativeFn{"CREATESPRITEANIMATION", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("CREATESPRITEANIMATION: expected 4 args");
        std::string name = args[0].as_string();
        int texture_id = args[1].as_int();
        double width = args[2].as_number();
        double height = args[3].as_number();
        int sprite_id = SpriteAnimationSystem::create_sprite(name, texture_id, width, height);
        return Value::from_int(sprite_id);
    }});
    
    R.add("SETSPRITEANIMATIONORIGIN", NativeFn{"SETSPRITEANIMATIONORIGIN", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSPRITEANIMATIONORIGIN: expected 3 args");
        int sprite_id = args[0].as_int();
        double origin_x = args[1].as_number();
        double origin_y = args[2].as_number();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        if (sprite) {
            sprite->set_origin(origin_x, origin_y);
        }
        return Value::nil();
    }});
    
    R.add("SETSPRITEANIMATIONPOSITION", NativeFn{"SETSPRITEANIMATIONPOSITION", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETSPRITEPOSITION: expected 4 args");
        int sprite_id = args[0].as_int();
        double x = args[1].as_number();
        double y = args[2].as_number();
        double z = args[3].as_number();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        if (sprite) {
            sprite->set_position(x, y, z);
        }
        return Value::nil();
    }});
    
    R.add("SETSPRITEROTATION", NativeFn{"SETSPRITEROTATION", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSPRITEROTATION: expected 2 args");
        int sprite_id = args[0].as_int();
        double rotation = args[1].as_number();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        if (sprite) {
            sprite->set_rotation(rotation);
        }
        return Value::nil();
    }});
    
    R.add("SETSPRITESCALE", NativeFn{"SETSPRITESCALE", 3, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 3) throw std::runtime_error("SETSPRITESCALE: expected 3 args");
        int sprite_id = args[0].as_int();
        double scale_x = args[1].as_number();
        double scale_y = args[2].as_number();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        if (sprite) {
            sprite->set_scale(scale_x, scale_y);
        }
        return Value::nil();
    }});
    
    R.add("SETSPRITEVISIBLE", NativeFn{"SETSPRITEVISIBLE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETSPRITEVISIBLE: expected 2 args");
        int sprite_id = args[0].as_int();
        bool visible = args[1].as_bool();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        if (sprite) {
            sprite->set_visible(visible);
        }
        return Value::nil();
    }});
    
    R.add("GETSPRITEX", NativeFn{"GETSPRITEX", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETSPRITEX: expected 1 arg");
        int sprite_id = args[0].as_int();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        return sprite ? Value::from_number(sprite->get_x()) : Value::from_number(0.0);
    }});
    
    R.add("GETSPRITEY", NativeFn{"GETSPRITEY", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETSPRITEY: expected 1 arg");
        int sprite_id = args[0].as_int();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        return sprite ? Value::from_number(sprite->get_y()) : Value::from_number(0.0);
    }});
    
    R.add("GETSPRITEZ", NativeFn{"GETSPRITEZ", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETSPRITEZ: expected 1 arg");
        int sprite_id = args[0].as_int();
        auto sprite = SpriteAnimationSystem::get_sprite(sprite_id);
        return sprite ? Value::from_number(sprite->get_z()) : Value::from_number(0.0);
    }});
    
    R.add("DESTROYSPRITE", NativeFn{"DESTROYSPRITE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DESTROYSPRITE: expected 1 arg");
        int sprite_id = args[0].as_int();
        SpriteAnimationSystem::destroy_sprite(sprite_id);
        return Value::nil();
    }});
    
    // === ANIMATION FUNCTIONS ===
    
    R.add("CREATEANIMATION", NativeFn{"CREATEANIMATION", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("CREATEANIMATION: expected 2 args");
        std::string name = args[0].as_string();
        int sprite_id = args[1].as_int();
        int animation_id = SpriteAnimationSystem::create_animation(name, sprite_id);
        return Value::from_int(animation_id);
    }});
    
    R.add("ADDANIMATIONFRAME", NativeFn{"ADDANIMATIONFRAME", 6, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 6) throw std::runtime_error("ADDANIMATIONFRAME: expected 6 args");
        int animation_id = args[0].as_int();
        int frame_index = args[1].as_int();
        double x = args[2].as_number();
        double y = args[3].as_number();
        double width = args[4].as_number();
        double height = args[5].as_number();
        double duration = args[6].as_number();
        auto animation = SpriteAnimationSystem::get_animation(animation_id);
        if (animation) {
            animation->add_frame(frame_index, x, y, width, height, duration);
        }
        return Value::nil();
    }});
    
    R.add("SETANIMATIONLOOP", NativeFn{"SETANIMATIONLOOP", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETANIMATIONLOOP: expected 2 args");
        int animation_id = args[0].as_int();
        bool loop = args[1].as_bool();
        auto animation = SpriteAnimationSystem::get_animation(animation_id);
        if (animation) {
            animation->set_loop(loop);
        }
        return Value::nil();
    }});
    
    R.add("PLAYANIMATION", NativeFn{"PLAYANIMATION", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("PLAYANIMATION: expected 2 args");
        int sprite_id = args[0].as_int();
        int animation_id = args[1].as_int();
        SpriteAnimationSystem::play_animation(sprite_id, animation_id);
        return Value::nil();
    }});
    
    R.add("STOPANIMATION", NativeFn{"STOPANIMATION", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("STOPANIMATION: expected 1 arg");
        int sprite_id = args[0].as_int();
        SpriteAnimationSystem::stop_animation(sprite_id);
        return Value::nil();
    }});
    
    R.add("PAUSEANIMATION", NativeFn{"PAUSEANIMATION", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("PAUSEANIMATION: expected 1 arg");
        int sprite_id = args[0].as_int();
        SpriteAnimationSystem::pause_animation(sprite_id);
        return Value::nil();
    }});
    
    R.add("RESUMEANIMATION", NativeFn{"RESUMEANIMATION", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("RESUMEANIMATION: expected 1 arg");
        int sprite_id = args[0].as_int();
        SpriteAnimationSystem::resume_animation(sprite_id);
        return Value::nil();
    }});
    
    R.add("UPDATEANIMATIONS", NativeFn{"UPDATEANIMATIONS", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("UPDATEANIMATIONS: expected 1 arg");
        double delta_time = args[0].as_number();
        SpriteAnimationSystem::update_all_animations(delta_time);
        return Value::nil();
    }});
    
    R.add("DESTROYANIMATION", NativeFn{"DESTROYANIMATION", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DESTROYANIMATION: expected 1 arg");
        int animation_id = args[0].as_int();
        SpriteAnimationSystem::destroy_animation(animation_id);
        return Value::nil();
    }});
}

} // namespace bas
