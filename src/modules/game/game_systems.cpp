#include "bas/runtime.hpp"
#include "bas/game_systems.hpp"
#include "bas/physics.hpp"
#include "bas/ai.hpp"
#include "bas/navigation.hpp"
#include "bas/networking.hpp"
#include "bas/audio.hpp"
#include "bas/graphics.hpp"
#include "bas/input.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace bas {

// Game Systems Manager - Central hub for all game programming systems
class GameSystemsManager {
private:
    bool initialized = false;
    std::unordered_map<std::string, bool> system_status;
    
public:
    GameSystemsManager() {
        // Initialize all system statuses
        system_status["graphics"] = false;
        system_status["audio"] = false;
        system_status["input"] = false;
        system_status["physics"] = false;
        system_status["ai"] = false;
        system_status["navigation"] = false;
        system_status["networking"] = false;
    }
    
    void initialize_system(const std::string& system_name) {
        if (system_status.find(system_name) != system_status.end()) {
            system_status[system_name] = true;
        }
    }
    
    bool is_system_initialized(const std::string& system_name) const {
        auto it = system_status.find(system_name);
        return it != system_status.end() ? it->second : false;
    }
    
    void shutdown_system(const std::string& system_name) {
        if (system_status.find(system_name) != system_status.end()) {
            system_status[system_name] = false;
        }
    }
    
    void shutdown_all() {
        for (auto& pair : system_status) {
            pair.second = false;
        }
    }
};

static GameSystemsManager g_game_systems;

// Game Object System
struct GameObject {
    int id;
    std::string name;
    float x, y, z;
    float rotation_x, rotation_y, rotation_z;
    float scale_x, scale_y, scale_z;
    bool visible;
    bool active;
    std::unordered_map<std::string, Value> properties;
    
    GameObject(int obj_id, const std::string& obj_name) 
        : id(obj_id), name(obj_name), x(0), y(0), z(0), 
          rotation_x(0), rotation_y(0), rotation_z(0),
          scale_x(1), scale_y(1), scale_z(1), visible(true), active(true) {}
};

static std::unordered_map<int, std::unique_ptr<GameObject>> g_game_objects;
static int g_next_object_id = 1;

// Scene Management
struct Scene {
    std::string name;
    std::vector<int> object_ids;
    bool active;
    
    Scene(const std::string& scene_name) : name(scene_name), active(false) {}
};

static std::unordered_map<std::string, std::unique_ptr<Scene>> g_scenes;
static std::string g_current_scene;

// Game State Management
struct GameState {
    std::string current_state;
    std::unordered_map<std::string, Value> state_data;
    float delta_time;
    int frame_count;
    
    GameState() : current_state("menu"), delta_time(0.0f), frame_count(0) {}
};

static GameState g_game_state;

