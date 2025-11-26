cmake_minimum_required(VERSION 3.25)
project(basic_raylib LANGUAGES CXX VERSION 1.0.0)

# Prevent MSVC usage
include(cmake/PreventMSVC.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Static linking option for MinGW - default to ON for distribution builds
option(BASIC_STATIC_LINK "Static link on MinGW" ON)

# Module build options (all ON by default, can be disabled for minimal builds)
option(BUILD_GRAPHICS_MODULE "Build graphics module" ON)
option(BUILD_AUDIO_MODULE "Build audio module" ON)
option(BUILD_INPUT_MODULE "Build input module" ON)
option(BUILD_NETWORKING_MODULE "Build networking module" ON)
option(BUILD_PHYSICS_MODULE "Build physics module" ON)
option(BUILD_GUI_MODULE "Build GUI module" ON)
option(BUILD_3D_MODULE "Build 3D graphics module" ON)
option(BUILD_AI_MODULE "Build AI module" ON)
option(BUILD_GAME_MODULE "Build game systems module" ON)
option(BUILD_RAYMATH_MODULE "Build raymath module" ON)

# Use raylib from git submodule (included in repository)
# Set comprehensive CMake policies for future compatibility and to eliminate warnings

# Set all relevant CMake policies to NEW for consistent behavior
if(POLICY CMP0001)
    cmake_policy(SET CMP0001 NEW)  # Variable expansion in if() statements
endif()
if(POLICY CMP0066)
    cmake_policy(SET CMP0066 NEW)  # Honor per-config flags in try_compile()
endif()
if(POLICY CMP0067)
    cmake_policy(SET CMP0067 NEW)  # Honor language standard in try_compile()
endif()
if(POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)  # set(CACHE) does not remove normal variable
endif()
if(POLICY CMP0156)
    cmake_policy(SET CMP0156 NEW)  # De-duplicate libraries on link lines
endif()
if(POLICY CMP0181)
    cmake_policy(SET CMP0181 NEW)  # Link command-line fragment variables are parsed
endif()
if(POLICY CMP0200)
    cmake_policy(SET CMP0200 NEW)  # Location and configuration selection for imported targets
endif()

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Configure raylib build options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build raylib as static library" FORCE)
set(RAYLIB_BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
set(RAYLIB_BUILD_TOOLS OFF CACHE BOOL "Build raylib tools" FORCE)

# Configure raylib audio modules with full format support
set(SUPPORT_MODULE_RAUDIO ON CACHE BOOL "Build raylib audio module" FORCE)
set(SUPPORT_FILEFORMAT_MOD ON CACHE BOOL "Enable MOD audio format support" FORCE)
set(SUPPORT_FILEFORMAT_XM ON CACHE BOOL "Enable XM audio format support" FORCE)
set(SUPPORT_FILEFORMAT_S3M ON CACHE BOOL "Enable S3M audio format support" FORCE)

# Add raylib submodule
add_subdirectory(raylib)

# Generate raylib bindings
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Ensure bindings directory exists
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src/bindings)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/bindings/rt_raylib.gen.cpp
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/bindings
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/gen_raylib_bindings.py
    DEPENDS ${CMAKE_SOURCE_DIR}/tools/gen_raylib_bindings.py ${CMAKE_SOURCE_DIR}/specs/raylib_api.yaml
    COMMENT "Generating raylib bindings"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Create a custom target for just the bindings
add_custom_target(generate_bindings ALL
    DEPENDS ${CMAKE_BINARY_DIR}/src/bindings/rt_raylib.gen.cpp
    COMMENT "Generating raylib bindings target"
)

# Main executable
add_executable(cyberbasic
  src/main.cpp
  # Core interpreter files
  src/core/lexer.cpp
  src/core/parser.cpp
  src/core/interpreter.cpp
  src/core/runtime.cpp
  src/core/namespace_registry.cpp
  src/core/type_system.cpp
  # Builtin functions
  src/builtins_core.cpp
  src/core/builtins_objects.cpp
  src/core/builtins_json.cpp
  src/core/builtins_file.cpp
  src/builtins_console.cpp
  src/builtins_graphics.cpp
  src/builtins_audio.cpp
  # Core modules
  src/modules/core/enums_and_dicts.cpp
  src/modules/core/dot_notation_enhancements.cpp
  src/modules/core/advanced_features.cpp
  # Module files
  src/modules/input/input.cpp
  src/modules/input/input_module.cpp
  src/modules/ai/navigation.cpp
  src/modules/physics/physics.cpp
  src/modules/physics/physics_module.cpp
  src/modules/ai/ai.cpp
  src/modules/graphics/graphics.cpp
  src/modules/graphics/graphics_module.cpp
  src/modules/networking/networking.cpp
  src/modules/networking/networking_module.cpp
  src/modules/audio/audio.cpp
  src/modules/audio/audio_module.cpp
  src/modules/3d/camera3d.cpp
  src/modules/3d/lighting3d.cpp
  src/modules/3d/models3d.cpp
  src/modules/3d/3d_module.cpp
  src/modules/ai/ai_module.cpp
  src/modules/game/game_module.cpp
  src/modules/game/sprite_system.cpp
  src/modules/game/timer_system.cpp
  src/modules/game/input_events.cpp
  src/modules/game/animation_system.cpp
  src/modules/game/scene_entity_system.cpp
  src/modules/game/ecs_system.cpp
  src/modules/game/camera_system.cpp
  src/modules/game/collision_system.cpp
  src/modules/game/game_loop.cpp
  src/modules/game/game_helpers.cpp
  src/custom_cyberbasic.cpp
  src/modules/game/game_systems.cpp
  # Generated bindings
  ${CMAKE_BINARY_DIR}/src/bindings/rt_raylib.gen.cpp
  # Additional modules
  src/modules/game/level_editor.cpp
  src/modules/game/level_editor_bindings.cpp
  src/modules/game/asset_pipeline.cpp
  src/modules/game/asset_pipeline_bindings.cpp
  src/modules/game/sprite_animation.cpp
  src/modules/game/sprite_animation_bindings.cpp
  src/modules/game/tween_system.cpp
  src/modules/game/camera_shake.cpp
  src/modules/game/localization.cpp
  src/modules/game/state_machine.cpp
  src/modules/game/modern_state_system.cpp
  src/modules/game/game_commands.cpp
  src/modules/game/modern_features.cpp
  src/modules/game/advanced_features.cpp
  src/modules/game/post_processing.cpp
  src/modules/game/streaming.cpp
  src/modules/game/enhanced_events.cpp
  src/modules/game/screen_transitions.cpp
  src/modules/gui/gui_control.cpp
  src/modules/gui/gui_controls.cpp
  src/modules/gui/gui_manager.cpp
  src/modules/gui/gui_style.cpp
  src/modules/gui/gui_layout.cpp
  src/modules/gui/gui_bindings.cpp
  src/modules/gui/gui_module.cpp
  src/modules/raylib/raymath_bindings.cpp
  src/modules/raylib/raymath_module.cpp
  src/modules/raylib/raygui_impl.cpp
  src/modules/raylib/raygui_bindings.cpp
)

# Ensure cyberbasic depends on bindings generation
add_dependencies(cyberbasic generate_bindings)

# Include directories
target_include_directories(cyberbasic PRIVATE include)
target_include_directories(cyberbasic PRIVATE raygui/src)

# Compiler-specific options - enable comprehensive warnings to catch issues
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cyberbasic PRIVATE 
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter  # Only disable truly unavoidable warnings
        -Wno-missing-field-initializers  # For raylib compatibility
    )
