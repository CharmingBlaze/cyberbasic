cmake_minimum_required(VERSION 3.20)
project(basic_raylib LANGUAGES CXX)

# Prevent MSVC usage
include(cmake/PreventMSVC.cmake)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Static linking option for MinGW
option(BASIC_STATIC_LINK "Static link on MinGW" OFF)

# Find or fetch raylib
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Generate raylib bindings
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/src/rt_raylib.gen.cpp
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/gen_raylib_bindings.py
    DEPENDS ${CMAKE_SOURCE_DIR}/tools/gen_raylib_bindings.py ${CMAKE_SOURCE_DIR}/specs/raylib_api.yaml
    COMMENT "Generating raylib bindings"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Main executable
add_executable(cyberbasic
  src/main.cpp
  src/lexer.cpp
  src/parser.cpp
  src/interpreter.cpp
  src/builtins_core.cpp
  src/builtins_console.cpp
  src/builtins_graphics.cpp
  src/builtins_audio.cpp
  src/runtime.cpp
  src/input.cpp
  src/navigation.cpp
  src/physics.cpp
  src/ai.cpp
  src/graphics.cpp
  src/networking.cpp
  src/audio.cpp
  src/camera3d.cpp
  src/lighting3d.cpp
  src/models3d.cpp
  src/custom_cyberbasic.cpp
  src/game_systems.cpp
  src/rt_raylib.gen.cpp
  src/level_editor.cpp
  src/level_editor_bindings.cpp
  src/asset_pipeline.cpp
  src/asset_pipeline_bindings.cpp
  src/sprite_animation.cpp
  src/sprite_animation_bindings.cpp
  src/gui_control.cpp
  src/gui_controls.cpp
  src/gui_manager.cpp
  src/gui_style.cpp
  src/gui_layout.cpp
  src/gui_bindings.cpp
)

# Include directories
target_include_directories(cyberbasic PRIVATE include)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cyberbasic PRIVATE -Wall -Wextra -Werror -Wpedantic)
endif()

target_include_directories(cyberbasic PRIVATE include)
target_link_libraries(cyberbasic PRIVATE raylib)

# Platform-specific linking
if(WIN32)
    target_link_libraries(cyberbasic PRIVATE winmm)
    if(MINGW AND BASIC_STATIC_LINK)
        target_link_options(cyberbasic PRIVATE -static -static-libgcc -static-libstdc++)
    endif()
endif()

# Install rule
install(TARGETS cyberbasic RUNTIME DESTINATION bin)