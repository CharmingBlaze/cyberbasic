#pragma once
#include <unordered_map>

namespace bas {
namespace input {

// Initializes any platform-specific state if needed (no-op safe to call multiple times)
void init();

// Keyboard
bool key_down(int key);
bool key_hit(int key);    // edge: went down since last call
bool key_up(int key);     // edge: went up since last call

bool key_shift();
bool key_ctrl();
bool key_alt();

// Mouse
int mouse_wheel(); // integerized delta since last poll (raylib based if window exists)

// Gamepad (raylib-backed; returns safe defaults if not available)
int gamepad_count();              // number of available gamepads (0..)
const char* gamepad_name(int i);  // nullptr if unavailable
bool gamepad_down(int i, int btn);
bool gamepad_hit(int i, int btn);
bool gamepad_up(int i, int btn);
float gamepad_axis(int i, int axis);

} // namespace input
} // namespace bas
