#include "bas/builtins.hpp"
#include "bas/input.hpp"
#include <iostream>
#include <random>
#include <chrono>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <locale>
#include <cmath>
#include <cctype>
#include <unordered_map>
#include <memory>

// Windows API not needed for core functionality

#include "raylib.h"

using namespace bas;

static std::string to_string_value(const Value& v){
  if(auto s = std::get_if<std::string>(&v.v)) return *s;
  if(auto b = std::get_if<bool>(&v.v)) return *b?"TRUE":"FALSE";
  if(auto d = std::get_if<double>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*d; return os.str(); }
  if(auto i = std::get_if<long long>(&v.v)) { std::ostringstream os; os.imbue(std::locale::classic()); os<<*i; return os.str(); }
  return std::string{};
}

// NOTE: Legacy monolithic registrar disabled to prevent duplicate registrations.
// The active registrar is implemented in src/builtins_core.cpp as bas::register_builtins.
// Keeping this code under a different name preserves reference implementations without
// conflicting symbols or double-registration at runtime.
void bas::register_builtins_legacy_disabled(FunctionRegistry& R){
  // Initialize input subsystem (safe to call multiple times)
  bas::input::init();
  R.add("PRINT", NativeFn{"PRINT", 1, [](const std::vector<Value>& a){
    std::cout << to_string_value(a[0]) << std::endl; return Value::nil();
  }});

  R.add("INPUT", NativeFn{"INPUT", -1, [](const std::vector<Value>& a){
    if(!a.empty()) std::cout << to_string_value(a[0]);
    std::string line;
    std::getline(std::cin, line);
    return Value::from_string(line);
  }});

  R.add("STR", NativeFn{"STR", 1, [](const std::vector<Value>& a){
    return Value::from_string(to_string_value(a[0]));
  }});

  R.add("VAL", NativeFn{"VAL", 1, [](const std::vector<Value>& a){
    if(std::holds_alternative<double>(a[0].v)) return a[0];
    if(std::holds_alternative<long long>(a[0].v)) return Value::from_number(static_cast<double>(std::get<long long>(a[0].v)));
    if(std::holds_alternative<std::string>(a[0].v)) return Value::from_number(std::stod(std::get<std::string>(a[0].v)));
    if(std::holds_alternative<bool>(a[0].v)) return Value::from_number(std::get<bool>(a[0].v)?1.0:0.0);
    return Value::from_number(0.0);
  }});

  R.add("LEN", NativeFn{"LEN", 1, [](const std::vector<Value>& a){
    // LEN on string or array
    if (std::holds_alternative<std::string>(a[0].v)) {
      return Value::from_int(static_cast<long long>(a[0].as_string().size()));
    }
    if (a[0].is_array()) {
      const auto& arr = a[0].as_array();
      return Value::from_int(static_cast<long long>(arr.size()));
    }
    return Value::from_int(0);
  }});

  R.add("SUBSTR", NativeFn{"SUBSTR", 3, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    long long start = a[1].as_int();
    long long len = a[2].as_int();
    if(start < 0) start = 0;
    if(start > static_cast<long long>(s.size())) start = static_cast<long long>(s.size());
    long long end = start + len;
    if(end > static_cast<long long>(s.size())) end = static_cast<long long>(s.size());
    return Value::from_string(s.substr(static_cast<size_t>(start), static_cast<size_t>(end-start)));
  }});

  R.add("RANDOM", NativeFn{"RANDOM", -1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng{std::random_device{}()};
    if(a.empty()){
      std::uniform_real_distribution<double> dist(0.0,1.0); return Value::from_number(dist(eng));
    }
    long long n = a[0].as_int(); if(n<=0) return Value::from_int(0);
    std::uniform_int_distribution<long long> dist(0,n-1); return Value::from_int(dist(eng));
  }});

  R.add("SEED", NativeFn{"SEED", 1, [](const std::vector<Value>& a){
    static std::mt19937_64 eng; eng.seed(static_cast<uint64_t>(a[0].as_int())); return Value::nil();
  }});

  R.add("TIME", NativeFn{"TIME", 0, [](const std::vector<Value>&){
    using clock = std::chrono::system_clock;
    auto now = clock::now().time_since_epoch();
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    return Value::from_int(static_cast<long long>(secs));
  }});

  R.add("ASSERT", NativeFn{"ASSERT", 1, [](const std::vector<Value>& a){
    if(!a[0].as_bool()) throw std::runtime_error("ASSERT failed");
    return Value::nil();
  }});

  // Array functions
  R.add("ARRAY", NativeFn{"ARRAY", 1, [](const std::vector<Value>& a){
    long long nll = a[0].as_int();
    if(nll < 0) nll = 0;
    size_t n = static_cast<size_t>(nll);
    bas::Value::Array arr; arr.resize(n, Value::nil());
    return Value::from_array(std::move(arr));
  }});

  R.add("PUSH", NativeFn{"PUSH", 2, [](const std::vector<Value>& a){
    // Returns a NEW array with value appended (non-mutating)
    if(!a[0].is_array()) throw std::runtime_error("PUSH: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    copy.push_back(a[1]);
    return Value::from_array(std::move(copy));
  }});

  // APPEND: alias of PUSH
  R.add("APPEND", NativeFn{"APPEND", 2, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("APPEND: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    copy.push_back(a[1]);
    return Value::from_array(std::move(copy));
  }});

  // POP: returns a NEW array without the last element
  R.add("POP", NativeFn{"POP", 1, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("POP: argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    if(!copy.empty()) copy.pop_back();
    return Value::from_array(std::move(copy));
  }});

  // INSERT(arr, idx, val): returns NEW array with val inserted at idx (clamped to [0..len])
  R.add("INSERT", NativeFn{"INSERT", 3, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("INSERT: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    long long idx = a[1].as_int();
    if(idx < 0) idx = 0;
    if((size_t)idx > copy.size()) idx = (long long)copy.size();
    copy.insert(copy.begin() + (size_t)idx, a[2]);
    return Value::from_array(std::move(copy));
  }});

  // REMOVE(arr, idx): returns NEW array with element at idx removed (no-op if out of range)
  R.add("REMOVE", NativeFn{"REMOVE", 2, [](const std::vector<Value>& a){
    if(!a[0].is_array()) throw std::runtime_error("REMOVE: first argument must be an array");
    bas::Value::Array copy = a[0].as_array();
    long long idx = a[1].as_int();
    if(idx >= 0 && (size_t)idx < copy.size()) copy.erase(copy.begin() + (size_t)idx);
    return Value::from_array(std::move(copy));
  }});

  // File I/O functions
  R.add("FILEEXISTS", NativeFn{"FILEEXISTS", 1, [](const std::vector<Value>& a){
    std::string filename = a[0].as_string();
    std::ifstream file(filename);
    return Value::from_bool(file.good());
  }});

  R.add("FILESIZE", NativeFn{"FILESIZE", 1, [](const std::vector<Value>& a){
    std::string filename = a[0].as_string();
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file.is_open()) return Value::from_number(0.0);
    return Value::from_number(static_cast<double>(file.tellg()));
  }});

  // String manipulation
  R.add("LEFT", NativeFn{"LEFT", 2, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int count = static_cast<int>(a[1].as_number());
    if(count <= 0) return Value::from_string("");
    if(count >= static_cast<int>(str.length())) return Value::from_string(str);
    return Value::from_string(str.substr(0, count));
  }});

  R.add("RIGHT", NativeFn{"RIGHT", 2, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int count = static_cast<int>(a[1].as_number());
    if(count <= 0) return Value::from_string("");
    if(count >= static_cast<int>(str.length())) return Value::from_string(str);
    return Value::from_string(str.substr(str.length() - count));
  }});

  R.add("MID", NativeFn{"MID", 3, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    int start = static_cast<int>(a[1].as_number());
    int count = static_cast<int>(a[2].as_number());
    if(start < 0 || count <= 0) return Value::from_string("");
    if(start >= static_cast<int>(str.length())) return Value::from_string("");
    size_t startPos = static_cast<size_t>(start);
    size_t endPos = std::min(startPos + static_cast<size_t>(count), str.length());
    return Value::from_string(str.substr(startPos, endPos - startPos));
  }});

  R.add("UCASE", NativeFn{"UCASE", 1, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return Value::from_string(str);
  }});

  R.add("LCASE", NativeFn{"LCASE", 1, [](const std::vector<Value>& a){
    std::string str = a[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return Value::from_string(str);
  }});

  R.add("LTRIM", NativeFn{"LTRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    size_t i = 0; while(i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    return Value::from_string(s.substr(i));
  }});

  R.add("RTRIM", NativeFn{"RTRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    if(s.empty()) return Value::from_string("");
    size_t j = s.size(); while(j > 0 && std::isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return Value::from_string(s.substr(0, j));
  }});

  R.add("TRIM", NativeFn{"TRIM", 1, [](const std::vector<Value>& a){
    const std::string& s = a[0].as_string();
    size_t i = 0; while(i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    size_t j = s.size(); while(j > i && std::isspace(static_cast<unsigned char>(s[j-1]))) --j;
    return Value::from_string(s.substr(i, j - i));
  }});

  // INSTR(s, sub [, start]) -> 1-based index, 0 if not found
  R.add("INSTR", NativeFn{"INSTR", -1, [](const std::vector<Value>& a){
    if(a.size() < 2) throw std::runtime_error("INSTR: need at least 2 args");
    const std::string& s = a[0].as_string();
    const std::string& sub = a[1].as_string();
    long long start = 1;
    if(a.size() >= 3) start = a[2].as_int();
    if(start < 1) start = 1;
    size_t pos = s.find(sub, static_cast<size_t>(start - 1));
    if(pos == std::string::npos) return Value::from_int(0);
    return Value::from_int(static_cast<long long>(pos + 1));
  }});

  // File helpers: READALL(filename) -> string, WRITEALL(filename, data) -> bool
  R.add("READALL", NativeFn{"READALL", 1, [](const std::vector<Value>& a){
    std::ifstream f(a[0].as_string(), std::ios::binary);
    if(!f.is_open()) return Value::from_string("");
    std::ostringstream ss; ss << f.rdbuf();
    return Value::from_string(ss.str());
  }});

  R.add("WRITEALL", NativeFn{"WRITEALL", 2, [](const std::vector<Value>& a){
    std::ofstream f(a[0].as_string(), std::ios::binary);
    if(!f.is_open()) return Value::from_bool(false);
    const std::string& data = a[1].as_string();
    f.write(data.data(), static_cast<std::streamsize>(data.size()));
    return Value::from_bool(static_cast<bool>(f));
  }});

  // Mathematical functions
  R.add("ABS", NativeFn{"ABS", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::abs(num));
  }});

  R.add("SIN", NativeFn{"SIN", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::sin(angle));
  }});

  R.add("COS", NativeFn{"COS", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::cos(angle));
  }});

  R.add("TAN", NativeFn{"TAN", 1, [](const std::vector<Value>& a){
    double angle = a[0].as_number();
    return Value::from_number(std::tan(angle));
  }});

  R.add("ATAN", NativeFn{"ATAN", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::atan(num));
  }});

  R.add("SQRT", NativeFn{"SQRT", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if(num < 0) throw std::runtime_error("SQRT: cannot take square root of negative number");
    return Value::from_number(std::sqrt(num));
  }});

  R.add("POW", NativeFn{"POW", 2, [](const std::vector<Value>& a){
    double base = a[0].as_number();
    double exponent = a[1].as_number();
    return Value::from_number(std::pow(base, exponent));
  }});

  R.add("LOG", NativeFn{"LOG", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    if(num <= 0) throw std::runtime_error("LOG: cannot take log of non-positive number");
    return Value::from_number(std::log(num));
  }});

  R.add("EXP", NativeFn{"EXP", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_number(std::exp(num));
  }});

  // Type conversion
  R.add("INT", NativeFn{"INT", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(std::floor(num)));
  }});

  R.add("FIX", NativeFn{"FIX", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(num));
  }});

  R.add("ROUND", NativeFn{"ROUND", 1, [](const std::vector<Value>& a){
    double num = a[0].as_number();
    return Value::from_int(static_cast<long long>(std::round(num)));
  }});

  // System functions
  R.add("SLEEP", NativeFn{"SLEEP", 1, [](const std::vector<Value>& a){
    double seconds = a[0].as_number();
    if(seconds > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(seconds * 1000)));
    }
    return Value::nil();
  }});

  R.add("BEEP", NativeFn{"BEEP", 0, [](const std::vector<Value>&){
    // Simple beep
    std::cout << "\a" << std::flush;
    return Value::nil();
  }});

  // --- Minimal graphics/timing with automatic raylib window ---
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

  auto ensureWindow = [&](){
    if (!gfx.windowInited) {
      InitWindow(gfx.width, gfx.height, "CyberBasic");
      SetTargetFPS(gfx.targetFps);
      gfx.windowInited = true;
    }
  };
  // SETSYNCRATE(fps)
  R.add("SETSYNCRATE", NativeFn{"SETSYNCRATE", 1, [](const std::vector<Value>& a){
    double fps = a[0].as_number();
    if(fps <= 0) fps = 60.0;
    static double* target_ms_ptr = nullptr;
    static double target_ms_val = 1000.0/60.0;
    if(!target_ms_ptr) target_ms_ptr = &target_ms_val;
    *target_ms_ptr = 1000.0 / fps;
    // If raylib window is active, also set target FPS
    if (IsWindowReady()) SetTargetFPS((int)fps);
    return Value::nil();
  }});

  // SYNC(): sleep to maintain target frame time
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
    // If a raylib frame was begun, end it here
    if (IsWindowReady()) {
      if (gfx.frameBegun) { EndDrawing(); gfx.frameBegun = false; }
      // Window stays open; ESC close handled by program logic if desired
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

  // SETWINDOWTITLE(title): set window title
  R.add("SETWINDOWTITLE", NativeFn{"SETWINDOWTITLE", 1, [](const std::vector<Value>& a){
    const std::string& title = a[0].as_string();
    if (IsWindowReady()) SetWindowTitle(title.c_str());
    return Value::nil();
  }});

  // SETCURSORVISIBLE(flag)
  R.add("SETCURSORVISIBLE", NativeFn{"SETCURSORVISIBLE", 1, [](const std::vector<Value>& a){
    bool show = a[0].as_bool();
    if(show) std::cout << "\x1b[?25h"; else std::cout << "\x1b[?25l";
    return Value::nil();
  }});

  // INK(r,g,b): set ANSI color (best-effort). Values 0-255, mapped to 38;2;r;g;bm
  R.add("INK", NativeFn{"INK", 3, [](const std::vector<Value>& a){
    int r = (int)a[0].as_int(); int g=(int)a[1].as_int(); int b=(int)a[2].as_int();
    if(r<0) { r=0; }
    if(r>255) { r=255; }
    if(g<0) { g=0; }
    if(g>255) { g=255; }
    if(b<0) { b=0; }
    if(b>255) { b=255; }
    std::cout << "\x1b[38;2;" << r << ";" << g << ";" << b << "m";
    return Value::nil();
  }});

  // TEXT(x,y,str): move cursor and print
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

  // KEYDOWN(key): general key state (raylib handled)
  R.add("KEYDOWN", NativeFn{"KEYDOWN", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    if (IsWindowReady()) {
      return Value::from_bool(IsKeyDown(key));
    }
    return Value::from_bool(false);
  }});

  // MOUSEWHEEL(): returns wheel delta since last poll
  R.add("MOUSEWHEEL", NativeFn{"MOUSEWHEEL", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseWheelMove());
    }
    return Value::from_int(0);
  }});

  // MOUSEX(): screen X position of the cursor
  R.add("MOUSEX", NativeFn{"MOUSEX", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseX());
    }
    return Value::from_int(0);
  }});

  // MOUSEY(): screen Y position of the cursor
  R.add("MOUSEY", NativeFn{"MOUSEY", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) {
      return Value::from_int((long long)GetMouseY());
    }
    return Value::from_int(0);
  }});

  // MOUSEBUTTON(btn): 1=Left, 2=Right, 3=Middle. Returns TRUE if pressed.
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

  // KEYHIT(key): edge-triggered press
  R.add("KEYHIT", NativeFn{"KEYHIT", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    return Value::from_bool(bas::input::key_hit(key));
  }});

  // KEYUP(key): edge-triggered release
  R.add("KEYUP", NativeFn{"KEYUP", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    return Value::from_bool(bas::input::key_up(key));
  }});

  // --- Compatibility aliases used in examples ---
  // ISKEYPRESSED(key) -> alias of KEYHIT
  R.add("ISKEYPRESSED", NativeFn{"ISKEYPRESSED", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    return Value::from_bool(bas::input::key_hit(key));
  }});

  // ISKEYDOWN(key) -> alias of KEYDOWN
  R.add("ISKEYDOWN", NativeFn{"ISKEYDOWN", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    return Value::from_bool(bas::input::key_down(key));
  }});

  // ISKEYRELEASED(key) -> alias of KEYUP
  R.add("ISKEYRELEASED", NativeFn{"ISKEYRELEASED", 1, [](const std::vector<Value>& a){
    int key = (int)a[0].as_int();
    return Value::from_bool(bas::input::key_up(key));
  }});

  // ISMOUSEBUTTONDOWN(btn): 1=Left, 2=Right, 3=Middle
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

  // GETMOUSEX/GETMOUSEY -> aliases of MOUSEX/MOUSEY
  R.add("GETMOUSEX", NativeFn{"GETMOUSEX", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) return Value::from_int((long long)GetMouseX());
    return Value::from_int(0);
  }});
  R.add("GETMOUSEY", NativeFn{"GETMOUSEY", 0, [](const std::vector<Value>&){
    if (IsWindowReady()) return Value::from_int((long long)GetMouseY());
    return Value::from_int(0);
  }});

  // Modifier helpers: KEYSHIFT(), KEYCTRL(), KEYALT()
  R.add("KEYSHIFT", NativeFn{"KEYSHIFT", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_shift());
  }});

  R.add("KEYCTRL", NativeFn{"KEYCTRL", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_ctrl());
  }});

  R.add("KEYALT", NativeFn{"KEYALT", 0, [](const std::vector<Value>&){
    return Value::from_bool(bas::input::key_alt());
  }});

  // --- Gamepad builtins (raylib-backed) ---
  R.add("GAMEPADCOUNT", NativeFn{"GAMEPADCOUNT", 0, [](const std::vector<Value>&){
    return Value::from_int((long long)bas::input::gamepad_count());
  }});
  R.add("GAMEPADNAME", NativeFn{"GAMEPADNAME", 1, [](const std::vector<Value>& a){
    int i = (int)a[0].as_int();
    const char* n = bas::input::gamepad_name(i);
    return Value::from_string(n ? std::string(n) : std::string());
  }});
  R.add("GAMEPADDOWN", NativeFn{"GAMEPADDOWN", 2, [](const std::vector<Value>& a){
    int i = (int)a[0].as_int(); int btn = (int)a[1].as_int();
    return Value::from_bool(bas::input::gamepad_down(i, btn));
  }});
  R.add("GAMEPADHIT", NativeFn{"GAMEPADHIT", 2, [](const std::vector<Value>& a){
    int i = (int)a[0].as_int(); int btn = (int)a[1].as_int();
    return Value::from_bool(bas::input::gamepad_hit(i, btn));
  }});
  R.add("GAMEPADUP", NativeFn{"GAMEPADUP", 2, [](const std::vector<Value>& a){
    int i = (int)a[0].as_int(); int btn = (int)a[1].as_int();
    return Value::from_bool(bas::input::gamepad_up(i, btn));
  }});
  R.add("GAMEPADAXIS", NativeFn{"GAMEPADAXIS", 2, [](const std::vector<Value>& a){
    int i = (int)a[0].as_int(); int axis = (int)a[1].as_int();
    return Value::from_number((double)bas::input::gamepad_axis(i, axis));
  }});

  // SETWINDOWSIZE(w,h): console stub (no-op for now)
  R.add("SETWINDOWSIZE", NativeFn{"SETWINDOWSIZE", 2, [](const std::vector<Value>& a){
    int w = (int)a[0].as_int();
    int h = (int)a[1].as_int();
    if (!IsWindowReady()) { gfx.width = w; gfx.height = h; }
    else SetWindowSize(w, h);
    return Value::nil();
  }});

  // --- Image/Texture and Sprite builtins ---
  // LOADIMAGE(path) -> texId (Texture2D)
  R.add("LOADIMAGE", NativeFn{"LOADIMAGE", 1, [ensureWindow](const std::vector<Value>& a){
    ensureWindow();
    std::string path = a[0].as_string();
    Texture2D tex = LoadTexture(path.c_str());
    long long id = gfx.nextTexId++;
    gfx.textures[id] = tex;
    return Value::from_int(id);
  }});

  // CREATESPRITE(texId) -> spriteId
  R.add("CREATESPRITE", NativeFn{"CREATESPRITE", 1, [](const std::vector<Value>& a){
    long long texId = a[0].as_int();
    auto it = gfx.textures.find(texId);
    if (it == gfx.textures.end()) return Value::from_int(0);
    const auto& t = it->second;
    long long sid = gfx.nextSpriteId++;
    gfx.sprites[sid] = Sprite{texId, 0.0f, 0.0f, (float)t.width, (float)t.height};
    return Value::from_int(sid);
  }});

  // SETSPRITEPOSITION(id, x, y)
  R.add("SETSPRITEPOSITION", NativeFn{"SETSPRITEPOSITION", 3, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    float x = (float)a[1].as_number();
    float y = (float)a[2].as_number();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) { it->second.x = x; it->second.y = y; }
    return Value::nil();
  }});

  // SETSPRITESIZE(id, w, h)
  R.add("SETSPRITESIZE", NativeFn{"SETSPRITESIZE", 3, [](const std::vector<Value>& a){
    long long sid = a[0].as_int();
    float w = (float)a[1].as_number();
    float h = (float)a[2].as_number();
    auto it = gfx.sprites.find(sid);
    if (it != gfx.sprites.end()) { it->second.w = w; it->second.h = h; }
    return Value::nil();
  }});

  // DRAWSPRITE(id)
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

  // --- Audio builtins (raylib) ---
  // We lazily initialize the audio device on first audio call to avoid startup cost.
  struct AudioState {
    bool inited = false;
    std::unordered_map<long long, std::unique_ptr<Sound>> sounds; // id -> Sound
    long long nextId = 1;
  };
  static AudioState audio;

  auto ensureAudio = [](){
    if (!audio.inited) {
      InitAudioDevice();
      audio.inited = true;
    }
  };

  // LOADSOUND(path) -> id
  R.add("LOADSOUND", NativeFn{"LOADSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    std::string path = a[0].as_string();
    auto snd = std::make_unique<Sound>(LoadSound(path.c_str()));
    // Basic validity check: in raylib Sound has a frameCount field; 0 suggests failure
    // We allow storing regardless; PlaySound on an invalid Sound is a no-op in raylib.
    long long id = audio.nextId++;
    audio.sounds[id] = std::move(snd);
    return Value::from_int(id);
  }});

  // PLAYSOUND(id)
  R.add("PLAYSOUND", NativeFn{"PLAYSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      PlaySound(*it->second);
    }
    return Value::nil();
  }});

  // STOPSOUND(id)
  R.add("STOPSOUND", NativeFn{"STOPSOUND", 1, [ensureAudio](const std::vector<Value>& a){
    ensureAudio();
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      ::StopSound(*it->second);
    }
    return Value::nil();
  }});

  // UNLOADSOUND(id)
  R.add("UNLOADSOUND", NativeFn{"UNLOADSOUND", 1, [](const std::vector<Value>& a){
    long long id = a[0].as_int();
    auto it = audio.sounds.find(id);
    if (it != audio.sounds.end() && it->second) {
      ::UnloadSound(*it->second);
      audio.sounds.erase(it);
    }
    return Value::nil();
  }});

  // CLOSEAUDIO(): unload all sounds and close the audio device
  R.add("CLOSEAUDIO", NativeFn{"CLOSEAUDIO", 0, [](const std::vector<Value>&){
    for (auto it = audio.sounds.begin(); it != audio.sounds.end(); ++it) {
      if (it->second) ::UnloadSound(*it->second);
    }
    audio.sounds.clear();
    if (audio.inited) { CloseAudioDevice(); audio.inited = false; }
    return Value::nil();
  }});
}
