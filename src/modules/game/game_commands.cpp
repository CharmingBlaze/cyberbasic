#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include "bas/game_commands.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>

namespace bas {

// Forward declarations for entity system
extern std::unordered_map<int, struct EntityData> g_entities;
extern std::unordered_map<int, struct SceneData> g_scenes;

// Forward declarations for animation system
extern std::unordered_map<int, struct AnimationData> g_animations;

// Forward declarations for audio system
extern std::unique_ptr<struct AudioSystem> g_audio_system;

// Forward declarations for physics system
extern std::unique_ptr<struct PhysicsWorld> g_physics_world;

// ===== ENTITY COMMANDS =====

// SPAWN entityType AT (x, y, [z])
static Value spawn_entity(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::nil();
    }
    
    std::string entityType = args[0].as_string();
    double x = args[1].as_number();
    double y = args[2].as_number();
    double z = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Find or create default scene
    int sceneId = 1; // Default scene
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        // Create default scene
        Value::Map sceneObj;
        sceneObj["_type"] = Value::from_string("Scene");
        sceneObj["_id"] = Value::from_int(sceneId);
        sceneObj["name"] = Value::from_string("DefaultScene");
        // Would need to call scene_constructor here
    }
    
    // Create entity using ECS system
    // This would call scene.createEntity() and entity.setPosition()
    // For now, return a placeholder
    Value::Map entityObj;
    entityObj["_type"] = Value::from_string("Entity");
    entityObj["name"] = Value::from_string(entityType);
    entityObj["x"] = Value::from_number(x);
    entityObj["y"] = Value::from_number(y);
    entityObj["z"] = Value::from_number(z);
    
    return Value::from_map(std::move(entityObj));
}

// DESTROY entity
static Value destroy_entity(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int entityId = idIt->second.as_int();
    // Would call scene.destroyEntity() here
    return Value::nil();
}

// MOVE entity TO (x, y, [z])
static Value move_entity(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    double x = args[1].as_number();
    double y = args[2].as_number();
    double z = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Would call entity.setPosition(x, y, z) here
    Value::Map updated = map;
    updated["x"] = Value::from_number(x);
    updated["y"] = Value::from_number(y);
    updated["z"] = Value::from_number(z);
    
    return Value::from_map(std::move(updated));
}

// ===== ANIMATION COMMANDS =====

// PLAY ANIMATION "name"
static Value play_animation(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string animName = args[0].as_string();
    
    // Find animation by name
    for (auto& pair : g_animations) {
        if (pair.second.name == animName) {
            pair.second.isPlaying = true;
            pair.second.currentTime = 0.0;
            return Value::from_bool(true);
        }
    }
    
    return Value::from_bool(false);
}

// STOP ANIMATION "name"
static Value stop_animation(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string animName = args[0].as_string();
    
    // Find animation by name
    for (auto& pair : g_animations) {
        if (pair.second.name == animName) {
            pair.second.isPlaying = false;
            pair.second.currentTime = 0.0;
            return Value::from_bool(true);
        }
    }
    
    return Value::from_bool(false);
}

// BLEND ANIMATION "name", TIME=blendTime
static Value blend_animation(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string animName = args[0].as_string();
    double blendTime = args.size() > 1 ? args[1].as_number() : 0.2;
    
    // Find animation and set blend time
    for (auto& pair : g_animations) {
        if (pair.second.name == animName) {
            // Store blend time (would be used in animation system)
            return Value::from_bool(true);
        }
    }
    
    return Value::from_bool(false);
}

// ===== INPUT COMMANDS =====

// Helper to convert key name to key code
static int get_key_code(const std::string& keyName) {
    static const std::unordered_map<std::string, int> keyMap = {
        {"SPACE", KEY_SPACE}, {"ENTER", KEY_ENTER}, {"ESCAPE", KEY_ESCAPE},
        {"UP", KEY_UP}, {"DOWN", KEY_DOWN}, {"LEFT", KEY_LEFT}, {"RIGHT", KEY_RIGHT},
        {"W", KEY_W}, {"A", KEY_A}, {"S", KEY_S}, {"D", KEY_D},
        {"J", KEY_J}, {"K", KEY_K}, {"L", KEY_L},
        {"SHIFT", KEY_LEFT_SHIFT}, {"CTRL", KEY_LEFT_CONTROL}, {"ALT", KEY_LEFT_ALT}
    };
    
    auto it = keyMap.find(keyName);
    return (it != keyMap.end()) ? it->second : 0;
}

