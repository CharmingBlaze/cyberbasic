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

namespace bas {

// Entity ID type
using EntityID = int;
constexpr EntityID INVALID_ENTITY = 0;

// Component type ID
using ComponentTypeID = size_t;
static ComponentTypeID g_next_component_type_id = 0;

// Component storage - stores components by type
struct ComponentStorage {
    std::unordered_map<EntityID, Value::Map> components;  // entityId -> component data
};

// Entity data
struct EntityData {
    EntityID id{INVALID_ENTITY};
    std::string name;
    bool active{true};
    std::bitset<64> componentMask;  // Track which components this entity has
    std::unordered_map<std::string, Value::Map> components;  // componentType -> component data
    int sceneId{0};
    EntityID parent{INVALID_ENTITY};
    std::vector<EntityID> children;
};

// Scene data
struct SceneData {
    int id{0};
    std::string name;
    std::vector<EntityID> entities;
    std::unordered_map<std::string, ComponentStorage> componentStorages;  // componentType -> storage
};

// Global storage
static std::unordered_map<EntityID, EntityData> g_entities;
static std::unordered_map<int, SceneData> g_scenes;
static EntityID g_next_entity_id = 1;
static int g_next_scene_id = 1;
static int g_current_scene = 0;

// Component type registry
static std::unordered_map<std::string, ComponentTypeID> g_component_types;
static std::unordered_map<ComponentTypeID, std::string> g_component_type_names;

// Get or create component type ID
static ComponentTypeID get_component_type_id(const std::string& typeName) {
    auto it = g_component_types.find(typeName);
    if (it != g_component_types.end()) {
        return it->second;
    }
    ComponentTypeID id = g_next_component_type_id++;
    g_component_types[typeName] = id;
    g_component_type_names[id] = typeName;
    return id;
}

// ===== SCENE FUNCTIONS =====

// Scene(name) - Create new scene
static Value scene_constructor(const std::vector<Value>& args) {
    std::string name = args.empty() ? "Scene" : args[0].as_string();
    
    int id = g_next_scene_id++;
    SceneData& scene = g_scenes[id];
    scene.id = id;
    scene.name = name;
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Scene");
    obj["_id"] = Value::from_int(id);
    obj["name"] = Value::from_string(name);
    
    return Value::from_map(std::move(obj));
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
    
    int sceneId = idIt->second.as_int();
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
    
    Value::Map entityObj;
    entityObj["_type"] = Value::from_string("Entity");
    entityObj["_id"] = Value::from_int(entityId);
    entityObj["name"] = Value::from_string(name);
    entityObj["active"] = Value::from_bool(true);
    entityObj["sceneId"] = Value::from_int(sceneId);
    
    return Value::from_map(std::move(entityObj));
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
    ComponentTypeID typeId = get_component_type_id(componentType);
    
    EntityData& entity = entityIt->second;
    entity.componentMask.set(typeId);
    
    // Create component data
    Value::Map componentData;
    componentData["_type"] = Value::from_string(componentType);
    componentData["_entityId"] = Value::from_int(entityId);
    
    // Initialize component based on type
    if (componentType == "Transform" || componentType == "TRANSFORM") {
        componentData["x"] = Value::from_number(0.0);
        componentData["y"] = Value::from_number(0.0);
        componentData["z"] = Value::from_number(0.0);
        componentData["rotationX"] = Value::from_number(0.0);
        componentData["rotationY"] = Value::from_number(0.0);
        componentData["rotationZ"] = Value::from_number(0.0);
        componentData["scaleX"] = Value::from_number(1.0);
        componentData["scaleY"] = Value::from_number(1.0);
        componentData["scaleZ"] = Value::from_number(1.0);
    } else if (componentType == "Sprite" || componentType == "SPRITE") {
        componentData["textureId"] = Value::from_int(0);
        componentData["visible"] = Value::from_bool(true);
        componentData["tint"] = Value::from_string("WHITE");
    } else if (componentType == "Model3D" || componentType == "MODEL3D") {
        componentData["modelId"] = Value::from_int(0);
        componentData["visible"] = Value::from_bool(true);
        componentData["tint"] = Value::from_string("WHITE");
    } else if (componentType == "RigidBody" || componentType == "RIGIDBODY") {
        componentData["bodyId"] = Value::from_int(0);
        componentData["mass"] = Value::from_number(1.0);
        componentData["friction"] = Value::from_number(0.5);
        componentData["restitution"] = Value::from_number(0.0);
    } else if (componentType == "Collider" || componentType == "COLLIDER") {
        componentData["shape"] = Value::from_string("Box");
        componentData["width"] = Value::from_number(1.0);
        componentData["height"] = Value::from_number(1.0);
        componentData["depth"] = Value::from_number(1.0);
        componentData["radius"] = Value::from_number(0.5);
        componentData["isTrigger"] = Value::from_bool(false);
    } else if (componentType == "Health" || componentType == "HEALTH") {
        componentData["maxHealth"] = Value::from_number(100.0);
        componentData["currentHealth"] = Value::from_number(100.0);
        componentData["isDead"] = Value::from_bool(false);
    } else if (componentType == "AI" || componentType == "AI") {
        componentData["behaviorTree"] = Value::from_string("");
        componentData["state"] = Value::from_string("Idle");
        componentData["targetId"] = Value::from_int(INVALID_ENTITY);
    } else if (componentType == "Inventory" || componentType == "INVENTORY") {
        componentData["items"] = Value::from_array(Value::Array{});
        componentData["maxSize"] = Value::from_int(10);
    } else if (componentType == "Animation" || componentType == "ANIMATION") {
        componentData["animationId"] = Value::from_int(0);
        componentData["playing"] = Value::from_bool(false);
        componentData["loop"] = Value::from_bool(true);
    } else if (componentType == "Light" || componentType == "LIGHT") {
        componentData["type"] = Value::from_string("Point");
        componentData["color"] = Value::from_string("WHITE");
        componentData["intensity"] = Value::from_number(1.0);
        componentData["range"] = Value::from_number(10.0);
    } else if (componentType == "AudioSource" || componentType == "AUDIOSOURCE") {
        componentData["soundId"] = Value::from_int(0);
        componentData["volume"] = Value::from_number(1.0);
        componentData["pitch"] = Value::from_number(1.0);
        componentData["loop"] = Value::from_bool(false);
    } else if (componentType == "Script" || componentType == "SCRIPT") {
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
    
    entity.components[componentType] = componentData;
    
    // Store in scene's component storage
    auto sceneIt = g_scenes.find(entity.sceneId);
    if (sceneIt != g_scenes.end()) {
        sceneIt->second.componentStorages[componentType].components[entityId] = componentData;
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
    
    std::string componentType = args[1].as_string();
    auto compIt = entityIt->second.components.find(componentType);
    if (compIt == entityIt->second.components.end()) {
        return Value::nil();
    }
    
    return Value::from_map(compIt->second);
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
    
    std::string componentType = args[1].as_string();
    ComponentTypeID typeId = get_component_type_id(componentType);
    
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
    ComponentTypeID typeId = get_component_type_id(componentType);
    
    EntityData& entity = entityIt->second;
    entity.componentMask.reset(typeId);
    entity.components.erase(componentType);
    
    // Remove from scene storage
    auto sceneIt = g_scenes.find(entity.sceneId);
    if (sceneIt != g_scenes.end()) {
        auto& storage = sceneIt->second.componentStorages[componentType];
        storage.components.erase(entityId);
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
    const auto& data = args[2].as_map();
    
    // Update component data
    auto compIt = entityIt->second.components.find(componentType);
    if (compIt != entityIt->second.components.end()) {
        for (const auto& pair : data) {
            compIt->second[pair.first] = pair.second;
        }
        
        // Update scene storage
        auto sceneIt = g_scenes.find(entityIt->second.sceneId);
        if (sceneIt != g_scenes.end()) {
            sceneIt->second.componentStorages[componentType].components[entityId] = compIt->second;
        }
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
    
    int sceneId = idIt->second.as_int();
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_array(Value::Array{});
    }
    
    std::string componentType = args[1].as_string();
    auto storageIt = sceneIt->second.componentStorages.find(componentType);
    if (storageIt == sceneIt->second.componentStorages.end()) {
        return Value::from_array(Value::Array{});
    }
    
    Value::Array result;
    for (const auto& pair : storageIt->second.components) {
        EntityID entityId = pair.first;
        auto entityIt = g_entities.find(entityId);
        if (entityIt != g_entities.end() && entityIt->second.active) {
            Value::Map entityObj;
            entityObj["_type"] = Value::from_string("Entity");
            entityObj["_id"] = Value::from_int(entityId);
            entityObj["name"] = Value::from_string(entityIt->second.name);
            result.push_back(Value::from_map(std::move(entityObj)));
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
    
    int sceneId = idIt->second.as_int();
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_array(Value::Array{});
    }
    
    // Collect required component types
    std::vector<std::string> requiredTypes;
    for (size_t i = 1; i < args.size(); ++i) {
        requiredTypes.push_back(args[i].as_string());
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
            ComponentTypeID typeId = get_component_type_id(type);
            if (!entityIt->second.componentMask.test(typeId)) {
                hasAll = false;
                break;
            }
        }
        
        if (hasAll) {
            Value::Map entityObj;
            entityObj["_type"] = Value::from_string("Entity");
            entityObj["_id"] = Value::from_int(entityId);
            entityObj["name"] = Value::from_string(entityIt->second.name);
            result.push_back(Value::from_map(std::move(entityObj)));
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
    
    int sceneId = idIt->second.as_int();
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
    
    int sceneId = idIt->second.as_int();
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
    
    int sceneId = idIt->second.as_int();
    auto sceneIt = g_scenes.find(sceneId);
    if (sceneIt == g_scenes.end()) {
        return Value::from_int(0);
    }
    
    return Value::from_int(static_cast<long long>(sceneIt->second.entities.size()));
}

// Register ECS system
void register_ecs_system(FunctionRegistry& R) {
    // Scene functions
    R.add("SCENE", NativeFn{"SCENE", 1, scene_constructor});
    R.add("SCENE_CREATEENTITY", NativeFn{"SCENE_CREATEENTITY", 3, scene_createEntity});
    R.add("SCENE_DESTROYENTITY", NativeFn{"SCENE_DESTROYENTITY", 2, scene_destroyEntity});
    R.add("SCENE_UPDATE", NativeFn{"SCENE_UPDATE", 2, scene_update});
    R.add("SCENE_DRAW", NativeFn{"SCENE_DRAW", 1, scene_draw});
    R.add("SCENE_GETENTITYCOUNT", NativeFn{"SCENE_GETENTITYCOUNT", 1, scene_getEntityCount});
    R.add("SCENE_QUERY", NativeFn{"SCENE_QUERY", 2, scene_query});
    R.add("SCENE_QUERYALL", NativeFn{"SCENE_QUERYALL", -1, scene_queryAll});
    
    // Entity component functions
    R.add("ENTITY_ADDCOMPONENT", NativeFn{"ENTITY_ADDCOMPONENT", 3, entity_addComponent});
    R.add("ENTITY_GETCOMPONENT", NativeFn{"ENTITY_GETCOMPONENT", 2, entity_getComponent});
    R.add("ENTITY_HASCOMPONENT", NativeFn{"ENTITY_HASCOMPONENT", 2, entity_hasComponent});
    R.add("ENTITY_REMOVECOMPONENT", NativeFn{"ENTITY_REMOVECOMPONENT", 2, entity_removeComponent});
    R.add("ENTITY_SETCOMPONENTDATA", NativeFn{"ENTITY_SETCOMPONENTDATA", 3, entity_setComponentData});
    
    // Entity property functions
    R.add("ENTITY_SETPOSITION", NativeFn{"ENTITY_SETPOSITION", -1, entity_setPosition});
    R.add("ENTITY_GETPOSITION", NativeFn{"ENTITY_GETPOSITION", 1, entity_getPosition});
    R.add("ENTITY_SETACTIVE", NativeFn{"ENTITY_SETACTIVE", 2, entity_setActive});
    R.add("ENTITY_SETPARENT", NativeFn{"ENTITY_SETPARENT", 2, entity_setParent});
}

}

