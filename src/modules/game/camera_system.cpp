#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <cmath>

namespace bas {

// Camera storage
struct Camera2DData {
    Vector2 target{0.0f, 0.0f};
    Vector2 offset{0.0f, 0.0f};
    float rotation{0.0f};
    float zoom{1.0f};
};

struct Camera3DData {
    Vector3 position{0.0f, 0.0f, 0.0f};
    Vector3 target{0.0f, 0.0f, 0.0f};
    Vector3 up{0.0f, 1.0f, 0.0f};
    float fovy{60.0f};
    int projection{CAMERA_PERSPECTIVE};
};

static std::unordered_map<int, Camera2DData> g_cameras2d;
static std::unordered_map<int, Camera3DData> g_cameras3d;
static int g_next_camera_id = 1;

// Camera2D constructor: Camera2D([targetX, targetY, offsetX, offsetY, rotation, zoom])
static Value camera2d_constructor(const std::vector<Value>& args) {
    int id = g_next_camera_id++;
    Camera2DData& cam = g_cameras2d[id];
    
    if (args.size() > 0) cam.target.x = static_cast<float>(args[0].as_number());
    if (args.size() > 1) cam.target.y = static_cast<float>(args[1].as_number());
    if (args.size() > 2) cam.offset.x = static_cast<float>(args[2].as_number());
    if (args.size() > 3) cam.offset.y = static_cast<float>(args[3].as_number());
    if (args.size() > 4) cam.rotation = static_cast<float>(args[4].as_number());
    if (args.size() > 5) cam.zoom = static_cast<float>(args[5].as_number());
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Camera2D");
    obj["_id"] = Value::from_int(id);
    obj["targetX"] = Value::from_number(cam.target.x);
    obj["targetY"] = Value::from_number(cam.target.y);
    obj["offsetX"] = Value::from_number(cam.offset.x);
    obj["offsetY"] = Value::from_number(cam.offset.y);
    obj["rotation"] = Value::from_number(cam.rotation);
    obj["zoom"] = Value::from_number(cam.zoom);
    
    return Value::from_map(std::move(obj));
}

// Camera3D constructor: Camera3D([posX, posY, posZ, targetX, targetY, targetZ, upX, upY, upZ, fovy])
static Value camera3d_constructor(const std::vector<Value>& args) {
    int id = g_next_camera_id++;
    Camera3DData& cam = g_cameras3d[id];
    
    if (args.size() > 0) cam.position.x = static_cast<float>(args[0].as_number());
    if (args.size() > 1) cam.position.y = static_cast<float>(args[1].as_number());
    if (args.size() > 2) cam.position.z = static_cast<float>(args[2].as_number());
    if (args.size() > 3) cam.target.x = static_cast<float>(args[3].as_number());
    if (args.size() > 4) cam.target.y = static_cast<float>(args[4].as_number());
    if (args.size() > 5) cam.target.z = static_cast<float>(args[5].as_number());
    if (args.size() > 6) cam.up.x = static_cast<float>(args[6].as_number());
    if (args.size() > 7) cam.up.y = static_cast<float>(args[7].as_number());
    if (args.size() > 8) cam.up.z = static_cast<float>(args[8].as_number());
    if (args.size() > 9) cam.fovy = static_cast<float>(args[9].as_number());
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Camera3D");
    obj["_id"] = Value::from_int(id);
    obj["posX"] = Value::from_number(cam.position.x);
    obj["posY"] = Value::from_number(cam.position.y);
    obj["posZ"] = Value::from_number(cam.position.z);
    obj["targetX"] = Value::from_number(cam.target.x);
    obj["targetY"] = Value::from_number(cam.target.y);
    obj["targetZ"] = Value::from_number(cam.target.z);
    obj["fovy"] = Value::from_number(cam.fovy);
    
    return Value::from_map(std::move(obj));
}

// Camera2D.setTarget(x, y)
static Value camera2d_setTarget(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int id = idIt->second.as_int();
    auto camIt = g_cameras2d.find(id);
    if (camIt != g_cameras2d.end()) {
        camIt->second.target.x = static_cast<float>(args[1].as_number());
        camIt->second.target.y = static_cast<float>(args[2].as_number());
    }
    
    Value::Map updated = map;
    updated["targetX"] = args[1];
    updated["targetY"] = args[2];
    return Value::from_map(std::move(updated));
}

// Camera2D.setZoom(zoom)
static Value camera2d_setZoom(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    float zoom = static_cast<float>(args[1].as_number());
    
    auto idIt = map.find("_id");
    if (idIt != map.end() && idIt->second.is_int()) {
        int id = idIt->second.as_int();
        auto camIt = g_cameras2d.find(id);
        if (camIt != g_cameras2d.end()) {
            camIt->second.zoom = zoom;
        }
    }
    
    Value::Map updated = map;
    updated["zoom"] = args[1];
    return Value::from_map(std::move(updated));
}

// Camera2D.begin() - starts 2D camera mode
static Value camera2d_begin(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = idIt->second.as_int();
    auto camIt = g_cameras2d.find(id);
    if (camIt == g_cameras2d.end()) {
        return Value::nil();
    }
    
    Camera2DData& cam = camIt->second;
    Camera2D raylibCam;
    raylibCam.target = cam.target;
    raylibCam.offset = cam.offset;
    raylibCam.rotation = cam.rotation;
    raylibCam.zoom = cam.zoom;
    
    BeginMode2D(raylibCam);
    
    return Value::nil();
}

// Camera2D.end() - ends 2D camera mode
static Value camera2d_end(const std::vector<Value>& args) {
    (void)args;
    EndMode2D();
    return Value::nil();
}

// Camera3D.setPosition(x, y, z)
static Value camera3d_setPosition(const std::vector<Value>& args) {
    if (args.size() < 4 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int id = idIt->second.as_int();
    auto camIt = g_cameras3d.find(id);
    if (camIt != g_cameras3d.end()) {
        camIt->second.position.x = static_cast<float>(args[1].as_number());
        camIt->second.position.y = static_cast<float>(args[2].as_number());
        camIt->second.position.z = static_cast<float>(args[3].as_number());
    }
    
    Value::Map updated = map;
    updated["posX"] = args[1];
    updated["posY"] = args[2];
    updated["posZ"] = args[3];
    return Value::from_map(std::move(updated));
}

// Camera3D.setTarget(x, y, z)
static Value camera3d_setTarget(const std::vector<Value>& args) {
    if (args.size() < 4 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int id = idIt->second.as_int();
    auto camIt = g_cameras3d.find(id);
    if (camIt != g_cameras3d.end()) {
        camIt->second.target.x = static_cast<float>(args[1].as_number());
        camIt->second.target.y = static_cast<float>(args[2].as_number());
        camIt->second.target.z = static_cast<float>(args[3].as_number());
    }
    
    Value::Map updated = map;
    updated["targetX"] = args[1];
    updated["targetY"] = args[2];
    updated["targetZ"] = args[3];
    return Value::from_map(std::move(updated));
}

// Camera3D.begin() - starts 3D camera mode
static Value camera3d_begin(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int id = idIt->second.as_int();
    auto camIt = g_cameras3d.find(id);
    if (camIt == g_cameras3d.end()) {
        return Value::nil();
    }
    
    Camera3DData& cam = camIt->second;
    Camera3D raylibCam;
    raylibCam.position = cam.position;
    raylibCam.target = cam.target;
    raylibCam.up = cam.up;
    raylibCam.fovy = cam.fovy;
    raylibCam.projection = cam.projection;
    
    BeginMode3D(raylibCam);
    
    return Value::nil();
}

// Camera3D.end() - ends 3D camera mode
static Value camera3d_end(const std::vector<Value>& args) {
    (void)args;
    EndMode3D();
    return Value::nil();
}

// Register camera system functions
void register_camera_system(FunctionRegistry& R) {
    R.add("CAMERA2D", NativeFn{"CAMERA2D", -1, camera2d_constructor});
    R.add("CAMERA2D_SETTARGET", NativeFn{"CAMERA2D_SETTARGET", 3, camera2d_setTarget});
    R.add("CAMERA2D_SETZOOM", NativeFn{"CAMERA2D_SETZOOM", 2, camera2d_setZoom});
    R.add("CAMERA2D_BEGIN", NativeFn{"CAMERA2D_BEGIN", 1, camera2d_begin});
    R.add("CAMERA2D_END", NativeFn{"CAMERA2D_END", 0, camera2d_end});
    R.add("CAMERA3D", NativeFn{"CAMERA3D", -1, camera3d_constructor});
    R.add("CAMERA3D_SETPOSITION", NativeFn{"CAMERA3D_SETPOSITION", 4, camera3d_setPosition});
    R.add("CAMERA3D_SETTARGET", NativeFn{"CAMERA3D_SETTARGET", 4, camera3d_setTarget});
    R.add("CAMERA3D_BEGIN", NativeFn{"CAMERA3D_BEGIN", 1, camera3d_begin});
    R.add("CAMERA3D_END", NativeFn{"CAMERA3D_END", 0, camera3d_end});
}

} // namespace bas

