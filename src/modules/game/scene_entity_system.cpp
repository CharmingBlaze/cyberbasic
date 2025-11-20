#include "bas/runtime.hpp"
#include "bas/value.hpp"
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

namespace bas {

// Entity ID type
using EntityID = int;

// Component types
enum class ComponentType {
    Transform,
    Sprite,
    Model3D,
    Physics,
    Collider,
    Script
};

// Transform component (position, rotation, scale)
struct TransformComponent {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    float rotationX{0.0f};
    float rotationY{0.0f};
    float rotationZ{0.0f};
    float scaleX{1.0f};
    float scaleY{1.0f};
    float scaleZ{1.0f};
};

// Sprite component (2D)
struct SpriteComponent {
    int textureId{0};
    Color tint{WHITE};
    bool visible{true};
    int animationId{0};  // Optional animation reference
};

// Model3D component (3D)
struct Model3DComponent {
    int modelId{0};
    Color tint{WHITE};
    bool visible{true};
};

// Scene/Entity storage
struct Entity {
    EntityID id{0};
    std::string name;
    bool active{true};
    TransformComponent transform;
    SpriteComponent sprite;
    Model3DComponent model3d;
    bool hasSprite{false};
    bool hasModel3D{false};
};

static std::unordered_map<EntityID, Entity> g_entities;
static EntityID g_next_entity_id = 1;
static EntityID g_root_entity = 0;

// Scene object
struct SceneData {
    std::string name;
    EntityID rootEntity{0};
    std::vector<EntityID> entities;
};

static std::unordered_map<int, SceneData> g_scenes;
static int g_next_scene_id = 1;
static int g_current_scene = 0;

// Scene constructor: Scene(name)
static Value scene_constructor(const std::vector<Value>& args) {
    std::string name = args.empty() ? "Scene" : (args[0].is_string() ? args[0].as_string() : "Scene");
    
    int id = g_next_scene_id++;
    SceneData& scene = g_scenes[id];
    scene.name = name;
    scene.rootEntity = g_next_entity_id++;
    
    // Create root entity
    Entity root;
    root.id = scene.rootEntity;
    root.name = "Root";
    g_entities[scene.rootEntity] = root;
    scene.entities.push_back(scene.rootEntity);
    
    Value::Map obj;
    obj["_type"] = Value::from_string("Scene");
    obj["_id"] = Value::from_int(id);
    obj["name"] = Value::from_string(name);
    obj["entityCount"] = Value::from_int(1);
    
    return Value::from_map(std::move(obj));
}

// Scene.createEntity(name) - creates a new entity in the scene
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
    
    std::string entityName = args.size() > 1 && args[1].is_string() ? args[1].as_string() : "Entity";
    
    EntityID entityId = g_next_entity_id++;
    Entity entity;
    entity.id = entityId;
    entity.name = entityName;
    entity.active = true;
    g_entities[entityId] = entity;
    sceneIt->second.entities.push_back(entityId);
    
    Value::Map entityObj;
    entityObj["_type"] = Value::from_string("Entity");
    entityObj["_id"] = Value::from_int(entityId);
    entityObj["name"] = Value::from_string(entityName);
    entityObj["active"] = Value::from_bool(true);
    entityObj["x"] = Value::from_number(0.0);
    entityObj["y"] = Value::from_number(0.0);
    entityObj["z"] = Value::from_number(0.0);
    
    return Value::from_map(std::move(entityObj));
}

// Entity.addComponent(componentType, data)
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
    
    std::string componentType = args[1].is_string() ? args[1].as_string() : "";
    
    Value::Map updated = map;
    
    if (componentType == "Sprite" || componentType == "SPRITE") {
        entityIt->second.hasSprite = true;
        updated["hasSprite"] = Value::from_bool(true);
    } else if (componentType == "Model3D" || componentType == "MODEL3D") {
        entityIt->second.hasModel3D = true;
        updated["hasModel3D"] = Value::from_bool(true);
    }
    
    return Value::from_map(std::move(updated));
}

// Entity.setPosition(x, y, [z])
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
    if (entityIt != g_entities.end()) {
        entityIt->second.transform.x = static_cast<float>(args[1].as_number());
        entityIt->second.transform.y = static_cast<float>(args[2].as_number());
        if (args.size() > 3) {
            entityIt->second.transform.z = static_cast<float>(args[3].as_number());
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

// Scene.update() - updates all entities in the scene
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
    
    // Update all entities (placeholder for future logic)
    // In a full implementation, this would update physics, animations, etc.
    
    return args[0];
}

// Scene.draw() - draws all entities in the scene
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
    
    // Draw all entities
    for (EntityID entityId : sceneIt->second.entities) {
        auto entityIt = g_entities.find(entityId);
        if (entityIt == g_entities.end() || !entityIt->second.active) {
            continue;
        }
        
        Entity& entity = entityIt->second;
        
        // Draw 2D sprite if present
        if (entity.hasSprite && entity.sprite.visible) {
            // Would draw sprite at entity.transform.x, entity.transform.y
            // This requires sprite system integration
        }
        
        // Draw 3D model if present
        if (entity.hasModel3D && entity.model3d.visible) {
            // Would draw 3D model at entity.transform position
            // This requires model system integration
        }
    }
    
    return Value::nil();
}

// Scene.getEntityCount() - returns number of entities
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

// Register scene/entity system functions
void register_scene_entity_system(FunctionRegistry& R) {
    R.add("SCENE", NativeFn{"SCENE", -1, scene_constructor});
    R.add("SCENE_CREATEENTITY", NativeFn{"SCENE_CREATEENTITY", -1, scene_createEntity});
    R.add("SCENE_UPDATE", NativeFn{"SCENE_UPDATE", 1, scene_update});
    R.add("SCENE_DRAW", NativeFn{"SCENE_DRAW", 1, scene_draw});
    R.add("SCENE_GETENTITYCOUNT", NativeFn{"SCENE_GETENTITYCOUNT", 1, scene_getEntityCount});
    R.add("ENTITY_ADDCOMPONENT", NativeFn{"ENTITY_ADDCOMPONENT", -1, entity_addComponent});
    R.add("ENTITY_SETPOSITION", NativeFn{"ENTITY_SETPOSITION", -1, entity_setPosition});
}

} // namespace bas

