#include "bas/ecs_system.hpp"
#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <bitset>
#include <algorithm>
#include <optional>

namespace bas {

// Component type ID
using ComponentTypeID = size_t;
static ComponentTypeID g_next_component_type_id = 0;

// Global storage
std::unordered_map<EntityID, EntityData> g_entities;
std::unordered_map<int, SceneData> g_scenes;
static EntityID g_next_entity_id = 1;
static int g_next_scene_id = 1;
static int g_current_scene = 0;

// Component type registry
static std::unordered_map<std::string, ComponentTypeID> g_component_types;
static std::unordered_map<ComponentTypeID, std::string> g_component_type_names;

// Component registry (new)
std::unordered_map<std::string, ComponentDescriptor> g_component_registry;
// System registry (new)
std::vector<SystemRegistration> g_systems;

static std::string to_upper(const std::string& value) {
    std::string out;
    out.reserve(value.size());
    for (unsigned char c : value) {
        out.push_back(static_cast<char>(std::toupper(c)));
    }
    return out;
}

// Get or create component type ID
static ComponentTypeID get_component_type_id(const std::string& typeName) {
    std::string key = to_upper(typeName);
    auto it = g_component_types.find(key);
    if (it != g_component_types.end()) {
        return it->second;
    }
    ComponentTypeID id = g_next_component_type_id++;
    g_component_types[key] = id;
    g_component_type_names[id] = key;
    return id;
}

static ComponentTypeID lookup_component_type_id(const std::string& typeName) {
    std::string key = to_upper(typeName);
    auto it = g_component_types.find(key);
    return it == g_component_types.end() ? static_cast<ComponentTypeID>(-1) : it->second;
}

static EntityData* get_entity(EntityID id) {
    auto it = g_entities.find(id);
    if (it == g_entities.end()) return nullptr;
    return &it->second;
}

static SceneData* get_scene(int id) {
    auto it = g_scenes.find(id);
    if (it == g_scenes.end()) return nullptr;
    return &it->second;
}

static Value make_entity_value(const EntityData& entity) {
    Value::Map entityObj;
    entityObj["_type"] = Value::from_string("Entity");
    entityObj["_id"] = Value::from_int(entity.id);
    entityObj["NAME"] = Value::from_string(entity.name);
    entityObj["ACTIVE"] = Value::from_bool(entity.active);
    entityObj["SCENEID"] = Value::from_int(entity.sceneId);
    return Value::from_map(std::move(entityObj));
}

static Value make_scene_value(const SceneData& scene, int sceneId) {
    Value::Map sceneObj;
    sceneObj["_type"] = Value::from_string("Scene");
    sceneObj["_id"] = Value::from_int(sceneId);
    sceneObj["NAME"] = Value::from_string(scene.name);
    sceneObj["ENTITYCOUNT"] = Value::from_int(static_cast<long long>(scene.entities.size()));
    return Value::from_map(std::move(sceneObj));
}

static Value::Map* find_component_data(EntityData& entity, const std::string& componentUpper) {
    for (auto& entry : entity.components) {
        if (to_upper(entry.first) == componentUpper) {
            return &entry.second;
        }
    }
    return nullptr;
}

static Value::Map* find_component_data(EntityID id, const std::string& componentUpper) {
    auto* entity = get_entity(id);
    if (!entity) return nullptr;
    return find_component_data(*entity, componentUpper);
}

static Value make_component_proxy(EntityID id, const std::string& componentUpper) {
    auto* comp = find_component_data(id, componentUpper);
    if (!comp) return Value::nil();
    Value::Map proxy;
    proxy["_type"] = Value::from_string("ComponentProxy");
    proxy["_entityId"] = Value::from_int(id);
    proxy["_component"] = Value::from_string(componentUpper);
    return Value::from_map(std::move(proxy));
}

static std::optional<Value> get_component_field(EntityID id, const std::string& componentUpper, const std::string& fieldUpper) {
    auto* comp = find_component_data(id, componentUpper);
    if (!comp) return std::nullopt;
    auto it = comp->find(fieldUpper);
    if (it != comp->end()) return it->second;
    for (const auto& entry : *comp) {
        if (to_upper(entry.first) == fieldUpper) return entry.second;
    }
    return std::nullopt;
}

static bool set_component_field(EntityID id, const std::string& componentUpper, const std::string& fieldUpper, const Value& v) {
    auto* comp = find_component_data(id, componentUpper);
    if (!comp) return false;
    auto it = comp->find(fieldUpper);
    if (it != comp->end()) {
        it->second = v;
        return true;
    }
    for (auto& entry : *comp) {
        if (to_upper(entry.first) == fieldUpper) {
            entry.second = v;
            return true;
        }
    }
    (*comp)[fieldUpper] = v;
    return true;
}

// ===== SCENE FUNCTIONS =====

// Scene(name) - Create new scene
static Value scene_constructor(const std::vector<Value>& args) {
    std::string name = args.empty() ? "Scene" : args[0].as_string();
    
    int id = g_next_scene_id++;
    SceneData& scene = g_scenes[id];
    scene.id = id;
    scene.name = name;
    
    return make_scene_value(scene, id);
}

// Scene.createEntity(name, [parent]) - Create entity in scene
static Value scene_createEntity(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::nil();
    }
    