// INPUT("keyName") - Returns true if key is down
static Value input_check(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::from_bool(false);
    }
    
    std::string keyName = args[0].as_string();
    int keyCode = get_key_code(keyName);
    
    if (keyCode == 0) {
        return Value::from_bool(false);
    }
    
    return Value::from_bool(IsKeyDown(keyCode));
}

// MOUSECLICK() - Returns true if mouse button was just clicked
static Value mouse_click(const std::vector<Value>& args) {
    (void)args;
    return Value::from_bool(IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
}

// MOUSEDOWN() - Returns true if mouse button is down
static Value mouse_down(const std::vector<Value>& args) {
    (void)args;
    return Value::from_bool(IsMouseButtonDown(MOUSE_BUTTON_LEFT));
}

// MOUSEX() - Returns mouse X position
static Value mouse_x(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetMouseX());
}

// MOUSEY() - Returns mouse Y position
static Value mouse_y(const std::vector<Value>& args) {
    (void)args;
    return Value::from_int(GetMouseY());
}

// ===== PHYSICS COMMANDS =====

// APPLY FORCE entity, x, y, [z]
static Value apply_force(const std::vector<Value>& args) {
    if (args.size() < 3) {
        return Value::nil();
    }
    
    // Get entity ID
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
    
    if (entityId == 0 || !g_physics_world) {
        return Value::nil();
    }
    
    double fx = args[1].as_number();
    double fy = args[2].as_number();
    double fz = args.size() > 3 ? args[3].as_number() : 0.0;
    
    // Get entity's physics body ID (would be stored in component)
    // For now, assume entityId maps to bodyId
    int bodyId = entityId;
    
    // Apply force using physics world
    // Would call: g_physics_world->apply_force(bodyId, fx, fy, fz);
    
    return Value::nil();
}

// SET GRAVITY x, y, [z]
static Value set_gravity(const std::vector<Value>& args) {
    if (args.size() < 2 || !g_physics_world) {
        return Value::nil();
    }
    
    double gx = args[0].as_number();
    double gy = args[1].as_number();
    double gz = args.size() > 2 ? args[2].as_number() : 0.0;
    
    // Set gravity using physics world
    // Would call: g_physics_world->set_gravity(gx, gy, gz);
    
    return Value::nil();
}

// COLLIDE WITH entity1, entity2 -> bool
static Value collide_with(const std::vector<Value>& args) {
    if (args.size() < 2) {
        return Value::from_bool(false);
    }
    
    // Get entity IDs
    int entity1Id = 0, entity2Id = 0;
    
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entity1Id = idIt->second.as_int();
        }
    } else if (args[0].is_int()) {
        entity1Id = args[0].as_int();
    }
    
    if (args[1].is_map()) {
        const auto& map = args[1].as_map();
        auto idIt = map.find("_id");
        if (idIt != map.end() && idIt->second.is_int()) {
            entity2Id = idIt->second.as_int();
        }
    } else if (args[1].is_int()) {
        entity2Id = args[1].as_int();
    }
    
    if (entity1Id == 0 || entity2Id == 0 || !g_physics_world) {
        return Value::from_bool(false);
    }
    
    // Check collision using physics world
    // Would call: g_physics_world->check_collision(entity1Id, entity2Id);
    
    return Value::from_bool(false);
}

// ===== SOUND & MUSIC COMMANDS =====

// Sound storage
static std::unordered_map<std::string, int> g_sound_cache; // name -> soundId
static std::unordered_map<std::string, int> g_music_cache; // name -> musicId
static int g_current_music_id = 0;

// PLAY SOUND "file.wav"
static Value play_sound(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string filePath = args[0].as_string();
    
    // Check cache
    auto it = g_sound_cache.find(filePath);
    int soundId = 0;
    
    if (it != g_sound_cache.end()) {
        soundId = it->second;
    } else {
        // Load sound (would use Raylib LoadSound)
        // For now, create a placeholder ID
        soundId = static_cast<int>(g_sound_cache.size() + 1);
        g_sound_cache[filePath] = soundId;
    }
    
    // Play sound (would use Raylib PlaySound)
    // PlaySound(soundId);
    
    return Value::from_int(soundId);
}

