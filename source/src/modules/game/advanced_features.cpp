#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/advanced_features.hpp"
#include "bas/networking.hpp"
#include "bas/ecs_system.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <thread>
#include <future>

namespace bas {

// Forward declarations
extern std::unique_ptr<NetworkingSystem> g_networking_system;

// ===== MACRO SYSTEM =====

struct MacroData {
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::string> body; // Macro body as code lines
};

static std::unordered_map<std::string, MacroData> g_macros;

// MACRO name(params) ... END MACRO
static Value macro_declare(const std::vector<Value>& args) {
    // This would be called from parser when MACRO block is parsed
    // For now, return nil
    return Value::nil();
}

// Expand macro: MACRO_NAME(arg1, arg2, ...)
static Value macro_expand(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string macroName = args[0].as_string();
    auto macroIt = g_macros.find(macroName);
    if (macroIt == g_macros.end()) {
        return Value::nil();
    }
    
    // Macro expansion would happen at parse time
    // This is a placeholder
    return Value::nil();
}

// ===== INLINE VECTOR OPERATIONS =====

// VEC3(x, y, z) -> Vector3
static Value vec3_inline(const std::vector<Value>& args) {
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

// DOT(vec1, vec2) -> float
static Value dot_inline(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_number(0.0);
    }
    
    // Extract vector components
    double x1 = 0.0, y1 = 0.0, z1 = 0.0;
    double x2 = 0.0, y2 = 0.0, z2 = 0.0;
    
    if (args[0].is_map()) {
        const auto& map1 = args[0].as_map();
        x1 = map1.count("x") ? map1.at("x").as_number() : 0.0;
        y1 = map1.count("y") ? map1.at("y").as_number() : 0.0;
        z1 = map1.count("z") ? map1.at("z").as_number() : 0.0;
    }
    
    if (args[1].is_map()) {
        const auto& map2 = args[1].as_map();
        x2 = map2.count("x") ? map2.at("x").as_number() : 0.0;
        y2 = map2.count("y") ? map2.at("y").as_number() : 0.0;
        z2 = map2.count("z") ? map2.at("z").as_number() : 0.0;
    }
    
    return Value::from_number(x1*x2 + y1*y2 + z1*z2);
}

// CROSS(vec1, vec2) -> Vector3
static Value cross_inline(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return vec3_inline({});
    }
    
    // Extract vector components
    double x1 = 0.0, y1 = 0.0, z1 = 0.0;
    double x2 = 0.0, y2 = 0.0, z2 = 0.0;
    
    if (args[0].is_map()) {
        const auto& map1 = args[0].as_map();
        x1 = map1.count("x") ? map1.at("x").as_number() : 0.0;
        y1 = map1.count("y") ? map1.at("y").as_number() : 0.0;
        z1 = map1.count("z") ? map1.at("z").as_number() : 0.0;
    }
    
    if (args[1].is_map()) {
        const auto& map2 = args[1].as_map();
        x2 = map2.count("x") ? map2.at("x").as_number() : 0.0;
        y2 = map2.count("y") ? map2.at("y").as_number() : 0.0;
        z2 = map2.count("z") ? map2.at("z").as_number() : 0.0;
    }
    
    // Cross product
    double x = y1*z2 - z1*y2;
    double y = z1*x2 - x1*z2;
    double z = x1*y2 - y1*x2;
    
    return vec3_inline({Value::from_number(x), Value::from_number(y), Value::from_number(z)});
}

// LERP(a, b, t) -> value or Vector3
static Value lerp_inline(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::from_number(0.0);
    }
    
    double t = args[2].as_number();
    t = std::max(0.0, std::min(1.0, t)); // Clamp 0-1
    
    // If both are numbers, lerp numbers
    if (args[0].is_number() && args[1].is_number()) {
        double a = args[0].as_number();
        double b = args[1].as_number();
        return Value::from_number(a + (b - a) * t);
    }
    
    // If both are vectors, lerp vectors
    if (args[0].is_map() && args[1].is_map()) {
        const auto& map1 = args[0].as_map();
        const auto& map2 = args[1].as_map();
        
        double x1 = map1.count("x") ? map1.at("x").as_number() : 0.0;
        double y1 = map1.count("y") ? map1.at("y").as_number() : 0.0;
        double z1 = map1.count("z") ? map1.at("z").as_number() : 0.0;
        
        double x2 = map2.count("x") ? map2.at("x").as_number() : 0.0;
        double y2 = map2.count("y") ? map2.at("y").as_number() : 0.0;
        double z2 = map2.count("z") ? map2.at("z").as_number() : 0.0;
        
        double x = x1 + (x2 - x1) * t;
        double y = y1 + (y2 - y1) * t;
        double z = z1 + (z2 - z1) * t;
        
        return vec3_inline({Value::from_number(x), Value::from_number(y), Value::from_number(z)});
    }
    
    return Value::from_number(0.0);
}