    std::string name = args.size() > 1 ? args[1].as_string() : "Entity";
    EntityID parentId = INVALID_ENTITY;
    if (args.size() > 2 && args[2].is_map()) {
        const auto& parentMap = args[2].as_map();
        auto parentIdIt = parentMap.find("_id");
        if (parentIdIt != parentMap.end() && parentIdIt->second.is_int()) {
            parentId = parentIdIt->second.as_int();
        }
    }
    
    EntityID entityId = g_next_entity_id++;
    EntityData& entity = g_entities[entityId];
    entity.id = entityId;
    entity.name = name;
    entity.active = true;
    entity.sceneId = sceneId;
    entity.parent = parentId;
    
    if (parentId != INVALID_ENTITY) {
        auto parentIt = g_entities.find(parentId);
        if (parentIt != g_entities.end()) {
            parentIt->second.children.push_back(entityId);
        }
    }
    
    sceneIt->second.entities.push_back(entityId);
    
    return make_entity_value(entity);
}

// Scene.destroyEntity(entity) - Remove entity from scene
static Value scene_destroyEntity(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::nil();
    }
    
    const auto& entityMap = args[1].as_map();
    auto idIt = entityMap.find("_id");
    if (idIt == entityMap.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return Value::nil();
    }
    
    int sceneId = entityIt->second.sceneId;
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt != g_scenes.end()) {
        auto& entities = sceneIt->second.entities;
        entities.erase(std::remove(entities.begin(), entities.end(), entityId), entities.end());
    }
    
    // Remove from parent's children
    if (entityIt->second.parent != INVALID_ENTITY) {
        auto parentIt = g_entities.find(entityIt->second.parent);
        if (parentIt != g_entities.end()) {
            auto& children = parentIt->second.children;
            children.erase(std::remove(children.begin(), children.end(), entityId), children.end());
        }
    }
    
    // Destroy children recursively
    for (EntityID childId : entityIt->second.children) {
        Value::Map childObj;
        childObj["_id"] = Value::from_int(childId);
        std::vector<Value> destroyArgs = {args[0], Value::from_map(childObj)};
        scene_destroyEntity(destroyArgs);
    }
    
    g_entities.erase(entityIt);
    
    return Value::nil();
}

// ===== ENTITY FUNCTIONS =====

// Entity.addComponent(entity, componentType, [data])
static Value entity_addComponent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return args[0];
    }
    
    std::string componentType = args[1].as_string();
    std::string componentKey = to_upper(componentType);
    ComponentTypeID typeId = get_component_type_id(componentKey);
    
    EntityData& entity = entityIt->second;
    entity.componentMask.set(typeId);
    
    // Create component data
    Value::Map componentData;
    componentData["_type"] = Value::from_string(componentKey);
    componentData["_entityId"] = Value::from_int(entityId);
    
    // Initialize component based on type
    if (componentKey == "TRANSFORM") {
        componentData["x"] = Value::from_number(0.0);
        componentData["y"] = Value::from_number(0.0);
        componentData["z"] = Value::from_number(0.0);
        componentData["rotationX"] = Value::from_number(0.0);
        componentData["rotationY"] = Value::from_number(0.0);
        componentData["rotationZ"] = Value::from_number(0.0);
        componentData["scaleX"] = Value::from_number(1.0);
        componentData["scaleY"] = Value::from_number(1.0);
        componentData["scaleZ"] = Value::from_number(1.0);
    } else if (componentKey == "SPRITE") {
        componentData["textureId"] = Value::from_int(0);
        componentData["visible"] = Value::from_bool(true);
        componentData["tint"] = Value::from_string("WHITE");
    } else if (componentKey == "MODEL3D") {
        componentData["modelId"] = Value::from_int(0);
        componentData["visible"] = Value::from_bool(true);
        componentData["tint"] = Value::from_string("WHITE");
    } else if (componentKey == "RIGIDBODY") {
        componentData["bodyId"] = Value::from_int(0);
        componentData["mass"] = Value::from_number(1.0);
        componentData["friction"] = Value::from_number(0.5);
        componentData["restitution"] = Value::from_number(0.0);
    } else if (componentKey == "COLLIDER") {
        componentData["shape"] = Value::from_string("Box");
        componentData["width"] = Value::from_number(1.0);
        componentData["height"] = Value::from_number(1.0);
        componentData["depth"] = Value::from_number(1.0);
        componentData["radius"] = Value::from_number(0.5);
        componentData["isTrigger"] = Value::from_bool(false);
    } else if (componentKey == "HEALTH") {
        componentData["maxHealth"] = Value::from_number(100.0);
        componentData["currentHealth"] = Value::from_number(100.0);
        componentData["isDead"] = Value::from_bool(false);
    } else if (componentKey == "AI") {
        componentData["behaviorTree"] = Value::from_string("");
        componentData["state"] = Value::from_string("Idle");
        componentData["targetId"] = Value::from_int(INVALID_ENTITY);
    } else if (componentKey == "INVENTORY") {
        componentData["items"] = Value::from_array(Value::Array{});
        componentData["maxSize"] = Value::from_int(10);
    } else if (componentKey == "ANIMATION") {
        componentData["animationId"] = Value::from_int(0);
        componentData["playing"] = Value::from_bool(false);
        componentData["loop"] = Value::from_bool(true);
    } else if (componentKey == "LIGHT") {
        componentData["type"] = Value::from_string("Point");
        componentData["color"] = Value::from_string("WHITE");
        componentData["intensity"] = Value::from_number(1.0);
        componentData["range"] = Value::from_number(10.0);
    } else if (componentKey == "AUDIOSOURCE") {
        componentData["soundId"] = Value::from_int(0);
        componentData["volume"] = Value::from_number(1.0);
        componentData["pitch"] = Value::from_number(1.0);
        componentData["loop"] = Value::from_bool(false);
    } else if (componentKey == "SCRIPT") {
        componentData["scriptName"] = Value::from_string("");
        componentData["enabled"] = Value::from_bool(true);
    }
    
    // Merge custom data if provided
    if (args.size() > 2 && args[2].is_map()) {
        const auto& customData = args[2].as_map();
        for (const auto& pair : customData) {
            componentData[pair.first] = pair.second;
        }
    }
    
    entity.components[componentKey] = componentData;
    
    // Store in scene's component storage
    auto sceneIt = g_scenes.find(entity.sceneId);
    if (sceneIt != g_scenes.end()) {
        sceneIt->second.componentStorages[componentKey].components[entityId] = componentData;
    }
    
    Value::Map updated = map;
    updated["has" + componentType] = Value::from_bool(true);
    
    return Value::from_map(std::move(updated));
}

