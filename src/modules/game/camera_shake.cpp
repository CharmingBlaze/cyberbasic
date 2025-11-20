#include "bas/camera_shake.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <unordered_map>
#include <cmath>
#include <cstdlib>

namespace bas {

// Camera shake data
struct CameraShakeData {
    float trauma{0.0f};      // Current trauma level (0.0 to 1.0)
    float decay{1.0f};        // Trauma decay rate per second
    float maxOffset{10.0f};   // Maximum shake offset in pixels
    float maxRotation{5.0f};  // Maximum rotation shake in degrees
    float currentOffsetX{0.0f};
    float currentOffsetY{0.0f};
    float currentRotation{0.0f};
    int cameraId{-1};        // Associated camera ID (-1 for global)
};

static std::unordered_map<int, CameraShakeData> g_camera_shakes;
static int g_next_shake_id = 1;
static CameraShakeData g_global_shake;

// Camera.shake(amount, [decay], [maxOffset], [maxRotation])
static Value camera_shake(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    float amount = static_cast<float>(args[0].as_number());
    float decay = args.size() > 1 ? static_cast<float>(args[1].as_number()) : 1.0f;
    float maxOffset = args.size() > 2 ? static_cast<float>(args[2].as_number()) : 10.0f;
    float maxRotation = args.size() > 3 ? static_cast<float>(args[3].as_number()) : 5.0f;
    
    // Add trauma to global shake
    g_global_shake.trauma = std::min(1.0f, g_global_shake.trauma + amount);
    g_global_shake.decay = decay;
    g_global_shake.maxOffset = maxOffset;
    g_global_shake.maxRotation = maxRotation;
    
    return Value::nil();
}

// Camera.setTrauma(amount)
static Value camera_setTrauma(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    float amount = static_cast<float>(args[0].as_number());
    g_global_shake.trauma = std::max(0.0f, std::min(1.0f, amount));
    
    return Value::nil();
}

// Camera.getTrauma() -> float
static Value camera_getTrauma(const std::vector<Value>& args) {
    return Value::from_number(static_cast<double>(g_global_shake.trauma));
}

// Camera.updateShake(deltaTime) - Update shake system
static Value camera_updateShake(const std::vector<Value>& args) {
    double deltaTime = args.size() > 0 ? args[0].as_number() : 0.016;
    
    // Decay trauma
    if (g_global_shake.trauma > 0.0f) {
        g_global_shake.trauma -= static_cast<float>(deltaTime) * g_global_shake.decay;
        if (g_global_shake.trauma < 0.0f) {
            g_global_shake.trauma = 0.0f;
        }
    }
    
    // Calculate shake offset (trauma^2 for smoother falloff)
    float shake = g_global_shake.trauma * g_global_shake.trauma;
    
    if (shake > 0.0f) {
        // Generate random offset
        g_global_shake.currentOffsetX = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * g_global_shake.maxOffset * shake;
        g_global_shake.currentOffsetY = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * g_global_shake.maxOffset * shake;
        g_global_shake.currentRotation = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * g_global_shake.maxRotation * shake;
    } else {
        g_global_shake.currentOffsetX = 0.0f;
        g_global_shake.currentOffsetY = 0.0f;
        g_global_shake.currentRotation = 0.0f;
    }
    
    return Value::nil();
}

// Camera.getShakeOffsetX() -> float
static Value camera_getShakeOffsetX(const std::vector<Value>& args) {
    return Value::from_number(static_cast<double>(g_global_shake.currentOffsetX));
}

// Camera.getShakeOffsetY() -> float
static Value camera_getShakeOffsetY(const std::vector<Value>& args) {
    return Value::from_number(static_cast<double>(g_global_shake.currentOffsetY));
}

// Camera.getShakeRotation() -> float
static Value camera_getShakeRotation(const std::vector<Value>& args) {
    return Value::from_number(static_cast<double>(g_global_shake.currentRotation));
}

void register_camera_shake(FunctionRegistry& registry) {
    registry.add("CAMERA_SHAKE", NativeFn{"CAMERA_SHAKE", 4, camera_shake});
    registry.add("CAMERA_SETTRAUMA", NativeFn{"CAMERA_SETTRAUMA", 1, camera_setTrauma});
    registry.add("CAMERA_GETTRAUMA", NativeFn{"CAMERA_GETTRAUMA", 0, camera_getTrauma});
    registry.add("CAMERA_UPDATESHAKE", NativeFn{"CAMERA_UPDATESHAKE", 1, camera_updateShake});
    registry.add("CAMERA_GETSHAKEOFFSETX", NativeFn{"CAMERA_GETSHAKEOFFSETX", 0, camera_getShakeOffsetX});
    registry.add("CAMERA_GETSHAKEOFFSETY", NativeFn{"CAMERA_GETSHAKEOFFSETY", 0, camera_getShakeOffsetY});
    registry.add("CAMERA_GETSHAKEROTATION", NativeFn{"CAMERA_GETSHAKEROTATION", 0, camera_getShakeRotation});
}

}

