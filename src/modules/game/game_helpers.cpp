#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <cmath>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>

namespace bas {

// Math helpers for game development

// Math.clamp(value, min, max) - clamps value between min and max
static Value math_clamp(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_number(0.0);
    }
    
    double value = args[0].as_number();
    double minVal = args[1].as_number();
    double maxVal = args[2].as_number();
    
    if (value < minVal) return Value::from_number(minVal);
    if (value > maxVal) return Value::from_number(maxVal);
    return Value::from_number(value);
}

// Math.lerp(a, b, t) - linear interpolation
static Value math_lerp(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_number(0.0);
    }
    
    double a = args[0].as_number();
    double b = args[1].as_number();
    double t = args[2].as_number();
    
    // Clamp t between 0 and 1
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    
    return Value::from_number(a + (b - a) * t);
}

// Math.lerpAngle(a, b, t) - lerp between angles (handles wrap-around)
static Value math_lerpAngle(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_number(0.0);
    }
    
    double a = args[0].as_number();
    double b = args[1].as_number();
    double t = args[2].as_number();
    
    // Normalize angles to 0-360
    a = std::fmod(a, 360.0);
    if (a < 0) a += 360.0;
    b = std::fmod(b, 360.0);
    if (b < 0) b += 360.0;
    
    // Find shortest path
    double diff = b - a;
    if (std::abs(diff) > 180.0) {
        if (diff > 0) diff -= 360.0;
        else diff += 360.0;
    }
    
    double result = a + diff * t;
    result = std::fmod(result, 360.0);
    if (result < 0) result += 360.0;
    
    return Value::from_number(result);
}

// Math.moveTowards(current, target, maxDelta) - move towards target
static Value math_moveTowards(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_number(0.0);
    }
    
    double current = args[0].as_number();
    double target = args[1].as_number();
    double maxDelta = std::abs(args[2].as_number());
    
    double diff = target - current;
    if (std::abs(diff) <= maxDelta) {
        return Value::from_number(target);
    }
    
    return Value::from_number(current + (diff > 0 ? maxDelta : -maxDelta));
}

// Math.smoothDamp(current, target, velocity, smoothTime, deltaTime) - smooth damping
static Value math_smoothDamp(const std::vector<Value>& args) {
    if (args.size() < 5) {
        return Value::from_number(0.0);
    }
    
    double current = args[0].as_number();
    double target = args[1].as_number();
    // velocity would be passed by reference in real implementation
    double smoothTime = args[3].as_number();
    double deltaTime = args[4].as_number();
    
    if (smoothTime <= 0.0) {
        return Value::from_number(target);
    }
    
    double omega = 2.0 / smoothTime;
    double x = omega * deltaTime;
    double exp = 1.0 / (1.0 + x + 0.48 * x * x + 0.235 * x * x * x);
    double change = current - target;
    double temp = (target + change * exp);
    
    return Value::from_number(temp);
}

// Draw helpers for 2D games

// Draw.drawRectangle(x, y, width, height, color)
static Value draw_drawRectangle(const std::vector<Value>& args) {
    if (args.size() < 5) {
        return Value::nil();
    }
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float width = static_cast<float>(args[2].as_number());
    float height = static_cast<float>(args[3].as_number());
    
    // Parse color
    unsigned char r = static_cast<unsigned char>(args[4].as_int());
    unsigned char g = args.size() > 5 ? static_cast<unsigned char>(args[5].as_int()) : r;
    unsigned char b = args.size() > 6 ? static_cast<unsigned char>(args[6].as_int()) : r;
    unsigned char a = args.size() > 7 ? static_cast<unsigned char>(args[7].as_int()) : 255;
    
    Color color{r, g, b, a};
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), 
                  static_cast<int>(width), static_cast<int>(height), color);
    
    return Value::nil();
}

// Draw.drawCircle(x, y, radius, color)
static Value draw_drawCircle(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::nil();
    }
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float radius = static_cast<float>(args[2].as_number());
    
    unsigned char r = static_cast<unsigned char>(args[3].as_int());
    unsigned char g = args.size() > 4 ? static_cast<unsigned char>(args[4].as_int()) : r;
    unsigned char b = args.size() > 5 ? static_cast<unsigned char>(args[5].as_int()) : r;
    unsigned char a = args.size() > 6 ? static_cast<unsigned char>(args[6].as_int()) : 255;
    
    Color color{r, g, b, a};
    DrawCircle(static_cast<int>(x), static_cast<int>(y), radius, color);
    
    return Value::nil();
}