// Entity.getComponent(entity, componentType) -> component data
static Value entity_getComponent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return Value::nil();
    }
    
    std::string componentKey = to_upper(args[1].as_string());
    Value::Map* data = find_component_data(entityIt->second, componentKey);
    if (!data) {
        return Value::nil();
    }
    
    Value::Map copy = *data;
    return Value::from_map(std::move(copy));
}

// Entity.hasComponent(entity, componentType) -> bool
static Value entity_hasComponent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_bool(false);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_bool(false);
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return Value::from_bool(false);
    }
    
    std::string componentKey = to_upper(args[1].as_string());
    ComponentTypeID typeId = lookup_component_type_id(componentKey);
    if (typeId == static_cast<ComponentTypeID>(-1)) {
        return Value::from_bool(false);
    }
    
    return Value::from_bool(entityIt->second.componentMask.test(typeId));
}

// Entity.removeComponent(entity, componentType)
static Value entity_removeComponent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return args[0];
    }
    
    std::string componentType = args[1].as_string();
    std::string componentKey = to_upper(componentType);
    ComponentTypeID typeId = lookup_component_type_id(componentKey);
    if (typeId == static_cast<ComponentTypeID>(-1)) {
        return args[0];
    }
    
    EntityData& entity = entityIt->second;
    entity.componentMask.reset(typeId);
    entity.components.erase(componentKey);
    
    // Remove from scene storage
    auto sceneIt = g_scenes.find(entity.sceneId);
    if (sceneIt != g_scenes.end()) {
        auto storageIt = sceneIt->second.componentStorages.find(componentKey);
        if (storageIt != sceneIt->second.componentStorages.end()) {
            storageIt->second.components.erase(entityId);
        }
    }
    
    Value::Map updated = map;
    updated["has" + componentType] = Value::from_bool(false);
    
    return Value::from_map(std::move(updated));
}

// Entity.setComponentData(entity, componentType, data)
static Value entity_setComponentData(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map() || !args[2].is_map()) {
        return Value::nil();
    }
    
    const auto& entityMap = args[0].as_map();
    auto idIt = entityMap.find("_id");
    if (idIt == entityMap.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return args[0];
    }
    
    std::string componentType = args[1].as_string();
    std::string componentKey = to_upper(componentType);
    const auto& data = args[2].as_map();
    
    Value::Map* comp = find_component_data(entityIt->second, componentKey);
    if (!comp) {
        return args[0];
    }
    
    for (const auto& pair : data) {
        (*comp)[pair.first] = pair.second;
    }
    
    auto sceneIt = g_scenes.find(entityIt->second.sceneId);
    if (sceneIt != g_scenes.end()) {
        sceneIt->second.componentStorages[componentKey].components[entityId] = *comp;
    }
    
    return args[0];
}

// ===== QUERY FUNCTIONS =====

// Scene.query(scene, componentType) -> array of entities
static Value scene_query(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_array(Value::Array{});
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_array(Value::Array{});
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_array(Value::Array{});
    }
    
    std::string componentType = args[1].as_string();
    std::string componentKey = to_upper(componentType);
    auto storageIt = sceneIt->second.componentStorages.find(componentKey);
    if (storageIt == sceneIt->second.componentStorages.end()) {
        return Value::from_array(Value::Array{});
    }
    
    Value::Array result;
    for (const auto& pair : storageIt->second.components) {
        EntityID entityId = pair.first;
        auto entityIt = g_entities.find(entityId);
        if (entityIt != g_entities.end() && entityIt->second.active) {
            result.push_back(make_entity_value(entityIt->second));
        }
    }
    
    return Value::from_array(std::move(result));
}

