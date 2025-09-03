#pragma once

// Main Raylib Header for CyberBasic
// This file includes all modular Raylib modules
// Following best practices: clean, maintainable, scalable

// Core Raylib functionality
#include "raylib.h"

// Modular Raylib modules
#include "raylib/core_window.hpp"
#include "raylib/basic_graphics.hpp"
#include "raylib/colors.hpp"
#include "raylib/basic_drawing.hpp"
#include "raylib/input.hpp"
#include "raylib/audio.hpp"
#include "raylib/textures_images.hpp"
#include "raylib/fonts_text.hpp"
#include "raylib/3d_graphics.hpp"
#include "raylib/advanced_graphics.hpp"
#include "raylib/physics_animation.hpp"
#include "raylib/shaders.hpp"
#include "raylib/advanced_3d.hpp"
#include "raylib/post_processing.hpp"
#include "raylib/remaining_raylib.hpp"

// Custom CyberBasic extensions
#include "bas/custom_cyberbasic.hpp"

// Platform-specific features
#include "raylib/platform_specific.hpp"

namespace bas {
    // Main registration function that includes all modules
    void register_raylib_bindings(FunctionRegistry& R);
    
    // Individual module registration functions
    void register_core_window_bindings(FunctionRegistry& R);
    void register_basic_graphics_bindings(FunctionRegistry& R);
    void register_colors_bindings(FunctionRegistry& R);
    void register_basic_drawing_bindings(FunctionRegistry& R);
    void register_input_bindings(FunctionRegistry& R);
    void register_audio_bindings(FunctionRegistry& R);
    void register_textures_images_bindings(FunctionRegistry& R);
    void register_fonts_text_bindings(FunctionRegistry& R);
    void register_3d_graphics_bindings(FunctionRegistry& R);
    void register_advanced_graphics_bindings(FunctionRegistry& R);
    void register_physics_animation_bindings(FunctionRegistry& R);
    void register_shaders_bindings(FunctionRegistry& R);
    void register_advanced_3d_bindings(FunctionRegistry& R);
    void register_post_processing_bindings(FunctionRegistry& R);
    void register_remaining_raylib_bindings(FunctionRegistry& R);
    void register_platform_specific_bindings(FunctionRegistry& R);
    void register_custom_cyberbasic_bindings(FunctionRegistry& R);
    
    // Game Programming Systems
    void register_game_systems_bindings(FunctionRegistry& R);
    void register_networking_bindings(FunctionRegistry& R);
    void register_ai_bindings(FunctionRegistry& R);
    void register_navigation_bindings(FunctionRegistry& R);
    void register_physics_bindings(FunctionRegistry& R);
}