// ===== SCENE MANAGEMENT =====

static std::unordered_map<std::string, int> g_scene_name_to_id; // name -> id
static int g_active_scene_id = 0;

// LOAD SCENE "name"
static Value load_scene(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string sceneName = args[0].as_string();
    
    // Find scene by name
    auto nameIt = g_scene_name_to_id.find(sceneName);
    if (nameIt != g_scene_name_to_id.end()) {
        g_active_scene_id = nameIt->second;
        return Value::from_int(nameIt->second);
    }
    
    // Create new scene if doesn't exist
    // Would call scene_constructor here
    int newId = static_cast<int>(g_scenes.size() + 1);
    g_scene_name_to_id[sceneName] = newId;
    g_active_scene_id = newId;
    
    return Value::from_int(newId);
}

// UNLOAD SCENE "name"
static Value unload_scene(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string sceneName = args[0].as_string();
    
    auto nameIt = g_scene_name_to_id.find(sceneName);
    if (nameIt != g_scene_name_to_id.end()) {
        int sceneId = nameIt->second;
        auto sceneIt = g_scenes.find(sceneId);
        if (sceneIt != g_scenes.end()) {
            // Clear entities
            sceneIt->second.entities.clear();
            // Would destroy scene here
        }
        g_scene_name_to_id.erase(nameIt);
    }
    
    return Value::nil();
}

// ===== CAMERA CONTROL =====

static std::unordered_map<int, int> g_camera_follow_targets; // cameraId -> entityId

// SET CAMERA FOLLOW entity
static Value set_camera_follow(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    int cameraId = 0;
    int entityId = 0;
    
    // Get camera ID
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            cameraId = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        cameraId = args[0].as_int();
    }
    
    // Get entity ID
    if (args[1].is_map()) {
        const auto& map = args[1].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entityId = idIt->second.as_int();
        }
    } else if (args[1].is_int()) {
        entityId = args[1].as_int();
    }
    
    if (cameraId > 0 && entityId > 0) {
        g_camera_follow_targets[cameraId] = entityId;
    }
    
    return Value::nil();
}

// SHAKE CAMERA [camera], INTENSITY=amount
static Value shake_camera(const std::vector<Value>& args) {
    double intensity = 0.5;
    int cameraId = -1; // -1 for global
    
    if (!args.empty()) {
        if (args[0].is_number()) {
            intensity = args[0].as_number();
        } else if (args[0].is_map()) {
            const auto& map = args[0].as_map();
            auto idIt = map.find("_id");
            if (idIt != map.end() && idIt->second.is_int()) {
                cameraId = idIt->second.as_int();
            }
            if (args.size() > 1) {
                intensity = args[1].as_number();
            }
        }
    }
    
    // Call camera shake system
    // Would call: Camera.shake(intensity)
    
    return Value::nil();
}

// ===== PARTICLE SYSTEMS =====

struct ParticleSystem {
    std::string name;
    int particleCount{100};
    double lifetime{1.0};
    double spawnRate{10.0};
    double x{0.0}, y{0.0}, z{0.0};
    bool active{false};
};

static std::unordered_map<std::string, ParticleSystem> g_particle_systems;
static std::unordered_map<int, ParticleSystem> g_active_particles; // id -> system