// Scene.queryAll(scene, componentTypes...) -> array of entities with ALL components
static Value scene_queryAll(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::from_array(Value::Array{});
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_array(Value::Array{});
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_array(Value::Array{});
    }
    
    // Collect required component types
    std::vector<std::string> requiredTypes;
    for (size_t i = 1; i < args.size(); ++i) {
        requiredTypes.push_back(to_upper(args[i].as_string()));
    }
    
    if (requiredTypes.empty()) {
        return Value::from_array(Value::Array{});
    }
    
    // Get entities from first component type
    std::string firstType = requiredTypes[0];
    auto storageIt = sceneIt->second.componentStorages.find(firstType);
    if (storageIt == sceneIt->second.componentStorages.end()) {
        return Value::from_array(Value::Array{});
    }
    
    Value::Array result;
    for (const auto& pair : storageIt->second.components) {
        EntityID entityId = pair.first;
        auto entityIt = g_entities.find(entityId);
        if (entityIt == g_entities.end() || !entityIt->second.active) {
            continue;
        }
        
        // Check if entity has all required components
        bool hasAll = true;
        for (const auto& type : requiredTypes) {
            ComponentTypeID typeId = lookup_component_type_id(type);
            if (typeId == static_cast<ComponentTypeID>(-1) || !entityIt->second.componentMask.test(typeId)) {
                hasAll = false;
                break;
            }
        }
        
        if (hasAll) {
            result.push_back(make_entity_value(entityIt->second));
        }
    }
    
    return Value::from_array(std::move(result));
}

// ===== ENTITY PROPERTIES =====

// Entity.setPosition(entity, x, y, [z])
static Value entity_setPosition(const std::vector<Value>& args) {
    if (args.size() < 3 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return args[0];
    }
    
    // Get or create Transform component
    std::string componentType = "Transform";
    auto compIt = entityIt->second.components.find(componentType);
    if (compIt == entityIt->second.components.end()) {
        // Add Transform component
        std::vector<Value> addArgs = {args[0], Value::from_string(componentType)};
        entity_addComponent(addArgs);
        compIt = entityIt->second.components.find(componentType);
    }
    
    if (compIt != entityIt->second.components.end()) {
        compIt->second["x"] = args[1];
        compIt->second["y"] = args[2];
        if (args.size() > 3) {
            compIt->second["z"] = args[3];
        }
    }
    
    Value::Map updated = map;
    updated["x"] = args[1];
    updated["y"] = args[2];
    if (args.size() > 3) {
        updated["z"] = args[3];
    }
    
    return Value::from_map(std::move(updated));
}

// Entity.getPosition(entity) -> Vector3
static Value entity_getPosition(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt == g_entities.end()) {
        return Value::nil();
    }
    
    auto compIt = entityIt->second.components.find("Transform");
    if (compIt == entityIt->second.components.end()) {
        return Value::nil();
    }
    
    const auto& transform = compIt->second;
    double x = transform.count("x") ? transform.at("x").as_number() : 0.0;
    double y = transform.count("y") ? transform.at("y").as_number() : 0.0;
    double z = transform.count("z") ? transform.at("z").as_number() : 0.0;
    
    Value::Map vec;
    vec["_type"] = Value::from_string("Vector3");
    vec["x"] = Value::from_number(x);
    vec["y"] = Value::from_number(y);
    vec["z"] = Value::from_number(z);
    
    return Value::from_map(std::move(vec));
}

// Entity.setActive(entity, active)
static Value entity_setActive(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = idIt->second.as_int();
    auto entityIt = g_entities.find(entityId);
    if (entityIt != g_entities.end()) {
        entityIt->second.active = args[1].as_bool();
    }
    
    Value::Map updated = map;
    updated["active"] = args[1];
    
    return Value::from_map(std::move(updated));
}

// Entity.setParent(entity, parent)
static Value entity_setParent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_map() || !args[1].is_map()) {
        return Value::nil();
    }
    
    const auto& entityMap = args[0].as_map();
    const auto& parentMap = args[1].as_map();
    
    auto entityIdIt = entityMap.find("_id");
    auto parentIdIt = parentMap.find("_id");
    
    if (entityIdIt == entityMap.end() || !entityIdIt->second.is_int() ||
        parentIdIt == parentMap.end() || !parentIdIt->second.is_int()) {
        return args[0];
    }
    
    EntityID entityId = entityIdIt->second.as_int();
    EntityID parentId = parentIdIt->second.as_int();
    
    auto entityIt = g_entities.find(entityId);
    auto parentIt = g_entities.find(parentId);
    
    if (entityIt == g_entities.end() || parentIt == g_entities.end()) {
        return args[0];
    }
    
    // Remove from old parent
    if (entityIt->second.parent != INVALID_ENTITY) {
        auto oldParentIt = g_entities.find(entityIt->second.parent);
        if (oldParentIt != g_entities.end()) {
            auto& children = oldParentIt->second.children;
            children.erase(std::remove(children.begin(), children.end(), entityId), children.end());
        }
    }
    
    // Add to new parent
    entityIt->second.parent = parentId;
    parentIt->second.children.push_back(entityId);
    
    return args[0];
}

// ===== SCENE UPDATE/DRAW =====