// Draw.drawLine(x1, y1, x2, y2, color)
static Value draw_drawLine(const std::vector<Value>& args) {
    if (args.size() < 5) {
        return Value::nil();
    }
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float x2 = static_cast<float>(args[2].as_number());
    float y2 = static_cast<float>(args[3].as_number());
    
    unsigned char r = static_cast<unsigned char>(args[4].as_int());
    unsigned char g = args.size() > 5 ? static_cast<unsigned char>(args[5].as_int()) : r;
    unsigned char b = args.size() > 6 ? static_cast<unsigned char>(args[6].as_int()) : r;
    unsigned char a = args.size() > 7 ? static_cast<unsigned char>(args[7].as_int()) : 255;
    
    Color color{r, g, b, a};
    DrawLine(static_cast<int>(x1), static_cast<int>(y1), 
             static_cast<int>(x2), static_cast<int>(y2), color);
    
    return Value::nil();
}

// Draw.drawText(text, x, y, fontSize, color)
static Value draw_drawText(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::nil();
    }
    
    std::string text = args[0].is_string() ? args[0].as_string() : "";
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    int fontSize = static_cast<int>(args[3].as_int());
    
    unsigned char r = args.size() > 4 ? static_cast<unsigned char>(args[4].as_int()) : 255;
    unsigned char g = args.size() > 5 ? static_cast<unsigned char>(args[5].as_int()) : 255;
    unsigned char b = args.size() > 6 ? static_cast<unsigned char>(args[6].as_int()) : 255;
    unsigned char a = args.size() > 7 ? static_cast<unsigned char>(args[7].as_int()) : 255;
    
    Color color{r, g, b, a};
    DrawText(text.c_str(), static_cast<int>(x), static_cast<int>(y), fontSize, color);
    
    return Value::nil();
}

// 3D Draw helpers

// Draw3D.drawCube(position, width, height, length, color)
static Value draw3d_drawCube(const std::vector<Value>& args) {
    if (args.size() < 7) {
        return Value::nil();
    }
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float z = static_cast<float>(args[2].as_number());
    float width = static_cast<float>(args[3].as_number());
    float height = static_cast<float>(args[4].as_number());
    float length = static_cast<float>(args[5].as_number());
    
    unsigned char r = static_cast<unsigned char>(args[6].as_int());
    unsigned char g = args.size() > 7 ? static_cast<unsigned char>(args[7].as_int()) : r;
    unsigned char b = args.size() > 8 ? static_cast<unsigned char>(args[8].as_int()) : r;
    unsigned char a = args.size() > 9 ? static_cast<unsigned char>(args[9].as_int()) : 255;
    
    Color color{r, g, b, a};
    Vector3 position{x, y, z};
    Vector3 size{width, height, length};
    
    DrawCube(position, size.x, size.y, size.z, color);
    
    return Value::nil();
}

// Draw3D.drawSphere(position, radius, color)
static Value draw3d_drawSphere(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::nil();
    }
    
    float x = static_cast<float>(args[0].as_number());
    float y = static_cast<float>(args[1].as_number());
    float z = static_cast<float>(args[2].as_number());
    float radius = static_cast<float>(args[3].as_number());
    
    unsigned char r = args.size() > 4 ? static_cast<unsigned char>(args[4].as_int()) : 255;
    unsigned char g = args.size() > 5 ? static_cast<unsigned char>(args[5].as_int()) : 255;
    unsigned char b = args.size() > 6 ? static_cast<unsigned char>(args[6].as_int()) : 255;
    unsigned char a = args.size() > 7 ? static_cast<unsigned char>(args[7].as_int()) : 255;
    
    Color color{r, g, b, a};
    Vector3 position{x, y, z};
    
    DrawSphere(position, radius, color);
    
    return Value::nil();
}

// Draw3D.drawGrid(slices, spacing)
static Value draw3d_drawGrid(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    int slices = static_cast<int>(args[0].as_int());
    float spacing = static_cast<float>(args[1].as_number());
    
    DrawGrid(slices, spacing);
    
    return Value::nil();
}

// Register game helper functions
void register_game_helpers(FunctionRegistry& R) {
    // Math helpers
    R.add("MATH_CLAMP", NativeFn{"MATH_CLAMP", 3, math_clamp});
    R.add("MATH_LERP", NativeFn{"MATH_LERP", 3, math_lerp});
    R.add("MATH_LERPANGLE", NativeFn{"MATH_LERPANGLE", 3, math_lerpAngle});
    R.add("MATH_MOVETOWARDS", NativeFn{"MATH_MOVETOWARDS", 3, math_moveTowards});
    R.add("MATH_SMOOTHDAMP", NativeFn{"MATH_SMOOTHDAMP", 5, math_smoothDamp});
    
    // 2D Draw helpers
    R.add("DRAW_DRAWRECTANGLE", NativeFn{"DRAW_DRAWRECTANGLE", -1, draw_drawRectangle});
    R.add("DRAW_DRAWCIRCLE", NativeFn{"DRAW_DRAWCIRCLE", -1, draw_drawCircle});
    R.add("DRAW_DRAWLINE", NativeFn{"DRAW_DRAWLINE", -1, draw_drawLine});
    R.add("DRAW_DRAWTEXT", NativeFn{"DRAW_DRAWTEXT", -1, draw_drawText});
    
    // 3D Draw helpers
    R.add("DRAW3D_DRAWCUBE", NativeFn{"DRAW3D_DRAWCUBE", -1, draw3d_drawCube});
    R.add("DRAW3D_DRAWSPHERE", NativeFn{"DRAW3D_DRAWSPHERE", -1, draw3d_drawSphere});
    R.add("DRAW3D_DRAWGRID", NativeFn{"DRAW3D_DRAWGRID", 2, draw3d_drawGrid});
}