// SPAWN PARTICLES "name" AT (x, y, [z])
static Value spawn_particles(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string name = args[0].as_string();
    double x = args[1].as_number();
    double y = args[2].as_number();
    double z = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Find or create particle system
    auto it = g_particle_systems.find(name);
    if (it == g_particle_systems.end()) {
        // Create default particle system
        ParticleSystem ps;
        ps.name = name;
        g_particle_systems[name] = ps;
        it = g_particle_systems.find(name);
    }
    
    ParticleSystem& ps = it->second;
    ps.x = x;
    ps.y = y;
    ps.z = z;
    ps.active = true;
    
    int id = static_cast<int>(g_active_particles.size() + 1);
    g_active_particles[id] = ps;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("ParticleSystem");
    obj["_id"] = Value::from_int(id);
    obj["name"] = Value::from_string(name);
    obj["x"] = Value::from_number(x);
    obj["y"] = Value::from_number(y);
    obj["z"] = Value::from_number(z);
    
    return Value::from_map(std::move(obj));
}

// ===== DIALOGUE SYSTEM =====

struct Dialogue {
    std::string npcName;
    std::string text;
    std::vector<std::string> choices;
    int selectedChoice{0};
};

static std::unordered_map<std::string, Dialogue> g_dialogues;
static std::string g_active_dialogue;

// DIALOGUE "NPC" SAYS "text"
static Value dialogue_says(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string() || !args[1].is_string()) {
        return Value::nil();
    }
    
    std::string npcName = args[0].as_string();
    std::string text = args[1].as_string();
    
    Dialogue& dialogue = g_dialogues[npcName];
    dialogue.npcName = npcName;
    dialogue.text = text;
    g_active_dialogue = npcName;
    
    // Would display dialogue UI here
    
    return Value::nil();
}

// DIALOGUE "NPC" CHOICE "option1", "option2", ...
static Value dialogue_choice(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string npcName = args[0].as_string();
    Dialogue& dialogue = g_dialogues[npcName];
    
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i].is_string()) {
            dialogue.choices.push_back(args[i].as_string());
        }
    }
    
    return Value::nil();
}

// ===== AI HOOKS =====

// AI PATHFIND entity TO (x, y, [z])
static Value ai_pathfind(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::nil();
    }
    
    int entityId = 0;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entityId = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        entityId = args[0].as_int();
    }
    
    if (entityId == 0) {
        return Value::nil();
    }
    
    double x = args[1].as_number();
    double y = args[2].as_number();
    double z = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Would call navigation system to pathfind
    // For now, return success
    return Value::from_bool(true);
}

// AI BEHAVIOR entity, "behaviorName"
static Value ai_behavior(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::nil();
    }
    
    int entityId = 0;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entityId = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        entityId = args[0].as_int();
    }
    
    if (entityId == 0 || !args[1].is_string()) {
        return Value::nil();
    }
    
    std::string behaviorName = args[1].as_string();
    
    // Would set AI behavior for entity
    // For now, return success
    return Value::from_bool(true);
}

// ===== CLOUD SAVE =====

// SAVE GAME TO CLOUD [slot]
static Value save_game_to_cloud(const std::vector<Value>& args) {
    std::string slot = args.empty() ? "default" : args[0].as_string();
    
    // Would serialize game state and upload to cloud
    // For now, return success
    return Value::from_bool(true);
}

// LOAD GAME FROM CLOUD [slot]
static Value load_game_from_cloud(const std::vector<Value>& args) {
    std::string slot = args.empty() ? "default" : args[0].as_string();
    
    // Would download from cloud and deserialize game state
    // For now, return success
    return Value::from_bool(true);
}

// ===== WEB APIs =====

// FETCH "url" INTO variable
static Value fetch_url(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string url = args[0].as_string();
    
    // Would make HTTP request
    // For now, return placeholder
    Value::Map response;
    response["status"] = Value::from_int(200);
    response["data"] = Value::from_string("");
    response["error"] = Value::from_string("");
    
    return Value::from_map(std::move(response));
}

// ===== MULTIPLAYER SYNC =====

// SYNC PLAYER POSITION player, x, y, [z]
static Value sync_player_position(const std::vector<Value>& args) {
    if (args.size() < 3 || !g_networking_system) {
        return Value::nil();
    }
    
    int playerId = 0;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            playerId = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        playerId = args[0].as_int();
    }
    
    if (playerId == 0) {
        return Value::nil();
    }
    
    double x = args[1].as_number();
    double y = args[2].as_number();
    double z = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Create sync message
    std::ostringstream oss;
    oss << "POS:" << x << "," << y << "," << z;
    
    // Would use networking system to broadcast
    // For now, just return success
    
    return Value::nil();
}

