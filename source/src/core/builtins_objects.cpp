#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace bas;

// Helper to create an object (map) with properties
[[maybe_unused]] static Value create_object(const std::string& type_name) {
    Value::Map obj;
    obj["_type"] = Value::from_string(type_name);
    return Value::from_map(std::move(obj));
}

// Vector3 object constructor: Vector3(x, y, z)
static Value vector3_constructor(const std::vector<Value>& args) {
    double x = args.size() > 0 ? args[0].as_number() : 0.0;
    double y = args.size() > 1 ? args[1].as_number() : 0.0;
    double z = args.size() > 2 ? args[2].as_number() : 0.0;
    
    Value::Map vec;
    vec["_type"] = Value::from_string("Vector3");
    vec["x"] = Value::from_number(x);
    vec["y"] = Value::from_number(y);
    vec["z"] = Value::from_number(z);
    
    return Value::from_map(std::move(vec));
}

// Vector2 object constructor: Vector2(x, y)
static Value vector2_constructor(const std::vector<Value>& args) {
    double x = args.size() > 0 ? args[0].as_number() : 0.0;
    double y = args.size() > 1 ? args[1].as_number() : 0.0;
    
    Value::Map vec;
    vec["_type"] = Value::from_string("Vector2");
    vec["x"] = Value::from_number(x);
    vec["y"] = Value::from_number(y);
    
    return Value::from_map(std::move(vec));
}

// Color object constructor: Color(r, g, b, a)
static Value color_constructor(const std::vector<Value>& args) {
    double r = args.size() > 0 ? args[0].as_number() : 0.0;
    double g = args.size() > 1 ? args[1].as_number() : 0.0;
    double b = args.size() > 2 ? args[2].as_number() : 0.0;
    double a = args.size() > 3 ? args[3].as_number() : 255.0;
    
    // Clamp values to 0-255
    r = std::max(0.0, std::min(255.0, r));
    g = std::max(0.0, std::min(255.0, g));
    b = std::max(0.0, std::min(255.0, b));
    a = std::max(0.0, std::min(255.0, a));
    
    Value::Map color;
    color["_type"] = Value::from_string("Color");
    color["r"] = Value::from_number(r);
    color["g"] = Value::from_number(g);
    color["b"] = Value::from_number(b);
    color["a"] = Value::from_number(a);
    
    return Value::from_map(std::move(color));
}

// Rectangle object constructor: Rectangle(x, y, width, height)
static Value rectangle_constructor(const std::vector<Value>& args) {
    double x = args.size() > 0 ? args[0].as_number() : 0.0;
    double y = args.size() > 1 ? args[1].as_number() : 0.0;
    double w = args.size() > 2 ? args[2].as_number() : 0.0;
    double h = args.size() > 3 ? args[3].as_number() : 0.0;
    
    Value::Map rect;
    rect["_type"] = Value::from_string("Rectangle");
    rect["x"] = Value::from_number(x);
    rect["y"] = Value::from_number(y);
    rect["width"] = Value::from_number(w);
    rect["height"] = Value::from_number(h);
    
    return Value::from_map(std::move(rect));
}

// Camera3D object constructor: Camera3D()
static Value camera3d_constructor(const std::vector<Value>& args) {
    Value::Map camera;
    camera["_type"] = Value::from_string("Camera3D");
    
    // Default camera properties - create Vector3 objects properly
    Value::Map position;
    position["_type"] = Value::from_string("Vector3");
    position["x"] = Value::from_number(0.0);
    position["y"] = Value::from_number(0.0);
    position["z"] = Value::from_number(10.0);
    
    Value::Map target;
    target["_type"] = Value::from_string("Vector3");
    target["x"] = Value::from_number(0.0);
    target["y"] = Value::from_number(0.0);
    target["z"] = Value::from_number(0.0);
    
    Value::Map up;
    up["_type"] = Value::from_string("Vector3");
    up["x"] = Value::from_number(0.0);
    up["y"] = Value::from_number(1.0);
    up["z"] = Value::from_number(0.0);
    
    camera["position"] = Value::from_map(std::move(position));
    camera["target"] = Value::from_map(std::move(target));
    camera["up"] = Value::from_map(std::move(up));
    camera["fovy"] = Value::from_number(60.0);
    camera["projection"] = Value::from_number(0.0); // CAMERA_PERSPECTIVE
    
    return Value::from_map(std::move(camera));
}