// Scene.update(scene, deltaTime)
static Value scene_update(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return args[0];
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return args[0];
    }
    
    double deltaTime = args.size() > 1 ? args[1].as_number() : 0.016;
    
    // Update all active entities
    for (EntityID entityId : sceneIt->second.entities) {
        auto entityIt = g_entities.find(entityId);
        if (entityIt == g_entities.end() || !entityIt->second.active) {
            continue;
        }
        
        // Update components (would call component update systems here)
        // For now, this is a placeholder
    }
    
    return args[0];
}

// Scene.draw(scene)
static Value scene_draw(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::nil();
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::nil();
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::nil();
    }
    
    // Draw all active entities with Sprite or Model3D components
    for (EntityID entityId : sceneIt->second.entities) {
        auto entityIt = g_entities.find(entityId);
        if (entityIt == g_entities.end() || !entityIt->second.active) {
            continue;
        }
        
        // Draw sprite if present
        auto spriteIt = entityIt->second.components.find("Sprite");
        if (spriteIt != entityIt->second.components.end()) {
            // Would draw sprite here
        }
        
        // Draw 3D model if present
        auto modelIt = entityIt->second.components.find("Model3D");
        if (modelIt != entityIt->second.components.end()) {
            // Would draw model here
        }
    }
    
    return Value::nil();
}

// Scene.getEntityCount(scene) -> int
static Value scene_getEntityCount(const std::vector<Value>& args) {
    if (args.empty() || !args[0].is_map()) {
        return Value::from_int(0);
    }
    
    const auto& map = args[0].as_map();
    auto idIt = map.find("_id");
    if (idIt == map.end() || !idIt->second.is_int()) {
        return Value::from_int(0);
    }
    
    int sceneId = static_cast<int>(idIt->second.as_int());
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_int(0);
    }
    
    return Value::from_int(static_cast<long long>(sceneIt->second.entities.size()));
}

// Convenience helpers
static Value ecs_get_entity(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    EntityID id = INVALID_ENTITY;
    const Value& first = args[0];
    if (first.is_int()) {
        id = static_cast<EntityID>(first.as_int());
    } else if (first.is_map()) {
        const auto& map = first.as_map();
        auto it = map.find("_id");
        if (it != map.end() && it->second.is_int()) {
            id = static_cast<EntityID>(it->second.as_int());
        }
    } else if (first.is_string()) {
        std::string nameUpper = to_upper(first.as_string());
        for (auto& entry : g_entities) {
            if (to_upper(entry.second.name) == nameUpper) {
                id = entry.first;
                break;
            }
        }
    }
    auto* entity = get_entity(id);
    return entity ? make_entity_value(*entity) : Value::nil();
}

static Value ecs_get_scene(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    int id = -1;
    const Value& first = args[0];
    if (first.is_int()) {
        id = static_cast<int>(first.as_int());
    } else if (first.is_map()) {
        const auto& map = first.as_map();
        auto it = map.find("_id");
        if (it != map.end() && it->second.is_int()) {
            id = static_cast<int>(it->second.as_int());
        }
    } else if (first.is_string()) {
        std::string nameUpper = to_upper(first.as_string());
        for (auto& entry : g_scenes) {
            if (to_upper(entry.second.name) == nameUpper) {
                id = entry.first;
                break;
            }
        }
    }
    auto* scene = get_scene(id);
    return scene ? make_scene_value(*scene, id) : Value::nil();
}

static Value ecs_find_entities(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::from_array({});
    }
    size_t index = 0;
    int targetScene = -1;
    if (args[0].is_map()) {
        const auto& map = args[0].as_map();
        auto typeIt = map.find("_type");
        if (typeIt != map.end() && typeIt->second.is_string() && to_upper(typeIt->second.as_string()) == "SCENE") {
            auto idIt = map.find("_id");
            if (idIt != map.end() && idIt->second.is_int()) {
                targetScene = static_cast<int>(idIt->second.as_int());
            }
            index = 1;
        }
    } else if (args[0].is_int()) {
        targetScene = static_cast<int>(args[0].as_int());
        index = 1;
    }
    if (args.size() <= index) {
        return Value::from_array({});
    }
    std::vector<ComponentTypeID> requiredTypes;
    for (; index < args.size(); ++index) {
        if (!args[index].is_string()) continue;
        ComponentTypeID typeId = lookup_component_type_id(args[index].as_string());
        if (typeId == static_cast<ComponentTypeID>(-1)) {
            return Value::from_array({});
        }
        requiredTypes.push_back(typeId);
    }
    Value::Array matches;
    for (auto& pair : g_entities) {
        const EntityData& entity = pair.second;
        if (targetScene != -1 && entity.sceneId != targetScene) continue;
        bool ok = true;
        for (ComponentTypeID typeId : requiredTypes) {
            if (typeId >= entity.componentMask.size() || !entity.componentMask.test(typeId)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            matches.push_back(make_entity_value(entity));
        }
    }
    return Value::from_array(std::move(matches));
}