// BROADCAST EVENT "eventName", [data]
static Value broadcast_event(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string() || !g_networking_system) {
        return Value::nil();
    }
    
    std::string eventName = args[0].as_string();
    std::string data = args.size() > 1 ? args[1].as_string() : "";
    
    // Would use networking system to broadcast event
    // For now, just return success
    
    return Value::nil();
}

// ===== MEMORY-SAFE SANDBOXING =====

struct SandboxConfig {
    int maxLoopIterations{10000};
    int maxExecutionTime{5000}; // milliseconds
    bool allowFileAccess{true};
    std::vector<std::string> allowedFilePaths;
    bool allowNetworkAccess{false};
};

static SandboxConfig g_sandbox_config;
static std::unordered_map<int, std::chrono::steady_clock::time_point> g_execution_starts;
static int g_next_sandbox_id = 1;

// ENABLE SANDBOX [config]
static Value enable_sandbox(const std::vector<Value>& args) {
    // Would configure sandbox settings
    // For now, just return success
    return Value::from_bool(true);
}

// CHECK SANDBOX [id] -> bool (check if execution should continue)
static Value check_sandbox(const std::vector<Value>& args) {
    int sandboxId = static_cast<int>(args.empty() ? 0 : args[0].as_int());
    
    // Would check execution time, loop counts, etc.
    // For now, always return true
    return Value::from_bool(true);
}

// Register all game advanced features (macros, particles, dialogue, etc.)
void register_game_advanced_features(FunctionRegistry& R) {
    // Macros
    R.add("MACRO_DECLARE", NativeFn{"MACRO_DECLARE", -1, macro_declare});
    R.add("MACRO_EXPAND", NativeFn{"MACRO_EXPAND", -1, macro_expand});
    
    // Inline vector operations
    R.add("VEC3", NativeFn{"VEC3", -1, vec3_inline});
    R.add("DOT", NativeFn{"DOT", 2, dot_inline});
    R.add("CROSS", NativeFn{"CROSS", 2, cross_inline});
    R.add_with_policy("LERP", NativeFn{"LERP", 3, lerp_inline}, true);
    
    // Scene management
    R.add("LOAD_SCENE", NativeFn{"LOAD_SCENE", 1, load_scene});
    R.add("UNLOAD_SCENE", NativeFn{"UNLOAD_SCENE", 1, unload_scene});
    
    // Camera control
    R.add("SET_CAMERA_FOLLOW", NativeFn{"SET_CAMERA_FOLLOW", 2, set_camera_follow});
    R.add("SHAKE_CAMERA", NativeFn{"SHAKE_CAMERA", -1, shake_camera});
    
    // Particle systems
    R.add("SPAWN_PARTICLES", NativeFn{"SPAWN_PARTICLES", -1, spawn_particles});
    
    // Dialogue system
    R.add("DIALOGUE_SAYS", NativeFn{"DIALOGUE_SAYS", 2, dialogue_says});
    R.add("DIALOGUE_CHOICE", NativeFn{"DIALOGUE_CHOICE", -1, dialogue_choice});
    
    // AI hooks
    R.add("AI_PATHFIND", NativeFn{"AI_PATHFIND", -1, ai_pathfind});
    R.add("AI_BEHAVIOR", NativeFn{"AI_BEHAVIOR", 2, ai_behavior});
    
    // Cloud save
    R.add("SAVE_GAME_TO_CLOUD", NativeFn{"SAVE_GAME_TO_CLOUD", 1, save_game_to_cloud});
    R.add("LOAD_GAME_FROM_CLOUD", NativeFn{"LOAD_GAME_FROM_CLOUD", 1, load_game_from_cloud});
    
    // Web APIs
    R.add("FETCH", NativeFn{"FETCH", 1, fetch_url});
    
    // Multiplayer sync
    R.add("SYNC_PLAYER_POSITION", NativeFn{"SYNC_PLAYER_POSITION", -1, sync_player_position});
    R.add("BROADCAST_EVENT", NativeFn{"BROADCAST_EVENT", -1, broadcast_event});
    
    // Sandboxing
    R.add("ENABLE_SANDBOX", NativeFn{"ENABLE_SANDBOX", 1, enable_sandbox});
    R.add("CHECK_SANDBOX", NativeFn{"CHECK_SANDBOX", 1, check_sandbox});
}

} // namespace bas