// Helper to extract Vector3 components
static void get_vector3_components(const Value& vec, double& x, double& y, double& z) {
    if(!vec.is_map()) {
        x = y = z = 0.0;
        return;
    }
    const auto& map = vec.as_map();
    x = map.count("x") ? map.at("x").as_number() : 0.0;
    y = map.count("y") ? map.at("y").as_number() : 0.0;
    z = map.count("z") ? map.at("z").as_number() : 0.0;
}

// Vector3 methods
static Value vector3_length(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::from_number(0.0);
    }
    double x, y, z;
    get_vector3_components(args[0], x, y, z);
    return Value::from_number(std::sqrt(x*x + y*y + z*z));
}

static Value vector3_normalize(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return vector3_constructor({});
    }
    double x, y, z;
    get_vector3_components(args[0], x, y, z);
    
    double len = std::sqrt(x*x + y*y + z*z);
    if(len < 0.0001) {
        return vector3_constructor({});
    }
    return vector3_constructor({
        Value::from_number(x / len),
        Value::from_number(y / len),
        Value::from_number(z / len)
    });
}

static Value vector3_dot(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::from_number(0.0);
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    return Value::from_number(x1*x2 + y1*y2 + z1*z2);
}

static Value vector3_cross(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return vector3_constructor({});
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    return vector3_constructor({
        Value::from_number(y1*z2 - z1*y2),
        Value::from_number(z1*x2 - x1*z2),
        Value::from_number(x1*y2 - y1*x2)
    });
}

static Value vector3_distance(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::from_number(0.0);
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return Value::from_number(std::sqrt(dx*dx + dy*dy + dz*dz));
}

// Vector2 methods
static void get_vector2_components(const Value& vec, double& x, double& y) {
    if(!vec.is_map()) {
        x = y = 0.0;
        return;
    }
    const auto& map = vec.as_map();
    x = map.count("x") ? map.at("x").as_number() : 0.0;
    y = map.count("y") ? map.at("y").as_number() : 0.0;
}

static Value vector2_length(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::from_number(0.0);
    }
    double x, y;
    get_vector2_components(args[0], x, y);
    return Value::from_number(std::sqrt(x*x + y*y));
}

static Value vector2_normalize(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return vector2_constructor({});
    }
    double x, y;
    get_vector2_components(args[0], x, y);
    double len = std::sqrt(x*x + y*y);
    if(len < 0.0001) {
        return vector2_constructor({});
    }
    return vector2_constructor({
        Value::from_number(x / len),
        Value::from_number(y / len)
    });
}

static Value vector2_dot(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::from_number(0.0);
    }
    double x1, y1, x2, y2;
    get_vector2_components(args[0], x1, y1);
    get_vector2_components(args[1], x2, y2);
    return Value::from_number(x1*x2 + y1*y2);
}

// Color methods
static Value color_brightness(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::from_number(0.0);
    }
    const auto& color = args[0].as_map();
    double r = color.count("r") ? color.at("r").as_number() : 0.0;
    double g = color.count("g") ? color.at("g").as_number() : 0.0;
    double b = color.count("b") ? color.at("b").as_number() : 0.0;
    // Calculate perceived brightness (luminance)
    return Value::from_number(0.299*r + 0.587*g + 0.114*b);
}

static Value color_darken(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return color_constructor({});
    }
    const auto& color = args[0].as_map();
    double r = color.count("r") ? color.at("r").as_number() : 0.0;
    double g = color.count("g") ? color.at("g").as_number() : 0.0;
    double b = color.count("b") ? color.at("b").as_number() : 0.0;
    double a = color.count("a") ? color.at("a").as_number() : 255.0;
    double factor = args.size() > 1 ? args[1].as_number() : 0.5;
    factor = std::max(0.0, std::min(1.0, factor));
    return color_constructor({
        Value::from_number(r * factor),
        Value::from_number(g * factor),
        Value::from_number(b * factor),
        Value::from_number(a)
    });
}

