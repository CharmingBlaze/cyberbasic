#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <cmath>

namespace bas {

// Check collision between two rectangles (2D)
static Value collision_checkRectRect(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& rect1 = args[0].as_map();
    const auto& rect2 = args[1].as_map();
    
    auto getFloat = [](const Value::Map& m, const std::string& key, float def) -> float {
        auto it = m.find(key);
        if (it != m.end()) {
            return static_cast<float>(it->second.as_number());
        }
        return def;
    };
    
    Rectangle r1;
    r1.x = getFloat(rect1, "x", 0.0f);
    r1.y = getFloat(rect1, "y", 0.0f);
    r1.width = getFloat(rect1, "width", 0.0f);
    r1.height = getFloat(rect1, "height", 0.0f);
    
    Rectangle r2;
    r2.x = getFloat(rect2, "x", 0.0f);
    r2.y = getFloat(rect2, "y", 0.0f);
    r2.width = getFloat(rect2, "width", 0.0f);
    r2.height = getFloat(rect2, "height", 0.0f);
    
    return Value::from_bool(CheckCollisionRecs(r1, r2));
}

// Check collision between rectangle and point (2D)
static Value collision_checkRectPoint(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& rect = args[0].as_map();
    float x = static_cast<float>(args[1].as_number());
    float y = static_cast<float>(args[2].as_number());
    
    auto getFloat = [](const Value::Map& m, const std::string& key, float def) -> float {
        auto it = m.find(key);
        if (it != m.end()) {
            return static_cast<float>(it->second.as_number());
        }
        return def;
    };
    
    Rectangle r;
    r.x = getFloat(rect, "x", 0.0f);
    r.y = getFloat(rect, "y", 0.0f);
    r.width = getFloat(rect, "width", 0.0f);
    r.height = getFloat(rect, "height", 0.0f);
    
    Vector2 point{x, y};
    return Value::from_bool(CheckCollisionPointRec(point, r));
}

// Check collision between two circles (2D)
static Value collision_checkCircleCircle(const std::vector<Value>& args) {
    if (args.size() < 6) {
        return Value::from_bool(false);
    }
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float r1 = static_cast<float>(args[2].as_number());
    float x2 = static_cast<float>(args[3].as_number());
    float y2 = static_cast<float>(args[4].as_number());
    float r2 = static_cast<float>(args[5].as_number());
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx * dx + dy * dy);
    float minDistance = r1 + r2;
    
    return Value::from_bool(distance < minDistance);
}

// Check collision between circle and point (2D)
static Value collision_checkCirclePoint(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::from_bool(false);
    }
    
    float cx = static_cast<float>(args[0].as_number());
    float cy = static_cast<float>(args[1].as_number());
    float radius = static_cast<float>(args[2].as_number());
    float px = static_cast<float>(args[3].as_number());
    float py = static_cast<float>(args[4].as_number());
    
    float dx = px - cx;
    float dy = py - cy;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    return Value::from_bool(distance < radius);
}

// Check collision between two spheres (3D)
static Value collision_checkSphereSphere(const std::vector<Value>& args) {
    if (args.size() < 7) {
        return Value::from_bool(false);
    }
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float z1 = static_cast<float>(args[2].as_number());
    float r1 = static_cast<float>(args[3].as_number());
    float x2 = static_cast<float>(args[4].as_number());
    float y2 = static_cast<float>(args[5].as_number());
    float z2 = static_cast<float>(args[6].as_number());
    float r2 = static_cast<float>(args[7].as_number());
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    float minDistance = r1 + r2;
    
    return Value::from_bool(distance < minDistance);
}

// Get distance between two 2D points
static Value collision_distance2D(const std::vector<Value>& args) {
    if (args.size() < 4) {
        return Value::from_number(0.0);
    }
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float x2 = static_cast<float>(args[2].as_number());
    float y2 = static_cast<float>(args[3].as_number());
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    return Value::from_number(distance);
}

// Get distance between two 3D points
static Value collision_distance3D(const std::vector<Value>& args) {
    if (args.size() < 6) {
        return Value::from_number(0.0);
    }
    
    float x1 = static_cast<float>(args[0].as_number());
    float y1 = static_cast<float>(args[1].as_number());
    float z1 = static_cast<float>(args[2].as_number());
    float x2 = static_cast<float>(args[3].as_number());
    float y2 = static_cast<float>(args[4].as_number());
    float z2 = static_cast<float>(args[5].as_number());
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    
    return Value::from_number(distance);
}

// Register collision system functions
void register_collision_system(FunctionRegistry& R) {
    R.add("COLLISION_CHECKRECTRECT", NativeFn{"COLLISION_CHECKRECTRECT", 2, collision_checkRectRect});
    R.add("COLLISION_CHECKRECTPOINT", NativeFn{"COLLISION_CHECKRECTPOINT", 3, collision_checkRectPoint});
    R.add("COLLISION_CHECKCIRCLECIRCLE", NativeFn{"COLLISION_CHECKCIRCLECIRCLE", 6, collision_checkCircleCircle});
    R.add("COLLISION_CHECKCIRCLEPOINT", NativeFn{"COLLISION_CHECKCIRCLEPOINT", 4, collision_checkCirclePoint});
    R.add("COLLISION_CHECKSPHERESPHERE", NativeFn{"COLLISION_CHECKSPHERESPHERE", 8, collision_checkSphereSphere});
    R.add("COLLISION_DISTANCE2D", NativeFn{"COLLISION_DISTANCE2D", 4, collision_distance2D});
    R.add("COLLISION_DISTANCE3D", NativeFn{"COLLISION_DISTANCE3D", 6, collision_distance3D});
}

} // namespace bas

