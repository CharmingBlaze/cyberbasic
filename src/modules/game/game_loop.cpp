#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/timer_system.hpp"
#include <raylib.h>
#include <string>

namespace bas {

// Game loop state
static bool g_game_running = false;
static int g_target_fps = 60;
static Color g_background_color = BLACK;

// Game.init(width, height, title, [targetFPS])
static Value game_init(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_bool(false);
    }
    
    int width = static_cast<int>(args[0].as_int());
    int height = static_cast<int>(args[1].as_int());
    std::string title = args[2].is_string() ? args[2].as_string() : "Game";
    int fps = args.size() > 3 ? static_cast<int>(args[3].as_int()) : 60;
    
    if (g_game_running) {
        CloseWindow();
    }
    
    InitWindow(width, height, title.c_str());
    SetTargetFPS(fps);
    
    g_target_fps = fps;
    g_game_running = true;
    
    return Value::from_bool(true);
}

// Game.close() - closes the game window
static Value game_close(const std::vector<Value>& args) {
    (void)args;
    if (g_game_running) {
        CloseWindow();
        g_game_running = false;
    }
    return Value::from_bool(true);
}

// Game.shouldClose() - returns true if window should close
static Value game_shouldClose(const std::vector<Value>& args) {
    (void)args;
    return Value::from_bool(WindowShouldClose());
}

// Game.setBackgroundColor(r, g, b, [a])
static Value game_setBackgroundColor(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::nil();
    }
    
    unsigned char r = static_cast<unsigned char>(args[0].as_int());
    unsigned char g = static_cast<unsigned char>(args[1].as_int());
    unsigned char b = static_cast<unsigned char>(args[2].as_int());
    unsigned char a = args.size() > 3 ? static_cast<unsigned char>(args[3].as_int()) : 255;
    
    g_background_color = {r, g, b, a};
    
    return Value::nil();
}

// Game.beginFrame() - begins a new frame (clears screen)
static Value game_beginFrame(const std::vector<Value>& args) {
    (void)args;
    BeginDrawing();
    ClearBackground(g_background_color);
    
    // Update delta time
    update_delta_time();
    
    return Value::nil();
}

// Game.endFrame() - ends the current frame
static Value game_endFrame(const std::vector<Value>& args) {
    (void)args;
    EndDrawing();
    return Value::nil();
}

// Game.getWidth() - returns window width
static Value game_getWidth(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetScreenWidth());
}

// Game.getHeight() - returns window height
static Value game_getHeight(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetScreenHeight());
}

// Game.setTargetFPS(fps)
static Value game_setTargetFPS(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    
    int fps = static_cast<int>(args[0].as_int());
    if (fps > 0) {
        SetTargetFPS(fps);
        g_target_fps = fps;
    }
    
    return Value::nil();
}

// Register game loop functions
void register_game_loop(FunctionRegistry& R) {
    R.add("GAME_INIT", NativeFn{"GAME_INIT", -1, game_init});
    R.add("GAME_CLOSE", NativeFn{"GAME_CLOSE", 0, game_close});
    R.add("GAME_SHOULDCLOSE", NativeFn{"GAME_SHOULDCLOSE", 0, game_shouldClose});
    R.add("GAME_SETBACKGROUNDCOLOR", NativeFn{"GAME_SETBACKGROUNDCOLOR", -1, game_setBackgroundColor});
    R.add("GAME_BEGINFRAME", NativeFn{"GAME_BEGINFRAME", 0, game_beginFrame});
    R.add("GAME_ENDFRAME", NativeFn{"GAME_ENDFRAME", 0, game_endFrame});
    R.add("GAME_GETWIDTH", NativeFn{"GAME_GETWIDTH", 0, game_getWidth});
    R.add("GAME_GETHEIGHT", NativeFn{"GAME_GETHEIGHT", 0, game_getHeight});
    R.add("GAME_SETTARGETFPS", NativeFn{"GAME_SETTARGETFPS", 1, game_setTargetFPS});
}

} // namespace bas