// More Vector3 methods for chaining
static Value vector3_scale(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return vector3_constructor({});
    }
    double x, y, z;
    get_vector3_components(args[0], x, y, z);
    double factor = args.size() > 1 ? args[1].as_number() : 1.0;
    return vector3_constructor({
        Value::from_number(x * factor),
        Value::from_number(y * factor),
        Value::from_number(z * factor)
    });
}

static Value vector3_add(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return vector3_constructor({});
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    return vector3_constructor({
        Value::from_number(x1 + x2),
        Value::from_number(y1 + y2),
        Value::from_number(z1 + z2)
    });
}

static Value vector3_subtract(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return vector3_constructor({});
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    return vector3_constructor({
        Value::from_number(x1 - x2),
        Value::from_number(y1 - y2),
        Value::from_number(z1 - z2)
    });
}

static Value vector3_multiply(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return vector3_constructor({});
    }
    double x, y, z;
    get_vector3_components(args[0], x, y, z);
    double factor = args.size() > 1 ? args[1].as_number() : 1.0;
    return vector3_constructor({
        Value::from_number(x * factor),
        Value::from_number(y * factor),
        Value::from_number(z * factor)
    });
}

static Value vector3_lerp(const std::vector<Value>& args) {
    if(args.size() < 3 || !args[0].is_map() || !args[1].is_map()) {
        return vector3_constructor({});
    }
    double x1, y1, z1, x2, y2, z2;
    get_vector3_components(args[0], x1, y1, z1);
    get_vector3_components(args[1], x2, y2, z2);
    double t = args[2].as_number();
    t = std::max(0.0, std::min(1.0, t));
    return vector3_constructor({
        Value::from_number(x1 + (x2 - x1) * t),
        Value::from_number(y1 + (y2 - y1) * t),
        Value::from_number(z1 + (z2 - z1) * t)
    });
}

// More Vector2 methods
static Value vector2_scale(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return vector2_constructor({});
    }
    double x, y;
    get_vector2_components(args[0], x, y);
    double factor = args.size() > 1 ? args[1].as_number() : 1.0;
    return vector2_constructor({
        Value::from_number(x * factor),
        Value::from_number(y * factor)
    });
}

static Value vector2_add(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return vector2_constructor({});
    }
    double x1, y1, x2, y2;
    get_vector2_components(args[0], x1, y1);
    get_vector2_components(args[1], x2, y2);
    return vector2_constructor({
        Value::from_number(x1 + x2),
        Value::from_number(y1 + y2)
    });
}

static Value vector2_subtract(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return vector2_constructor({});
    }
    double x1, y1, x2, y2;
    get_vector2_components(args[0], x1, y1);
    get_vector2_components(args[1], x2, y2);
    return vector2_constructor({
        Value::from_number(x1 - x2),
        Value::from_number(y1 - y2)
    });
}

static Value vector2_lerp(const std::vector<Value>& args) {
    if(args.size() < 3 || !args[0].is_map() || !args[1].is_map()) {
        return vector2_constructor({});
    }
    double x1, y1, x2, y2;
    get_vector2_components(args[0], x1, y1);
    get_vector2_components(args[1], x2, y2);
    double t = args[2].as_number();
    t = std::max(0.0, std::min(1.0, t));
    return vector2_constructor({
        Value::from_number(x1 + (x2 - x1) * t),
        Value::from_number(y1 + (y2 - y1) * t)
    });
}

// More Color methods
static Value color_lighten(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return color_constructor({});
    }
    const auto& color = args[0].as_map();
    double r = color.count("r") ? color.at("r").as_number() : 0.0;
    double g = color.count("g") ? color.at("g").as_number() : 0.0;
    double b = color.count("b") ? color.at("b").as_number() : 0.0;
    double a = color.count("a") ? color.at("a").as_number() : 255.0;
    double factor = args.size() > 1 ? args[1].as_number() : 0.5;
    factor = std::max(0.0, std::min(1.0, factor));
    return color_constructor({
        Value::from_number(std::min(255.0, r + (255.0 - r) * factor)),
        Value::from_number(std::min(255.0, g + (255.0 - g) * factor)),
        Value::from_number(std::min(255.0, b + (255.0 - b) * factor)),
        Value::from_number(a)
    });
}

