#include "bas/builtins.hpp"
#include "bas/input.hpp"

#include <unordered_map>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <iostream>
#include <ostream>

#include "raylib.h"

using namespace bas;

void register_builtins_graphics(FunctionRegistry& R) {
  // --- Minimal graphics/window state and helpers ---
  struct Sprite {
    long long texId;
    float x;
    float y;
    float w;
    float h;
  };

  struct GfxState {
    bool windowInited = false;
    bool frameBegun = false;
    int width = 800;
    int height = 600;
    int targetFps = 60;
    std::unordered_map<long long, Texture2D> textures; // id -> texture
    std::unordered_map<long long, Sprite> sprites;
    long long nextTexId = 1;
    long long nextSpriteId = 1;
  };
  static GfxState gfx;

  auto ensureWindow = [&](){
    if (!gfx.windowInited) {
      InitWindow(gfx.width, gfx.height, "CyberBasic");
      SetTargetFPS(gfx.targetFps);
      gfx.windowInited = true;
    }
  };

  // CLOSEWINDOW(): close raylib window and unload textures; clears sprites
  R.add("CLOSEWINDOW", NativeFn{"CLOSEWINDOW", 0, [](const std::vector<Value>&){
    for (auto it = gfx.textures.begin(); it != gfx.textures.end(); ++it) {
      UnloadTexture(it->second);
    }
    gfx.textures.clear();
    gfx.sprites.clear();
    if (IsWindowReady()) { CloseWindow(); }
    gfx.windowInited = false; gfx.frameBegun = false;
    return Value::nil();
  }});

  // UNLOADIMAGE(texId)
  R.add("UNLOADIMAGE", NativeFn{"UNLOADIMAGE", 1, [](const std::vector<Value>& a){
    long long id = a[0].as_int();
    auto it = gfx.textures.find(id);
    if (it != gfx.textures.end()) {
      UnloadTexture(it->second);
      gfx.textures.erase(it);
    }
    return Value::nil();
  }});

  // DELETESPRITE(id)
  R.add("DELETESPRITE", NativeFn{"DELETESPRITE", 1, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) { gfx.sprites.erase(it); }
    return Value::nil();
  }});

  // SETSYNCRATE(fps)
  R.add("SETSYNCRATE", NativeFn{"SETSYNCRATE", 1, [](const std::vector<Value>& a){
    double fps = a[0].as_number();
    if(fps <= 0) fps = 60.0;
    static double* target_ms_ptr = nullptr;
    static double target_ms_val = 1000.0/60.0;
    if(!target_ms_ptr) target_ms_ptr = &target_ms_val;
    *target_ms_ptr = 1000.0 / fps;
    if (IsWindowReady()) SetTargetFPS((int)fps);
    return Value::nil();
  }});

  // SYNC(): sleep to maintain target frame time and end frame if begun
  R.add("SYNC", NativeFn{"SYNC", 0, [](const std::vector<Value>&){
    using clock = std::chrono::steady_clock;
    static clock::time_point last = clock::now();
    static double target_ms = 1000.0/60.0;
    auto now = clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if(elapsed_ms < (long long)target_ms){
      std::this_thread::sleep_for(std::chrono::milliseconds((long long)target_ms - elapsed_ms));
    }
    last = clock::now();
    if (IsWindowReady()) {
      if (gfx.frameBegun) { EndDrawing(); gfx.frameBegun = false; }
    }
    return Value::nil();
  }});

  // CLS(): clear console screen or clear current frame if window already exists
  R.add("CLS", NativeFn{"CLS", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      if (!gfx.frameBegun) { BeginDrawing(); gfx.frameBegun = true; }
      ClearBackground(BLACK);
    } else {
      // ANSI clear + home (console)
      std::cout << "\x1b[2J\x1b[H" << std::flush;
    }
    return Value::nil();
  }});

  // SETWINDOWTITLE(title)
  R.add("SETWINDOWTITLE", NativeFn{"SETWINDOWTITLE", 1, [](const std::vector<Value>& a){
    const std::string& title = a[0].as_string();
    if (IsWindowReady()) SetWindowTitle(title.c_str());
    return Value::nil();
  }});

  // TEXT(x,y,str): move cursor and print (console) or draw text (window)
  R.add("TEXT", NativeFn{"TEXT", 3, [](const std::vector<Value>& a){
    long long x = a[0].as_int();
    long long y = a[1].as_int();
    const std::string& s = a[2].as_string();
    if(x<1) { x=1; }
    if(y<1) { y=1; }
    if (IsWindowReady()) {
      DrawText(s.c_str(), (int)x, (int)y, 20, RAYWHITE);
    } else {
      std::cout << "\x1b[" << y << ";" << x << "H" << s << std::flush;
    }
    return Value::nil();
  }});

  // SETWINDOWSIZE(w,h)
  R.add("SETWINDOWSIZE", NativeFn{"SETWINDOWSIZE", 2, [](const std::vector<Value>& a){
    int w = static_cast<int>((int)a[0].as_int());
    int h = static_cast<int>((int)a[1].as_int());
    if (!IsWindowReady()) { gfx.width = w; gfx.height = h; }
    else SetWindowSize(w, h);
    return Value::nil();
  }});

  // --- Image/Texture and Sprite builtins ---
  R.add("LOADIMAGE", NativeFn{"LOADIMAGE", 1, [ensureWindow](const std::vector<Value>& a){
    ensureWindow();
    std::string path = a[0].as_string();
    Texture2D tex = LoadTexture(path.c_str());
    long long id = gfx.nextTexId++;
    gfx.textures[id] = tex;
    return Value::from_int(id);
  }});

  R.add("CREATESPRITE", NativeFn{"CREATESPRITE", 1, [](const std::vector<Value>& a){
    long long texId = a[0].as_int();
    auto it = gfx.textures.find(texId);
    if (it == gfx.textures.end()) return Value::from_int(0);
    const auto& t = it->second;
    long long sid = gfx.nextSpriteId++;
    gfx.sprites[sid] = Sprite{texId, 0.0f, 0.0f, (float)t.width, (float)t.height};
    return Value::from_int(sid);
  }});

  R.add("SETSPRITEPOSITION", NativeFn{"SETSPRITEPOSITION", 3, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    float x = (float)a[1].as_number();
    float y = (float)a[2].as_number();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) { it->second.x = x; it->second.y = y; }
    return Value::nil();
  }});

  R.add("SETSPRITESIZE", NativeFn{"SETSPRITESIZE", 3, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    float w = (float)a[1].as_number();
    float h = (float)a[2].as_number();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) { it->second.w = w; it->second.h = h; }
    return Value::nil();
  }});

  R.add("DRAWSPRITE", NativeFn{"DRAWSPRITE", 1, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) {
      auto s = it->second;
      auto tit = gfx.textures.find(s.texId);
      if (tit != gfx.textures.end()) {
        Texture2D& tex = tit->second;
        Rectangle src{0,0,(float)tex.width,(float)tex.height};
        Rectangle dst{s.x, s.y, s.w, s.h};
        Vector2 origin{0,0};
        if (!gfx.frameBegun && IsWindowReady()) { BeginDrawing(); gfx.frameBegun = true; }
        DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
      }
    }
    return Value::nil();
  }});

  // --- Input helpers backed by raylib or bas::input ---
  R.add("KEYDOWN", NativeFn{"KEYDOWN", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    if (IsWindowReady()) {
      return Value::from_bool(IsKeyDown(key));
    }
    return Value::from_bool(false);
  }});

  R.add("MOUSEWHEEL", NativeFn{"MOUSEWHEEL", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseWheelMove());
    }
    return Value::from_int(0);
  }});

  R.add("MOUSEX", NativeFn{"MOUSEX", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseX());
    }
    return Value::from_int(0);
  }});

  R.add("MOUSEY", NativeFn{"MOUSEY", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseY());
    }
    return Value::from_int(0);
  }});

  R.add("MOUSEBUTTON", NativeFn{"MOUSEBUTTON", 1, [](const std::vector<Value>& a){
    long long b = a[0].as_int();
    int rlbtn = 0;
    if (b == 1) rlbtn = MOUSE_BUTTON_LEFT;
    else if (b == 2) rlbtn = MOUSE_BUTTON_RIGHT;
    else if (b == 3) rlbtn = MOUSE_BUTTON_MIDDLE;
    else return Value::from_bool(false);
    if (IsWindowReady()) {
      return Value::from_bool(IsMouseButtonDown(rlbtn));
    }
    return Value::from_bool(false);
  }});

  R.add("KEYHIT", NativeFn{"KEYHIT", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    return Value::from_bool(bas::input::key_hit(key));
  }});

  R.add("KEYUP", NativeFn{"KEYUP", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    return Value::from_bool(bas::input::key_up(key));
  }});

  // Compatibility aliases
  R.add("ISKEYPRESSED", NativeFn{"ISKEYPRESSED", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    return Value::from_bool(bas::input::key_hit(key));
  }});
  R.add("ISKEYDOWN", NativeFn{"ISKEYDOWN", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    return Value::from_bool(bas::input::key_down(key));
  }});
  R.add("ISKEYRELEASED", NativeFn{"ISKEYRELEASED", 1, [](const std::vector<Value>& a){
    int key = static_cast<int>((int)a[0].as_int());
    return Value::from_bool(bas::input::key_up(key));
  }});

  R.add("ISMOUSEBUTTONDOWN", NativeFn{"ISMOUSEBUTTONDOWN", 1, [](const std::vector<Value>& a){
    long long b = a[0].as_int();
    int rlbtn = 0;
    if (b == 1) rlbtn = MOUSE_BUTTON_LEFT;
    else if (b == 2) rlbtn = MOUSE_BUTTON_RIGHT;
    else if (b == 3) rlbtn = MOUSE_BUTTON_MIDDLE;
    else return Value::from_bool(false);
    if (IsWindowReady()) return Value::from_bool(IsMouseButtonDown(rlbtn));
    return Value::from_bool(false);
  }});

  R.add("GETMOUSEX", NativeFn{"GETMOUSEX", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) return Value::from_int((long long)GetMouseX());
    return Value::from_int(0);
  }});
  R.add("GETMOUSEY", NativeFn{"GETMOUSEY", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) return Value::from_int((long long)GetMouseY());
    return Value::from_int(0);
  }});

  // Modifier helpers
  R.add("KEYSHIFT", NativeFn{"KEYSHIFT", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_shift());
  }});
  R.add("KEYCTRL", NativeFn{"KEYCTRL", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_ctrl());
  }});
  R.add("KEYALT", NativeFn{"KEYALT", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_alt());
  }});

  // Gamepad builtins
  R.add("GAMEPADCOUNT", NativeFn{"GAMEPADCOUNT", 0, [](const std::vector<Value>&){
    return Value::from_int((long long)bas::input::gamepad_count());
  }});
  R.add("GAMEPADNAME", NativeFn{"GAMEPADNAME", 1, [](const std::vector<Value>& a){
    int i = static_cast<int>((int)a[0].as_int());
    const char* n = bas::input::gamepad_name(i);
    return Value::from_string(n ? std::string(n) : std::string());
  }});
  R.add("GAMEPADDOWN", NativeFn{"GAMEPADDOWN", 2, [](const std::vector<Value>& a){
    int i = static_cast<int>((int)a[0].as_int()); int btn = static_cast<int>((int)a[1].as_int());
    return Value::from_bool(bas::input::gamepad_down(i, btn));
  }});
  R.add("GAMEPADHIT", NativeFn{"GAMEPADHIT", 2, [](const std::vector<Value>& a){
    int i = static_cast<int>((int)a[0].as_int()); int btn = static_cast<int>((int)a[1].as_int());
    return Value::from_bool(bas::input::gamepad_hit(i, btn));
  }});
  R.add("GAMEPADUP", NativeFn{"GAMEPADUP", 2, [](const std::vector<Value>& a){
    int i = static_cast<int>((int)a[0].as_int()); int btn = static_cast<int>((int)a[1].as_int());
    return Value::from_bool(bas::input::gamepad_up(i, btn));
  }});
  R.add("GAMEPADAXIS", NativeFn{"GAMEPADAXIS", 2, [](const std::vector<Value>& a){
    int i = static_cast<int>((int)a[0].as_int()); int axis = static_cast<int>((int)a[1].as_int());
    return Value::from_number((double)bas::input::gamepad_axis(i, axis));
  }});
}