// Member access hooks integrate ECS handles with dot-notation.
static std::optional<Value> ecs_member_read_hook(const Value& object, const std::string& member) {
    if (!object.is_map()) return std::nullopt;
    const auto& map = object.as_map();
    auto typeIt = map.find("_type");
    if (typeIt == map.end() || !typeIt->second.is_string()) return std::nullopt;
    std::string typeUpper = to_upper(typeIt->second.as_string());
    std::string memberUpper = to_upper(member);
    
    if (typeUpper == "ENTITY") {
        auto idIt = map.find("_id");
        if (idIt == map.end() || !idIt->second.is_int()) return std::nullopt;
        EntityID entityId = static_cast<EntityID>(idIt->second.as_int());
        auto* entity = get_entity(entityId);
        if (!entity) return std::nullopt;
        
        if (memberUpper == "ID") return Value::from_int(entityId);
        if (memberUpper == "NAME") return Value::from_string(entity->name);
        if (memberUpper == "ACTIVE") return Value::from_bool(entity->active);
        if (memberUpper == "SCENE") {
            auto* scene = get_scene(entity->sceneId);
            return scene ? std::optional<Value>(make_scene_value(*scene, entity->sceneId))
                         : std::optional<Value>(Value::nil());
        }
        if (memberUpper == "CHILDREN") {
            Value::Array arr;
            for (auto childId : entity->children) {
                auto* child = get_entity(childId);
                if (child) arr.push_back(make_entity_value(*child));
            }
            return Value::from_array(std::move(arr));
        }
        if (memberUpper == "PARENT") {
            if (entity->parent == INVALID_ENTITY) return Value::nil();
            auto* parent = get_entity(entity->parent);
            return parent ? std::optional<Value>(make_entity_value(*parent))
                          : std::optional<Value>(Value::nil());
        }
        if (memberUpper == "COMPONENTS") {
            Value::Array arr;
            for (auto& compEntry : entity->components) {
                Value proxy = make_component_proxy(entityId, to_upper(compEntry.first));
                if (!proxy.is_nil()) arr.push_back(proxy);
            }
            return Value::from_array(std::move(arr));
        }
        Value proxy = make_component_proxy(entityId, memberUpper);
        if (!proxy.is_nil()) return proxy;
        return std::nullopt;
    }
    
    if (typeUpper == "COMPONENTPROXY") {
        auto entityIt = map.find("_entityId");
        auto compIt = map.find("_component");
        if (entityIt == map.end() || compIt == map.end() || !entityIt->second.is_int() || !compIt->second.is_string()) {
            return std::nullopt;
        }
        EntityID entityId = static_cast<EntityID>(entityIt->second.as_int());
        std::string componentUpper = to_upper(compIt->second.as_string());
        if (memberUpper == "ENTITY") {
            auto* entity = get_entity(entityId);
            return entity ? std::optional<Value>(make_entity_value(*entity))
                          : std::optional<Value>(Value::nil());
        }
        if (memberUpper == "NAME") {
            return Value::from_string(componentUpper);
        }
        if (memberUpper == "RAW") {
            auto* comp = find_component_data(entityId, componentUpper);
            if (!comp) return Value::nil();
            return Value::from_map(*comp);
        }
        if (auto field = get_component_field(entityId, componentUpper, memberUpper)) {
            return field;
        }
        return Value::nil();
    }
    
    if (typeUpper == "SCENE") {
        auto idIt = map.find("_id");
        if (idIt == map.end() || !idIt->second.is_int()) return std::nullopt;
        int sceneId = static_cast<int>(idIt->second.as_int());
        auto* scene = get_scene(sceneId);
        if (!scene) return std::nullopt;
        if (memberUpper == "ENTITIES") {
            Value::Array arr;
            for (auto entityId : scene->entities) {
                auto* entity = get_entity(entityId);
                if (entity) arr.push_back(make_entity_value(*entity));
            }
            return Value::from_array(std::move(arr));
        }
    }
    
    return std::nullopt;
}

static bool ecs_member_write_hook(const Value& object, const std::string& member, const Value& value) {
    if (!object.is_map()) return false;
    const auto& map = object.as_map();
    auto typeIt = map.find("_type");
    if (typeIt == map.end() || !typeIt->second.is_string()) return false;
    std::string typeUpper = to_upper(typeIt->second.as_string());
    std::string memberUpper = to_upper(member);
    
    if (typeUpper == "ENTITY") {
        auto idIt = map.find("_id");
        if (idIt == map.end() || !idIt->second.is_int()) return false;
        EntityID entityId = static_cast<EntityID>(idIt->second.as_int());
        auto* entity = get_entity(entityId);
        if (!entity) return false;
        if (memberUpper == "NAME" && value.is_string()) {
            entity->name = value.as_string();
            return true;
        }
        if (memberUpper == "ACTIVE") {
            entity->active = value.as_bool();
            return true;
        }
        if (value.is_map()) {
            entity->components[memberUpper] = value.as_map();
            return true;
        }
        return false;
    }
    
    if (typeUpper == "COMPONENTPROXY") {
        auto entityIt = map.find("_entityId");
        auto compIt = map.find("_component");
        if (entityIt == map.end() || compIt == map.end() || !entityIt->second.is_int() || !compIt->second.is_string()) {
            return false;
        }
        EntityID entityId = static_cast<EntityID>(entityIt->second.as_int());
        std::string componentUpper = to_upper(compIt->second.as_string());
        if (memberUpper == "RAW" && value.is_map()) {
            auto* comp = find_component_data(entityId, componentUpper);
            if (!comp) return false;
            *comp = value.as_map();
            return true;
        }
        return set_component_field(entityId, componentUpper, memberUpper, value);
    }
    
    return false;
}