// LOOP MUSIC "file.mp3"
static Value loop_music(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_string()) {
        return Value::nil();
    }
    
    std::string filePath = args[0].as_string();
    
    // Check cache
    auto it = g_music_cache.find(filePath);
    int musicId = 0;
    
    if (it != g_music_cache.end()) {
        musicId = it->second;
    } else {
        // Load music (would use Raylib LoadMusicStream)
        musicId = static_cast<int>(g_music_cache.size() + 1);
        g_music_cache[filePath] = musicId;
    }
    
    // Play music in loop (would use Raylib PlayMusicStream, SetMusicLoop)
    // PlayMusicStream(musicId);
    // SetMusicLoop(musicId, true);
    g_current_music_id = musicId;
    
    return Value::from_int(musicId);
}

// SET VOLUME volume, [type] - type: "MASTER", "MUSIC", "SFX"
static Value set_volume(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    
    double volume = args[0].as_number();
    volume = std::max(0.0, std::min(1.0, volume)); // Clamp 0-1
    
    std::string type = args.size() > 1 ? args[1].as_string() : "MASTER";
    
    if (type == "MASTER" || type == "master") {
        // Set master volume
        if (g_audio_system) {
            // g_audio_system->set_master_volume(volume);
        }
    } else if (type == "MUSIC" || type == "music") {
        // Set music volume
        if (g_audio_system) {
            // g_audio_system->set_music_volume(volume);
        }
    } else if (type == "SFX" || type == "sfx" || type == "SOUND") {
        // Set sound effects volume
        if (g_audio_system) {
            // g_audio_system->set_sfx_volume(volume);
        }
    }
    
    return Value::nil();
}

// STOP SOUND [soundId]
static Value stop_sound(const std::vector<Value>& args) {
    if (args.empty()) {
        // Stop all sounds
        // Would iterate through all playing sounds and stop them
        return Value::nil();
    }
    
    int soundId = args[0].as_int();
    // Stop specific sound (would use Raylib StopSound)
    // StopSound(soundId);
    
    return Value::nil();
}

// STOP MUSIC
static Value stop_music(const std::vector<Value>& args) {
    (void)args;
    
    if (g_current_music_id > 0) {
        // Stop current music (would use Raylib StopMusicStream)
        // StopMusicStream(g_current_music_id);
        g_current_music_id = 0;
    }
    
    return Value::nil();
}

// Register all game commands
void register_game_commands(FunctionRegistry& R) {
    // Entity commands
    R.add("SPAWN", NativeFn{"SPAWN", -1, spawn_entity});
    R.add("DESTROY", NativeFn{"DESTROY", 1, destroy_entity});
    R.add("MOVE", NativeFn{"MOVE", -1, move_entity});
    
    // Animation commands
    R.add("PLAY_ANIMATION", NativeFn{"PLAY_ANIMATION", 1, play_animation});
    R.add("STOP_ANIMATION", NativeFn{"STOP_ANIMATION", 1, stop_animation});
    R.add("BLEND_ANIMATION", NativeFn{"BLEND_ANIMATION", 2, blend_animation});
    
    // Input commands
    R.add("INPUT", NativeFn{"INPUT", 1, input_check});
    R.add("MOUSECLICK", NativeFn{"MOUSECLICK", 0, mouse_click});
    R.add("MOUSEDOWN", NativeFn{"MOUSEDOWN", 0, mouse_down});
    R.add("MOUSEX", NativeFn{"MOUSEX", 0, mouse_x});
    R.add("MOUSEY", NativeFn{"MOUSEY", 0, mouse_y});
    
    // Physics commands
    R.add("APPLY_FORCE", NativeFn{"APPLY_FORCE", -1, apply_force});
    R.add("SET_GRAVITY", NativeFn{"SET_GRAVITY", -1, set_gravity});
    R.add("COLLIDE_WITH", NativeFn{"COLLIDE_WITH", 2, collide_with});
    
    // Sound & music commands
    R.add("PLAY_SOUND", NativeFn{"PLAY_SOUND", 1, play_sound});
    R.add("LOOP_MUSIC", NativeFn{"LOOP_MUSIC", 1, loop_music});
    R.add("SET_VOLUME", NativeFn{"SET_VOLUME", -1, set_volume});
    R.add("STOP_SOUND", NativeFn{"STOP_SOUND", 1, stop_sound});
    R.add("STOP_MUSIC", NativeFn{"STOP_MUSIC", 0, stop_music});
}

} // namespace bas