static Value color_mix(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return color_constructor({});
    }
    const auto& c1 = args[0].as_map();
    const auto& c2 = args[1].as_map();
    double r1 = c1.count("r") ? c1.at("r").as_number() : 0.0;
    double g1 = c1.count("g") ? c1.at("g").as_number() : 0.0;
    double b1 = c1.count("b") ? c1.at("b").as_number() : 0.0;
    double a1 = c1.count("a") ? c1.at("a").as_number() : 255.0;
    double r2 = c2.count("r") ? c2.at("r").as_number() : 0.0;
    double g2 = c2.count("g") ? c2.at("g").as_number() : 0.0;
    double b2 = c2.count("b") ? c2.at("b").as_number() : 0.0;
    double a2 = c2.count("a") ? c2.at("a").as_number() : 255.0;
    double t = args.size() > 2 ? args[2].as_number() : 0.5;
    t = std::max(0.0, std::min(1.0, t));
    return color_constructor({
        Value::from_number(r1 + (r2 - r1) * t),
        Value::from_number(g1 + (g2 - g1) * t),
        Value::from_number(b1 + (b2 - b1) * t),
        Value::from_number(a1 + (a2 - a1) * t)
    });
}

static Value color_toHex(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::from_string("#000000");
    }
    const auto& color = args[0].as_map();
    int r = static_cast<int>(color.count("r") ? color.at("r").as_number() : 0.0);
    int g = static_cast<int>(color.count("g") ? color.at("g").as_number() : 0.0);
    int b = static_cast<int>(color.count("b") ? color.at("b").as_number() : 0.0);
    r = std::max(0, std::min(255, r));
    g = std::max(0, std::min(255, g));
    b = std::max(0, std::min(255, b));
    char hex[8];
    std::snprintf(hex, sizeof(hex), "#%02X%02X%02X", r, g, b);
    return Value::from_string(hex);
}

// String methods
static Value string_upper(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_string()) {
        return Value::from_string("");
    }
    std::string str = args[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](unsigned char c) { return std::toupper(c); });
    return Value::from_string(str);
}

static Value string_lower(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_string()) {
        return Value::from_string("");
    }
    std::string str = args[0].as_string();
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    return Value::from_string(str);
}

static Value string_trim(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_string()) {
        return Value::from_string("");
    }
    std::string str = args[0].as_string();
    // Trim leading whitespace
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), 
        [](unsigned char c) { return !std::isspace(c); }));
    // Trim trailing whitespace
    str.erase(std::find_if(str.rbegin(), str.rend(), 
        [](unsigned char c) { return !std::isspace(c); }).base(), str.end());
    return Value::from_string(str);
}

static Value string_split(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_string()) {
        return Value::from_array({});
    }
    std::string str = args[0].as_string();
    std::string delimiter = args.size() > 1 ? args[1].as_string() : " ";
    
    Value::Array result;
    size_t pos = 0;
    while((pos = str.find(delimiter)) != std::string::npos) {
        result.push_back(Value::from_string(str.substr(0, pos)));
        str.erase(0, pos + delimiter.length());
    }
    if(!str.empty()) {
        result.push_back(Value::from_string(str));
    }
    return Value::from_array(std::move(result));
}

static Value string_contains(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_string() || !args[1].is_string()) {
        return Value::from_bool(false);
    }
    std::string str = args[0].as_string();
    std::string substr = args[1].as_string();
    return Value::from_bool(str.find(substr) != std::string::npos);
}

// Array methods
static Value array_length(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_array()) {
        return Value::from_int(0);
    }
    return Value::from_int(static_cast<long long>(args[0].as_array().size()));
}

static Value array_first(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_array() || args[0].as_array().empty()) {
        return Value::nil();
    }
    return args[0].as_array()[0];
}

static Value array_last(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_array() || args[0].as_array().empty()) {
        return Value::nil();
    }
    const auto& arr = args[0].as_array();
    return arr[arr.size() - 1];
}