// Asset Manager
static std::map<std::string, Value> asset_cache;

void register_asset_manager(FunctionRegistry& R) {
    R.add("ASSETS_LOAD", NativeFn{"ASSETS_LOAD", 1, [](const std::vector<Value>& args) -> Value {
        std::string path = args[0].as_string();
        // In a real implementation, this would load the asset
        // For now, just cache the path
        asset_cache[path] = Value::from_string(path);
        return Value::from_bool(true);
    }});
    
    R.add("ASSETS_GET", NativeFn{"ASSETS_GET", 1, [](const std::vector<Value>& args) -> Value {
        std::string path = args[0].as_string();
        auto it = asset_cache.find(path);
        if (it != asset_cache.end()) {
            return it->second;
        }
        return Value::nil();
    }});
    
    R.add("ASSETS_LOADALL", NativeFn{"ASSETS_LOADALL", 1, [](const std::vector<Value>& args) -> Value {
        std::string dir = args[0].as_string();
        // In a real implementation, this would load all assets in directory
        return Value::from_int(0);
    }});
}

// Save/Load System
void register_save_load(FunctionRegistry& R) {
    static std::map<std::string, Value> save_data;
    
    R.add("SAVE_SAVE", NativeFn{"SAVE_SAVE", 2, [](const std::vector<Value>& args) -> Value {
        std::string key = args[0].as_string();
        save_data[key] = args[1];
        return Value::nil();
    }});
    
    R.add("SAVE_LOAD", NativeFn{"SAVE_LOAD", 1, [](const std::vector<Value>& args) -> Value {
        std::string key = args[0].as_string();
        auto it = save_data.find(key);
        if (it != save_data.end()) {
            return it->second;
        }
        return Value::nil();
    }});
    
    R.add("SAVE_EXISTS", NativeFn{"SAVE_EXISTS", 1, [](const std::vector<Value>& args) -> Value {
        std::string key = args[0].as_string();
        return Value::from_bool(save_data.find(key) != save_data.end());
    }});
}

// Profiling System
static std::map<std::string, std::chrono::steady_clock::time_point> profile_starts;
static std::map<std::string, double> profile_times;

void register_profiling(FunctionRegistry& R) {
    R.add_with_policy("PROFILE_START", NativeFn{"PROFILE_START", 1, [](const std::vector<Value>& args) -> Value {
        std::string name = args[0].as_string();
        profile_starts[name] = std::chrono::steady_clock::now();
        return Value::nil();
    }}, true);
    
    R.add_with_policy("PROFILE_END", NativeFn{"PROFILE_END", 1, [](const std::vector<Value>& args) -> Value {
        std::string name = args[0].as_string();
        auto it = profile_starts.find(name);
        if (it != profile_starts.end()) {
            auto elapsed = std::chrono::steady_clock::now() - it->second;
            double seconds = std::chrono::duration<double>(elapsed).count();
            profile_times[name] = seconds;
            profile_starts.erase(it);
        }
        return Value::nil();
    }}, true);
    
    R.add("PROFILE_GETTIME", NativeFn{"PROFILE_GETTIME", 1, [](const std::vector<Value>& args) -> Value {
        std::string name = args[0].as_string();
        auto it = profile_times.find(name);
        if (it != profile_times.end()) {
            return Value::from_number(it->second);
        }
        return Value::from_number(0.0);
    }});
}

// Debug Visualization
void register_debug_viz(FunctionRegistry& R) {
    R.add("DEBUG_DRAWCOLLISIONBOXES", NativeFn{"DEBUG_DRAWCOLLISIONBOXES", 1, [](const std::vector<Value>& args) -> Value {
        bool enabled = args[0].as_bool();
        // In a real implementation, this would toggle collision box rendering
        (void)enabled;
        return Value::nil();
    }});
    
    R.add("DEBUG_SHOWFPS", NativeFn{"DEBUG_SHOWFPS", 1, [](const std::vector<Value>& args) -> Value {
        bool enabled = args[0].as_bool();
        // In a real implementation, this would toggle FPS display
        (void)enabled;
        return Value::nil();
    }});
    
    R.add("DEBUG_SHOWMEMORY", NativeFn{"DEBUG_SHOWMEMORY", 1, [](const std::vector<Value>& args) -> Value {
        bool enabled = args[0].as_bool();
        // In a real implementation, this would toggle memory display
        (void)enabled;
        return Value::nil();
    }});
}

} // namespace bas

