#pragma once
#include "runtime.hpp"
#include "value.hpp"
#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace bas {

using EntityID = int;
constexpr EntityID INVALID_ENTITY = 0;

// Component descriptor for registry
struct ComponentDescriptor {
    std::string name;
    Value::Map defaults;
    std::function<Value::Map()> factory; // Optional factory function
};

// System function type for component updates
using SystemUpdateFn = std::function<void(EntityID, double)>;

// System registration
struct SystemRegistration {
    std::string name;
    std::vector<std::string> requiredComponents;
    SystemUpdateFn updateFn;
    int priority{0}; // Lower = higher priority
    bool enabled{true};
};

struct ComponentStorage {
    std::unordered_map<EntityID, Value::Map> components;
};

struct EntityData {
    EntityID id{INVALID_ENTITY};
    std::string name;
    bool active{true};
    std::bitset<64> componentMask;
    std::unordered_map<std::string, Value::Map> components;
    int sceneId{0};
    EntityID parent{INVALID_ENTITY};
    std::vector<EntityID> children;
    int generation{0}; // For safer handles
};

struct SceneData {
    int id{0};
    std::string name;
    std::vector<EntityID> entities;
    std::unordered_map<std::string, ComponentStorage> componentStorages;
};

// Component registry
extern std::unordered_map<std::string, ComponentDescriptor> g_component_registry;
// System registry
extern std::vector<SystemRegistration> g_systems;

extern std::unordered_map<EntityID, EntityData> g_entities;
extern std::unordered_map<int, SceneData> g_scenes;

// Component registry functions
void register_component_type(const std::string& name, const Value::Map& defaults);
void register_component_type(const std::string& name, std::function<Value::Map()> factory);
ComponentDescriptor* get_component_descriptor(const std::string& name);

// System registration
void register_system(const std::string& name, const std::vector<std::string>& components, SystemUpdateFn fn, int priority = 0);
void unregister_system(const std::string& name);
void enable_system(const std::string& name, bool enabled);

// Query functions
Value::Array query_entities(const std::vector<std::string>& componentTypes, int sceneId = -1);
Value::Array query_entities_with_filter(const std::vector<std::string>& componentTypes, 
                                         std::function<bool(EntityID)> filter, 
                                         int sceneId = -1);

// Update all systems
void update_systems(double deltaTime, int sceneId = -1);

void register_ecs_system(FunctionRegistry& registry);

} // namespace bas