static Value array_push(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_array()) {
        return Value::from_array({});
    }
    Value result = args[0];
    auto& arr = result.as_array();
    for(size_t i = 1; i < args.size(); ++i) {
        arr.push_back(args[i]);
    }
    return result;
}

static Value array_pop(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_array() || args[0].as_array().empty()) {
        return Value::nil();
    }
    Value result = args[0];
    auto& arr = result.as_array();
    Value last = arr.back();
    arr.pop_back();
    return last;
}

// Texture2D object constructor - wraps texture ID
static Value texture2d_constructor(const std::vector<Value>& args) {
    if(args.empty()) {
        Value::Map tex;
        tex["_type"] = Value::from_string("Texture2D");
        tex["_id"] = Value::from_int(-1); // Invalid ID
        return Value::from_map(std::move(tex));
    }
    // If first arg is a string, it's a file path - use LOADTEXTURE
    // If first arg is an int, it's an existing texture ID
    Value::Map tex;
    tex["_type"] = Value::from_string("Texture2D");
    if(args[0].is_string()) {
        // Will be loaded via Texture2D.load() method
        tex["_path"] = args[0];
        tex["_id"] = Value::from_int(-1);
    } else {
        tex["_id"] = args[0];
    }
    return Value::from_map(std::move(tex));
}

// Texture2D methods
static Value texture2d_load(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return texture2d_constructor({});
    }
    const auto& tex = args[0].as_map();
    auto path_it = tex.find("_path");
    if(path_it == tex.end() || !path_it->second.is_string()) {
        return args[0]; // Return as-is if no path
    }
    // Call LOADTEXTURE via function registry - this will be handled by the method call system
    // For now, return the object with path set
    return args[0];
}

static Value texture2d_getWidth(const std::vector<Value>& args) {
    // This would need access to raylib resources - simplified for now
    return Value::from_int(0);
}

static Value texture2d_getHeight(const std::vector<Value>& args) {
    return Value::from_int(0);
}

// Model object constructor
static Value model_constructor(const std::vector<Value>& args) {
    Value::Map model;
    model["_type"] = Value::from_string("Model");
    if(args.empty()) {
        model["_id"] = Value::from_int(-1);
    } else if(args[0].is_string()) {
        model["_path"] = args[0];
        model["_id"] = Value::from_int(-1);
    } else {
        model["_id"] = args[0];
    }
    return Value::from_map(std::move(model));
}

// Sound object constructor
static Value sound_constructor(const std::vector<Value>& args) {
    Value::Map sound;
    sound["_type"] = Value::from_string("Sound");
    if(args.empty()) {
        sound["_id"] = Value::from_int(-1);
    } else if(args[0].is_string()) {
        sound["_path"] = args[0];
        sound["_id"] = Value::from_int(-1);
    } else {
        sound["_id"] = args[0];
    }
    return Value::from_map(std::move(sound));
}

// Sound methods
static Value sound_play(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    // Would call PLAYSOUND with the ID
    return Value::nil();
}