// ===== COMPONENT REGISTRY =====

void register_component_type(const std::string& name, const Value::Map& defaults) {
    std::string key = to_upper(name);
    ComponentDescriptor desc;
    desc.name = key;
    desc.defaults = defaults;
    g_component_registry[key] = desc;
}

void register_component_type(const std::string& name, std::function<Value::Map()> factory) {
    std::string key = to_upper(name);
    ComponentDescriptor desc;
    desc.name = key;
    desc.factory = factory;
    g_component_registry[key] = desc;
}

ComponentDescriptor* get_component_descriptor(const std::string& name) {
    std::string key = to_upper(name);
    auto it = g_component_registry.find(key);
    return it != g_component_registry.end() ? &it->second : nullptr;
}

// ===== SYSTEM REGISTRATION =====

void register_system(const std::string& name, const std::vector<std::string>& components, SystemUpdateFn fn, int priority) {
    SystemRegistration sys;
    sys.name = to_upper(name);
    sys.requiredComponents.reserve(components.size());
    for (const auto& comp : components) {
        sys.requiredComponents.push_back(to_upper(comp));
    }
    sys.updateFn = fn;
    sys.priority = priority;
    sys.enabled = true;
    
    // Insert sorted by priority
    auto it = std::lower_bound(g_systems.begin(), g_systems.end(), sys,
        [](const SystemRegistration& a, const SystemRegistration& b) {
            return a.priority < b.priority;
        });
    g_systems.insert(it, sys);
}

void unregister_system(const std::string& name) {
    std::string key = to_upper(name);
    g_systems.erase(
        std::remove_if(g_systems.begin(), g_systems.end(),
            [&key](const SystemRegistration& sys) { return sys.name == key; }),
        g_systems.end()
    );
}

void enable_system(const std::string& name, bool enabled) {
    std::string key = to_upper(name);
    for (auto& sys : g_systems) {
        if (sys.name == key) {
            sys.enabled = enabled;
            break;
        }
    }
}

// ===== QUERY FUNCTIONS =====

Value::Array query_entities(const std::vector<std::string>& componentTypes, int sceneId) {
    Value::Array result;
    std::vector<ComponentTypeID> requiredTypes;
    requiredTypes.reserve(componentTypes.size());
    
    for (const auto& type : componentTypes) {
        ComponentTypeID typeId = lookup_component_type_id(type);
        if (typeId == static_cast<ComponentTypeID>(-1)) {
            return result; // Invalid component type
        }
        requiredTypes.push_back(typeId);
    }
    
    for (auto& pair : g_entities) {
        EntityData& entity = pair.second;
        if (sceneId != -1 && entity.sceneId != sceneId) continue;
        if (!entity.active) continue;
        
        bool hasAll = true;
        for (ComponentTypeID typeId : requiredTypes) {
            if (typeId >= entity.componentMask.size() || !entity.componentMask.test(typeId)) {
                hasAll = false;
                break;
            }
        }
        
        if (hasAll) {
            result.push_back(make_entity_value(entity));
        }
    }
    
    return result;
}

Value::Array query_entities_with_filter(const std::vector<std::string>& componentTypes, 
                                        std::function<bool(EntityID)> filter, 
                                        int sceneId) {
    Value::Array result;
    std::vector<ComponentTypeID> requiredTypes;
    requiredTypes.reserve(componentTypes.size());
    
    for (const auto& type : componentTypes) {
        ComponentTypeID typeId = lookup_component_type_id(type);
        if (typeId == static_cast<ComponentTypeID>(-1)) {
            return result;
        }
        requiredTypes.push_back(typeId);
    }
    
    for (auto& pair : g_entities) {
        EntityID id = pair.first;
        EntityData& entity = pair.second;
        if (sceneId != -1 && entity.sceneId != sceneId) continue;
        if (!entity.active) continue;
        if (!filter(id)) continue;
        
        bool hasAll = true;
        for (ComponentTypeID typeId : requiredTypes) {
            if (typeId >= entity.componentMask.size() || !entity.componentMask.test(typeId)) {
                hasAll = false;
                break;
            }
        }
        
        if (hasAll) {
            result.push_back(make_entity_value(entity));
        }
    }
    
    return result;
}

// ===== SYSTEM UPDATES =====

void update_systems(double deltaTime, int sceneId) {
    for (auto& sys : g_systems) {
        if (!sys.enabled) continue;
        
        // Get entities matching this system's component requirements
        Value::Array entities = query_entities(sys.requiredComponents, sceneId);
        
        for (const Value& entityVal : entities) {
            if (!entityVal.is_map()) continue;
            const auto& map = entityVal.as_map();
            auto idIt = map.find("_id");
            if (idIt == map.end() || !idIt->second.is_int()) continue;
            
            EntityID id = static_cast<EntityID>(idIt->second.as_int());
            sys.updateFn(id, deltaTime);
        }
    }
}