void register_game_systems_bindings(FunctionRegistry& R) {
    
    // === GAME SYSTEMS MANAGEMENT ===
    
    R.add("INITGAMESYSTEMS", NativeFn{"INITGAMESYSTEMS", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        // Initialize all game systems
        g_game_systems.initialize_system("graphics");
        g_game_systems.initialize_system("audio");
        g_game_systems.initialize_system("input");
        g_game_systems.initialize_system("physics");
        g_game_systems.initialize_system("ai");
        g_game_systems.initialize_system("navigation");
        g_game_systems.initialize_system("networking");
        return Value::nil();
    }});
    
    R.add("SHUTDOWNGAMESYSTEMS", NativeFn{"SHUTDOWNGAMESYSTEMS", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        g_game_systems.shutdown_all();
        return Value::nil();
    }});
    
    R.add("ISSYSTEMINITIALIZED", NativeFn{"ISSYSTEMINITIALIZED", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("ISSYSTEMINITIALIZED: expected 1 arg");
        std::string system_name = args[0].as_string();
        return Value::from_bool(g_game_systems.is_system_initialized(system_name));
    }});
    
    // === GAME OBJECT MANAGEMENT ===
    
    R.add("CREATEGAMEOBJECT", NativeFn{"CREATEGAMEOBJECT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CREATEGAMEOBJECT: expected 1 arg");
        std::string name = args[0].as_string();
        int id = g_next_object_id++;
        g_game_objects[id] = std::make_unique<GameObject>(id, name);
        return Value::from_int(id);
    }});
    
    R.add("DESTROYGAMEOBJECT", NativeFn{"DESTROYGAMEOBJECT", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("DESTROYGAMEOBJECT: expected 1 arg");
        int id = args[0].as_int();
        g_game_objects.erase(id);
        return Value::nil();
    }});
    
    R.add("SETGAMEOBJECTPOSITION", NativeFn{"SETGAMEOBJECTPOSITION", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEOBJECTPOSITION: expected 4 args");
        int id = args[0].as_int();
        float x = static_cast<float>(args[1].as_number());
        float y = static_cast<float>(args[2].as_number());
        float z = static_cast<float>(args[3].as_number());
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            it->second->x = x;
            it->second->y = y;
            it->second->z = z;
        }
        return Value::nil();
    }});
    
    R.add("GETGAMEOBJECTPOSITION", NativeFn{"GETGAMEOBJECTPOSITION", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMEOBJECTPOSITION: expected 1 arg");
        int id = args[0].as_int();
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            Value::Array pos;
            pos.push_back(Value::from_number(it->second->x));
            pos.push_back(Value::from_number(it->second->y));
            pos.push_back(Value::from_number(it->second->z));
            return Value::from_array(pos);
        }
        return Value::from_array({Value::from_number(0), Value::from_number(0), Value::from_number(0)});
    }});
    
    R.add("SETGAMEOBJECTROTATION", NativeFn{"SETGAMEOBJECTROTATION", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEOBJECTROTATION: expected 4 args");
        int id = args[0].as_int();
        float rx = static_cast<float>(args[1].as_number());
        float ry = static_cast<float>(args[2].as_number());
        float rz = static_cast<float>(args[3].as_number());
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            it->second->rotation_x = rx;
            it->second->rotation_y = ry;
            it->second->rotation_z = rz;
        }
        return Value::nil();
    }});
    
    R.add("SETGAMEOBJECTSCALE", NativeFn{"SETGAMEOBJECTSCALE", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("SETGAMEOBJECTSCALE: expected 4 args");
        int id = args[0].as_int();
        float sx = static_cast<float>(args[1].as_number());
        float sy = static_cast<float>(args[2].as_number());
        float sz = static_cast<float>(args[3].as_number());
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            it->second->scale_x = sx;
            it->second->scale_y = sy;
            it->second->scale_z = sz;
        }
        return Value::nil();
    }});
    
    R.add("SETGAMEOBJECTVISIBLE", NativeFn{"SETGAMEOBJECTVISIBLE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETGAMEOBJECTVISIBLE: expected 2 args");
        int id = args[0].as_int();
        bool visible = args[1].as_bool();
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            it->second->visible = visible;
        }
        return Value::nil();
    }});
    
    R.add("SETGAMEOBJECTACTIVE", NativeFn{"SETGAMEOBJECTACTIVE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETGAMEOBJECTACTIVE: expected 2 args");
        int id = args[0].as_int();
        bool active = args[1].as_bool();
        
        auto it = g_game_objects.find(id);
        if (it != g_game_objects.end()) {
            it->second->active = active;
        }
        return Value::nil();
    }});
    
    R.add("GETGAMEOBJECTCOUNT", NativeFn{"GETGAMEOBJECTCOUNT", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        return Value::from_int(static_cast<int>(g_game_objects.size()));
    }});
    
    // === SCENE MANAGEMENT ===
    
    R.add("CREATESCENE", NativeFn{"CREATESCENE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("CREATESCENE: expected 1 arg");
        std::string name = args[0].as_string();
        g_scenes[name] = std::make_unique<Scene>(name);
        return Value::nil();
    }});
    
    R.add("LOADSCENE", NativeFn{"LOADSCENE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("LOADSCENE: expected 1 arg");
        std::string name = args[0].as_string();
        
        // Deactivate current scene
        if (!g_current_scene.empty()) {
            auto it = g_scenes.find(g_current_scene);
            if (it != g_scenes.end()) {
                it->second->active = false;
            }
        }
        
        // Activate new scene
        auto it = g_scenes.find(name);
        if (it != g_scenes.end()) {
            it->second->active = true;
            g_current_scene = name;
        }
        return Value::nil();
    }});
    
    R.add("ADDOBJECTTOSCENE", NativeFn{"ADDOBJECTTOSCENE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("ADDOBJECTTOSCENE: expected 2 args");
        std::string scene_name = args[0].as_string();
        int object_id = args[1].as_int();
        
        auto it = g_scenes.find(scene_name);
        if (it != g_scenes.end()) {
            it->second->object_ids.push_back(object_id);
        }
        return Value::nil();
    }});
    
    R.add("REMOVEOBJECTFROMSCENE", NativeFn{"REMOVEOBJECTFROMSCENE", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("REMOVEOBJECTFROMSCENE: expected 2 args");
        std::string scene_name = args[0].as_string();
        int object_id = args[1].as_int();
        
        auto it = g_scenes.find(scene_name);
        if (it != g_scenes.end()) {
            auto& ids = it->second->object_ids;
            ids.erase(std::remove(ids.begin(), ids.end(), object_id), ids.end());
        }
        return Value::nil();
    }});
    
    R.add("GETCURRENTSCENE", NativeFn{"GETCURRENTSCENE", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        return Value::from_string(g_current_scene);
    }});
    
    // === GAME STATE MANAGEMENT ===
    
    R.add("SETGAMESTATE", NativeFn{"SETGAMESTATE", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("SETGAMESTATE: expected 1 arg");
        g_game_state.current_state = args[0].as_string();
        return Value::nil();
    }});
    
    R.add("GETGAMESTATE", NativeFn{"GETGAMESTATE", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        return Value::from_string(g_game_state.current_state);
    }});
    
    R.add("SETGAMESTATEDATA", NativeFn{"SETGAMESTATEDATA", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("SETGAMESTATEDATA: expected 2 args");
        std::string key = args[0].as_string();
        g_game_state.state_data[key] = args[1];
        return Value::nil();
    }});
    
    R.add("GETGAMESTATEDATA", NativeFn{"GETGAMESTATEDATA", 1, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 1) throw std::runtime_error("GETGAMESTATEDATA: expected 1 arg");
        std::string key = args[0].as_string();
        
        auto it = g_game_state.state_data.find(key);
        if (it != g_game_state.state_data.end()) {
            return it->second;
        }
        return Value::nil();
    }});
    
    R.add("UPDATEGAMESTATE", NativeFn{"UPDATEGAMESTATE", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        g_game_state.delta_time = GetFrameTime();
        g_game_state.frame_count++;
        return Value::nil();
    }});
    
    R.add("GETFRAMECOUNT", NativeFn{"GETFRAMECOUNT", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        return Value::from_int(g_game_state.frame_count);
    }});
    

    
    // === COLLISION DETECTION ===
    
    R.add("CHECKCOLLISION", NativeFn{"CHECKCOLLISION", 2, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 2) throw std::runtime_error("CHECKCOLLISION: expected 2 args");
        int obj1_id = args[0].as_int();
        int obj2_id = args[1].as_int();
        
        auto it1 = g_game_objects.find(obj1_id);
        auto it2 = g_game_objects.find(obj2_id);
        
        if (it1 != g_game_objects.end() && it2 != g_game_objects.end()) {
            auto& obj1 = it1->second;
            auto& obj2 = it2->second;
            
            // Simple AABB collision detection
            float obj1_left = obj1->x - obj1->scale_x / 2;
            float obj1_right = obj1->x + obj1->scale_x / 2;
            float obj1_top = obj1->y - obj1->scale_y / 2;
            float obj1_bottom = obj1->y + obj1->scale_y / 2;
            
            float obj2_left = obj2->x - obj2->scale_x / 2;
            float obj2_right = obj2->x + obj2->scale_x / 2;
            float obj2_top = obj2->y - obj2->scale_y / 2;
            float obj2_bottom = obj2->y + obj2->scale_y / 2;
            
            bool colliding = (obj1_left < obj2_right && obj1_right > obj2_left &&
                            obj1_top < obj2_bottom && obj1_bottom > obj2_top);
            
            return Value::from_bool(colliding);
        }
        return Value::from_bool(false);
    }});
    
    // === GAME LOOP HELPERS ===
    
    R.add("GAMELOOPSTART", NativeFn{"GAMELOOPSTART", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        // Initialize game systems if not already done
        if (!g_game_systems.is_system_initialized("graphics")) {
            g_game_systems.initialize_system("graphics");
        }
        if (!g_game_systems.is_system_initialized("input")) {
            g_game_systems.initialize_system("input");
        }
        
        BeginDrawing();
        return Value::nil();
    }});
    
    R.add("GAMELOOPEND", NativeFn{"GAMELOOPEND", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        EndDrawing();
        return Value::nil();
    }});
    
    R.add("GAMELOOPUPDATE", NativeFn{"GAMELOOPUPDATE", 0, [](const std::vector<Value>& args) -> Value {
        (void)args; // Suppress unused parameter warning
        // Update game state
        g_game_state.delta_time = GetFrameTime();
        g_game_state.frame_count++;
        
        // Update physics if initialized
        if (g_game_systems.is_system_initialized("physics") && g_physics_world) {
            g_physics_world->update(g_game_state.delta_time);
        }
        
        return Value::nil();
    }});
    
    // === UTILITY FUNCTIONS ===
    

    
    R.add("ANGLE", NativeFn{"ANGLE", 4, [](const std::vector<Value>& args) -> Value {
        if (args.size() != 4) throw std::runtime_error("ANGLE: expected 4 args");
        float x1 = static_cast<float>(args[0].as_number());
        float y1 = static_cast<float>(args[1].as_number());
        float x2 = static_cast<float>(args[2].as_number());
        float y2 = static_cast<float>(args[3].as_number());
        
        float dx = x2 - x1;
        float dy = y2 - y1;
        float angle = std::atan2(dy, dx);
        
        return Value::from_number(angle);
    }});
    
    // LERP function removed - already available in core builtins
    

}

} // namespace bas
