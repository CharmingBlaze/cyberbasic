#include "bas/input.hpp"

// Windows API not needed for core functionality

#include "raylib.h"
#include <cmath>

namespace bas { namespace input {

static bool inited = false;

void init(){ if(inited) return; inited = true; }

// Helper: is window ready for raylib functions that require it
static inline bool win_ready(){ return IsWindowReady(); }

bool key_down(int key){
  if (win_ready()) return IsKeyDown(key);
  return false;
}

bool key_hit(int key){
  if (win_ready()) return IsKeyPressed(key);
  return false;
}

bool key_up(int key){
  if (win_ready()) return IsKeyReleased(key);
  return false;
}

bool key_shift(){
  if (win_ready()) return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
  return false;
}

bool key_ctrl(){
  if (win_ready()) return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
  return false;
}

bool key_alt(){
  if (win_ready()) return IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
  return false;
}

int mouse_wheel(){
  // raylib accumulates per-frame; round to nearest int for BASIC usage
  float dw = GetMouseWheelMove();
  return (int)std::lround(dw);
}

int gamepad_count(){
  int n = 0;
  for (int i = 0; i < 8; ++i) if (IsGamepadAvailable(i)) ++n;
  return n;
}

const char* gamepad_name(int i){
  if (!IsGamepadAvailable(i)) return nullptr;
  return GetGamepadName(i);
}

bool gamepad_down(int i, int btn){
  if (!IsGamepadAvailable(i)) return false;
  return IsGamepadButtonDown(i, btn);
}

bool gamepad_hit(int i, int btn){
  if (!IsGamepadAvailable(i)) return false;
  return IsGamepadButtonPressed(i, btn);
}

bool gamepad_up(int i, int btn){
  if (!IsGamepadAvailable(i)) return false;
  return IsGamepadButtonReleased(i, btn);
}

float gamepad_axis(int i, int axis){
  if (!IsGamepadAvailable(i)) return 0.0f;
  return GetGamepadAxisMovement(i, axis);
}

}} // namespace bas::input