static Value sound_stop(const std::vector<Value>& args) {
    if(args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    // Would call STOPSOUND with the ID
    return Value::nil();
}

static Value sound_setVolume(const std::vector<Value>& args) {
    if(args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    // Would call SETSOUNDVOLUME with the ID and volume
    return Value::nil();
}

// Helper to create nested arrays recursively
static Value create_nested_array(const std::vector<long long>& dimensions, size_t currentDim) {
    if (currentDim >= dimensions.size()) {
        return Value::nil();
    }
    
    long long size = dimensions[currentDim];
    if (size < 0) size = 0;
    
    Value::Array arr;
    arr.reserve(static_cast<size_t>(size));
    
    if (currentDim == dimensions.size() - 1) {
        // Last dimension - create leaf elements
        for (long long i = 0; i < size; ++i) {
            arr.push_back(Value::nil());
        }
    } else {
        // Intermediate dimension - create nested arrays
        for (long long i = 0; i < size; ++i) {
            arr.push_back(create_nested_array(dimensions, currentDim + 1));
        }
    }
    
    return Value::from_array(std::move(arr));
}

// Array constructor: Array(size) or Array(width, height) for 2D, Array(x, y, z) for 3D, etc.
static Value array_constructor(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::from_array({});
    }
    
    // Collect dimensions
    std::vector<long long> dimensions;
    dimensions.reserve(args.size());
    for (const auto& arg : args) {
        long long dim = arg.as_int();
        if (dim < 0) dim = 0;
        dimensions.push_back(dim);
    }
    
    // Single dimension: Array(size)
    if (dimensions.size() == 1) {
        std::vector<Value> arr;
        arr.resize(static_cast<size_t>(dimensions[0]), Value::nil());
        return Value::from_array(std::move(arr));
    }
    
    // Multi-dimensional: Array(dim1, dim2, dim3, ...) - creates nested arrays
    // This properly supports 2D, 3D, and higher dimensions
    return create_nested_array(dimensions, 0);
}

// Array helper functions
// Array.getDimensions(arr) - returns array of dimensions [width, height, depth, ...]
static Value array_getDimensions(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_array()) {
        return Value::from_array({});
    }
    
    const auto& arr = args[0].as_array();
    if (arr.empty()) {
        return Value::from_array({Value::from_int(0)});
    }
    
    std::vector<Value> dimensions;
    const Value* current = &args[0];
    
    // Traverse nested arrays to determine dimensions
    while (current->is_array()) {
        const auto& currentArr = current->as_array();
        dimensions.push_back(Value::from_int(static_cast<long long>(currentArr.size())));
        
        if (!currentArr.empty() && currentArr[0].is_array()) {
            current = &currentArr[0];
        } else {
            break;
        }
    }
    
    return Value::from_array(std::move(dimensions));
}

// Array.getSize(arr) - returns total number of elements
static Value array_getSize(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_array()) {
        return Value::from_int(0);
    }
    
    long long total = 1;
    
    // Calculate total size by multiplying all dimensions
    const Value* current = &args[0];
    while (current->is_array()) {
        const auto& currentArr = current->as_array();
        total *= static_cast<long long>(currentArr.size());
        
        if (!currentArr.empty() && currentArr[0].is_array()) {
            current = &currentArr[0];
        } else {
            break;
        }
    }
    
    return Value::from_int(total);
}

