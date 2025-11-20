#include "bas/post_processing.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <vector>
#include <string>
#include <unordered_map>

namespace bas {

// Post-processing effect data
struct PostFXEffect {
    std::string name;
    Shader shader;
    RenderTexture2D target;
    bool isActive;
    std::unordered_map<std::string, int> uniformLocations;
};

static std::vector<PostFXEffect> g_postfx_effects;
static RenderTexture2D g_postfx_target{0};
static bool g_postfx_initialized = false;

// Initialize post-processing system
static void init_postfx() {
    if (g_postfx_initialized) return;
    
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    if (width == 0) width = 800;
    if (height == 0) height = 600;
    
    g_postfx_target = LoadRenderTexture(width, height);
    g_postfx_initialized = true;
}

// PostFX.add(effectName, [shaderPath]) -> bool
static Value postfx_add(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    init_postfx();
    
    std::string effectName = args[0].as_string();
    std::string shaderPath = args.size() > 1 ? args[1].as_string() : "";
    
    PostFXEffect effect;
    effect.name = effectName;
    effect.isActive = true;
    
    // Load shader if path provided
    if (!shaderPath.empty()) {
        effect.shader = LoadShader(nullptr, shaderPath.c_str());
        if (effect.shader.id == 0) {
            // Create default shader for common effects
            if (effectName == "blur") {
                // Would create blur shader here
            } else if (effectName == "grayscale") {
                // Would create grayscale shader here
            } else if (effectName == "vignette") {
                // Would create vignette shader here
            }
        }
    }
    
    g_postfx_effects.push_back(effect);
    return Value::from_bool(true);
}

// PostFX.remove(effectName)
static Value postfx_remove(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::nil();
    }
    
    std::string effectName = args[0].as_string();
    
    for (auto it = g_postfx_effects.begin(); it != g_postfx_effects.end(); ++it) {
        if (it->name == effectName) {
            if (it->shader.id != 0) {
                UnloadShader(it->shader);
            }
            g_postfx_effects.erase(it);
            break;
        }
    }
    
    return Value::nil();
}

// PostFX.clear()
static Value postfx_clear(const std::vector<Value>& args) {
    (void)args;
    
    for (auto& effect : g_postfx_effects) {
        if (effect.shader.id != 0) {
            UnloadShader(effect.shader);
        }
    }
    g_postfx_effects.clear();
    
    return Value::nil();
}

// PostFX.begin() - Begin post-processing render
static Value postfx_begin(const std::vector<Value>& args) {
    (void)args;
    
    if (!g_postfx_initialized || g_postfx_effects.empty()) {
        return Value::nil();
    }
    
    BeginTextureMode(g_postfx_target);
    return Value::nil();
}

// PostFX.end() - End post-processing and apply effects
static Value postfx_end(const std::vector<Value>& args) {
    (void)args;
    
    if (!g_postfx_initialized || g_postfx_effects.empty()) {
        return Value::nil();
    }
    
    EndTextureMode();
    
    // Apply effects
    for (const auto& effect : g_postfx_effects) {
        if (!effect.isActive || effect.shader.id == 0) continue;
        
        BeginShaderMode(effect.shader);
        DrawTextureRec(g_postfx_target.texture, 
                      Rectangle{0, 0, static_cast<float>(g_postfx_target.texture.width), 
                               static_cast<float>(-g_postfx_target.texture.height)},
                      Vector2{0, 0}, WHITE);
        EndShaderMode();
    }
    
    return Value::nil();
}

// PostFX.setActive(effectName, active)
static Value postfx_setActive(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    std::string effectName = args[0].as_string();
    bool active = args[1].as_bool();
    
    for (auto& effect : g_postfx_effects) {
        if (effect.name == effectName) {
            effect.isActive = active;
            break;
        }
    }
    
    return Value::nil();
}

// PostFX.isActive(effectName) -> bool
static Value postfx_isActive(const std::vector<Value>& args) {
    if (args.size() < 1) {
        return Value::from_bool(false);
    }
    
    std::string effectName = args[0].as_string();
    
    for (const auto& effect : g_postfx_effects) {
        if (effect.name == effectName) {
            return Value::from_bool(effect.isActive);
        }
    }
    
    return Value::from_bool(false);
}

void register_post_processing(FunctionRegistry& registry) {
    registry.add("POSTFX_ADD", NativeFn{"POSTFX_ADD", 2, postfx_add});
    registry.add("POSTFX_REMOVE", NativeFn{"POSTFX_REMOVE", 1, postfx_remove});
    registry.add("POSTFX_CLEAR", NativeFn{"POSTFX_CLEAR", 0, postfx_clear});
    registry.add("POSTFX_BEGIN", NativeFn{"POSTFX_BEGIN", 0, postfx_begin});
    registry.add("POSTFX_END", NativeFn{"POSTFX_END", 0, postfx_end});
    registry.add("POSTFX_SETACTIVE", NativeFn{"POSTFX_SETACTIVE", 2, postfx_setActive});
    registry.add("POSTFX_ISACTIVE", NativeFn{"POSTFX_ISACTIVE", 1, postfx_isActive});
}

}

