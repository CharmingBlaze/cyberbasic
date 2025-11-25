#include "bas/module_base.hpp"
#include "bas/game_systems.hpp"
#include "bas/level_editor.hpp"
#include "bas/asset_pipeline.hpp"
#include "bas/sprite_animation.hpp"
#include "bas/runtime.hpp"

namespace bas {

/**
 * Game Systems Module
 * Provides game development systems: level editor, asset pipeline, sprite animation
 */
class GameModule : public Module {
public:
    const char* name() const override { return "game"; }
    
    void register_functions(FunctionRegistry& registry) override {
        register_game_systems_bindings(registry);
        register_level_editor_functions(registry);
        register_asset_pipeline_functions(registry);
        register_sprite_animation_functions(registry);
    }
    
    void initialize() override {}
    void shutdown() override {}
};

extern "C" Module* create_game_module() {
    return new GameModule();
}

} // namespace bas