// Register object constructors and methods
void bas::register_object_constructors(FunctionRegistry& R) {
    // Object constructors
    R.add("VECTOR3", NativeFn{"VECTOR3", -1, vector3_constructor});
    R.add("VECTOR2", NativeFn{"VECTOR2", -1, vector2_constructor});
    R.add("COLOR", NativeFn{"COLOR", -1, color_constructor});
    R.add("RECTANGLE", NativeFn{"RECTANGLE", -1, rectangle_constructor});
    R.add("CAMERA3D", NativeFn{"CAMERA3D", -1, camera3d_constructor});
    R.add("TEXTURE2D", NativeFn{"TEXTURE2D", -1, texture2d_constructor});
    R.add("MODEL", NativeFn{"MODEL", -1, model_constructor});
    R.add("SOUND", NativeFn{"SOUND", -1, sound_constructor});
    R.add_with_policy("ARRAY", NativeFn{"ARRAY", -1, array_constructor}, true);
    R.add("ARRAY_GETDIMENSIONS", NativeFn{"ARRAY_GETDIMENSIONS", 1, array_getDimensions});
    R.add("ARRAY_GETSIZE", NativeFn{"ARRAY_GETSIZE", 1, array_getSize});
    
    // Texture2D methods
    R.add("TEXTURE2D_LOAD", NativeFn{"TEXTURE2D_LOAD", 1, texture2d_load});
    R.add("TEXTURE2D_GETWIDTH", NativeFn{"TEXTURE2D_GETWIDTH", 1, texture2d_getWidth});
    R.add("TEXTURE2D_GETHEIGHT", NativeFn{"TEXTURE2D_GETHEIGHT", 1, texture2d_getHeight});
    
    // Sound methods
    R.add("SOUND_PLAY", NativeFn{"SOUND_PLAY", 1, sound_play});
    R.add("SOUND_STOP", NativeFn{"SOUND_STOP", 1, sound_stop});
    R.add("SOUND_SETVOLUME", NativeFn{"SOUND_SETVOLUME", 2, sound_setVolume});
    
    // Vector3 methods (accessible via vec.method())
    R.add("VECTOR3_LENGTH", NativeFn{"VECTOR3_LENGTH", 1, vector3_length});
    R.add("VECTOR3_NORMALIZE", NativeFn{"VECTOR3_NORMALIZE", 1, vector3_normalize});
    R.add("VECTOR3_DOT", NativeFn{"VECTOR3_DOT", 2, vector3_dot});
    R.add("VECTOR3_CROSS", NativeFn{"VECTOR3_CROSS", 2, vector3_cross});
    R.add("VECTOR3_DISTANCE", NativeFn{"VECTOR3_DISTANCE", 2, vector3_distance});
    R.add("VECTOR3_SCALE", NativeFn{"VECTOR3_SCALE", -1, vector3_scale});
    R.add("VECTOR3_ADD", NativeFn{"VECTOR3_ADD", 2, vector3_add});
    R.add("VECTOR3_SUBTRACT", NativeFn{"VECTOR3_SUBTRACT", 2, vector3_subtract});
    R.add("VECTOR3_MULTIPLY", NativeFn{"VECTOR3_MULTIPLY", -1, vector3_multiply});
    R.add("VECTOR3_LERP", NativeFn{"VECTOR3_LERP", 3, vector3_lerp});
    
    // Vector2 methods
    R.add("VECTOR2_LENGTH", NativeFn{"VECTOR2_LENGTH", 1, vector2_length});
    R.add("VECTOR2_NORMALIZE", NativeFn{"VECTOR2_NORMALIZE", 1, vector2_normalize});
    R.add("VECTOR2_DOT", NativeFn{"VECTOR2_DOT", 2, vector2_dot});
    R.add("VECTOR2_SCALE", NativeFn{"VECTOR2_SCALE", -1, vector2_scale});
    R.add("VECTOR2_ADD", NativeFn{"VECTOR2_ADD", 2, vector2_add});
    R.add("VECTOR2_SUBTRACT", NativeFn{"VECTOR2_SUBTRACT", 2, vector2_subtract});
    R.add("VECTOR2_LERP", NativeFn{"VECTOR2_LERP", 3, vector2_lerp});
    
    // Color methods
    R.add("COLOR_BRIGHTNESS", NativeFn{"COLOR_BRIGHTNESS", 1, color_brightness});
    R.add("COLOR_DARKEN", NativeFn{"COLOR_DARKEN", -1, color_darken});
    R.add("COLOR_LIGHTEN", NativeFn{"COLOR_LIGHTEN", -1, color_lighten});
    R.add("COLOR_MIX", NativeFn{"COLOR_MIX", -1, color_mix});
    R.add("COLOR_TOHEX", NativeFn{"COLOR_TOHEX", 1, color_toHex});
    
    // String methods (accessible via str.method())
    R.add("STRING_UPPER", NativeFn{"STRING_UPPER", 1, string_upper});
    R.add("STRING_LOWER", NativeFn{"STRING_LOWER", 1, string_lower});
    R.add("STRING_TRIM", NativeFn{"STRING_TRIM", 1, string_trim});
    R.add("STRING_SPLIT", NativeFn{"STRING_SPLIT", -1, string_split});
    R.add("STRING_CONTAINS", NativeFn{"STRING_CONTAINS", 2, string_contains});
    
    // Array methods (accessible via arr.method())
    R.add("ARRAY_LENGTH", NativeFn{"ARRAY_LENGTH", 1, array_length});
    R.add("ARRAY_FIRST", NativeFn{"ARRAY_FIRST", 1, array_first});
    R.add("ARRAY_LAST", NativeFn{"ARRAY_LAST", 1, array_last});
    R.add("ARRAY_PUSH", NativeFn{"ARRAY_PUSH", -1, array_push});
    R.add("ARRAY_POP", NativeFn{"ARRAY_POP", 1, array_pop});
}

