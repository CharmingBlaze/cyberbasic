#include "raylib.hpp"
#include "bas/runtime.hpp"

namespace bas {

void register_raylib_bindings(FunctionRegistry& R) {
    // Register all modular Raylib systems
    register_core_window_bindings(R);
    register_basic_graphics_bindings(R);
    register_colors_bindings(R);
    register_basic_drawing_bindings(R);
    register_input_bindings(R);
    register_audio_bindings(R);
    register_textures_images_bindings(R);
    register_fonts_text_bindings(R);
    register_3d_graphics_bindings(R);
    register_advanced_graphics_bindings(R);
    register_physics_animation_bindings(R);
    register_shaders_bindings(R);
    register_advanced_3d_bindings(R);
    register_post_processing_bindings(R);
    register_remaining_raylib_bindings(R);
    register_platform_specific_bindings(R);
    register_custom_cyberbasic_bindings(R);
}

} // namespace bas