endif()

# Ensure bindings are generated before building
add_dependencies(cyberbasic generate_bindings)

# Link libraries
target_link_libraries(cyberbasic PRIVATE raylib)

# Platform-specific linking
if(WIN32)
    # Don't link winmm explicitly - raylib handles audio and includes winmm internally
    # target_link_libraries(cyberbasic PRIVATE winmm)
    if(MINGW AND BASIC_STATIC_LINK)
        # Enhanced static linking flags for zero DLL dependencies
        target_link_options(cyberbasic PRIVATE 
            -static 
            -static-libgcc 
            -static-libstdc++
            -Wl,--strip-all
            -Wl,--gc-sections
        )
        # Additional static linking libraries for Windows
        target_link_libraries(cyberbasic PRIVATE 
            -static
            gdi32 
            ole32 
            oleaut32 
            imm32 
            winmm 
            version
            setupapi
            advapi32
            shell32
            user32
            kernel32
        )
        
        # Set static runtime library
        set_property(TARGET cyberbasic PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
elseif(APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(OPENGL_FRAMEWORK OpenGL)
    target_link_libraries(cyberbasic PRIVATE ${COCOA_FRAMEWORK} ${IOKIT_FRAMEWORK} ${OPENGL_FRAMEWORK})
    
    if(BASIC_STATIC_LINK)
        # Static linking options for macOS
        target_link_options(cyberbasic PRIVATE 
            -static-libgcc 
            -static-libstdc++
        )
    endif()
elseif(UNIX AND NOT APPLE)
    # Linux dependencies
    target_link_libraries(cyberbasic PRIVATE pthread dl m)
    
    if(BASIC_STATIC_LINK)
        # Static linking options for Linux
        target_link_options(cyberbasic PRIVATE 
            -static-libgcc 
            -static-libstdc++
            -Wl,--gc-sections
            -Wl,--strip-all
        )
    endif()
endif()

# Install rule
install(TARGETS cyberbasic RUNTIME DESTINATION bin)

# Set output directory for executables
set_target_properties(cyberbasic PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Include module configuration
include(cmake/Modules.cmake)