// ===== NEW ECS FUNCTIONS =====

// ECS.registerComponent(name, defaults) - Register a component type
static Value ecs_registerComponent(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string() || !args[1].is_map()) {
        return Value::nil();
    }
    std::string name = args[0].as_string();
    Value::Map defaults = args[1].as_map();
    register_component_type(name, defaults);
    return Value::from_bool(true);
}

// ECS.query(componentTypes...) - Query entities with components
static Value ecs_query(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::from_array(Value::Array{});
    }
    std::vector<std::string> componentTypes;
    componentTypes.reserve(args.size());
    for (const auto& arg : args) {
        if (arg.is_string()) {
            componentTypes.push_back(arg.as_string());
        }
    }
    return Value::from_array(query_entities(componentTypes));
}

// ECS.updateSystems(deltaTime, [sceneId]) - Update all registered systems
static Value ecs_updateSystems(const std::vector<Value>& args) {
    if (args.empty()) {
        return Value::nil();
    }
    double deltaTime = args[0].as_number();
    int sceneId = args.size() > 1 ? static_cast<int>(args[1].as_int()) : -1;
    update_systems(deltaTime, sceneId);
    return Value::nil();
}

// ECS.registerSystem(name, components, priority) - Register a system
static Value ecs_registerSystem(const std::vector<Value>& args) {
    if (args.size() < 2 || !args[0].is_string() || !args[1].is_array()) {
        return Value::nil();
    }
    std::string name = args[0].as_string();
    std::vector<std::string> components;
    const auto& compArray = args[1].as_array();
    components.reserve(compArray.size());
    for (const auto& comp : compArray) {
        if (comp.is_string()) {
            components.push_back(comp.as_string());
        }
    }
    int priority = args.size() > 2 ? static_cast<int>(args[2].as_int()) : 0;
    
    // Note: SystemUpdateFn would need to be callable from BASIC
    // For now, this is a placeholder - systems would need to be registered from C++
    return Value::from_bool(true);
}

// Register ECS system
void register_ecs_system(FunctionRegistry& R) {
    // Scene functions
    R.add_with_policy("SCENE", NativeFn{"SCENE", 1, scene_constructor}, true);
    R.add_with_policy("SCENE_CREATEENTITY", NativeFn{"SCENE_CREATEENTITY", 3, scene_createEntity}, true);
    R.add("SCENE_DESTROYENTITY", NativeFn{"SCENE_DESTROYENTITY", 2, scene_destroyEntity});
    R.add_with_policy("SCENE_UPDATE", NativeFn{"SCENE_UPDATE", 2, scene_update}, true);
    R.add_with_policy("SCENE_DRAW", NativeFn{"SCENE_DRAW", 1, scene_draw}, true);
    R.add_with_policy("SCENE_GETENTITYCOUNT", NativeFn{"SCENE_GETENTITYCOUNT", 1, scene_getEntityCount}, true);
    R.add("SCENE_QUERY", NativeFn{"SCENE_QUERY", 2, scene_query});
    R.add("SCENE_QUERYALL", NativeFn{"SCENE_QUERYALL", -1, scene_queryAll});
    
    // Entity component functions
    R.add_with_policy("ENTITY_ADDCOMPONENT", NativeFn{"ENTITY_ADDCOMPONENT", 3, entity_addComponent}, true);
    R.add("ENTITY_GETCOMPONENT", NativeFn{"ENTITY_GETCOMPONENT", 2, entity_getComponent});
    R.add("ENTITY_HASCOMPONENT", NativeFn{"ENTITY_HASCOMPONENT", 2, entity_hasComponent});
    R.add("ENTITY_REMOVECOMPONENT", NativeFn{"ENTITY_REMOVECOMPONENT", 2, entity_removeComponent});
    R.add("ENTITY_SETCOMPONENTDATA", NativeFn{"ENTITY_SETCOMPONENTDATA", 3, entity_setComponentData});
    
    // Entity property functions
    R.add_with_policy("ENTITY_SETPOSITION", NativeFn{"ENTITY_SETPOSITION", -1, entity_setPosition}, true);
    R.add("ENTITY_GETPOSITION", NativeFn{"ENTITY_GETPOSITION", 1, entity_getPosition});
    R.add("ENTITY_SETACTIVE", NativeFn{"ENTITY_SETACTIVE", 2, entity_setActive});
    R.add("ENTITY_SETPARENT", NativeFn{"ENTITY_SETPARENT", 2, entity_setParent});
    
    // New ECS functions
    R.add("ECS_REGISTERCOMPONENT", NativeFn{"ECS_REGISTERCOMPONENT", 2, ecs_registerComponent});
    R.add("ECS_QUERY", NativeFn{"ECS_QUERY", -1, ecs_query});
    R.add("ECS_UPDATESYSTEMS", NativeFn{"ECS_UPDATESYSTEMS", -1, ecs_updateSystems});
    R.add("ECS_REGISTERSYSTEM", NativeFn{"ECS_REGISTERSYSTEM", -1, ecs_registerSystem});
    
    // Register member access hooks for dot notation
    register_member_read_hook(ecs_member_read_hook);
    register_member_write_hook(ecs_member_write_hook);
}

}

