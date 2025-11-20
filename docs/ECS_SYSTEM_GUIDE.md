# Enhanced ECS (Entity-Component-System) Guide

## Overview

CyberBasic now includes a **feature-rich ECS system** that provides dynamic component management, entity queries, hierarchical scenes, and much more.

## Current System Assessment

### Previous System (Basic)
- Hardcoded components in Entity struct
- Only `addComponent()` - no get/remove/has
- No component queries
- No component systems
- Limited to 6 component types

### New Enhanced System
- **Dynamic component storage** - Add any component type at runtime
- **Full CRUD operations** - Add, Get, Remove, Has, SetData
- **Component queries** - Find entities by component type(s)
- **Hierarchical entities** - Parent/child relationships
- **12+ built-in component types** - Transform, Sprite, Model3D, RigidBody, Collider, Health, AI, Inventory, Animation, Light, AudioSource, Script
- **Component data access** - Read and modify component properties
- **Scene management** - Create, destroy, query entities in scenes

## Core Concepts

### Entities
Entities are containers for components. They have:
- Unique ID
- Name
- Active state
- Parent/child relationships
- Scene membership

### Components
Components are data containers attached to entities:
- Transform (position, rotation, scale)
- Sprite (2D rendering)
- Model3D (3D rendering)
- RigidBody (physics)
- Collider (collision detection)
- Health (gameplay)
- AI (behavior)
- Inventory (items)
- Animation (frame-based)
- Light (lighting)
- AudioSource (sound)
- Script (custom logic)

### Scenes
Scenes contain entities and manage their lifecycle:
- Create/destroy entities
- Query entities by component
- Update/draw all entities

## API Reference

### Scene Functions

#### `Scene(name)` - Create Scene
```basic
VAR scene = Scene("MainScene")
```

#### `scene.createEntity(name, [parent])` - Create Entity
```basic
VAR player = scene.createEntity("Player")
VAR child = scene.createEntity("Child", player)  // With parent
```

#### `scene.destroyEntity(entity)` - Remove Entity
```basic
scene.destroyEntity(player)  // Also destroys children recursively
```

#### `scene.query(componentType)` - Query Entities
```basic
VAR sprites = scene.query("Sprite")  // All entities with Sprite component
FOR EACH entity IN sprites
    PRINT entity.name
NEXT
```

#### `scene.queryAll(componentType1, componentType2, ...)` - Query Multiple
```basic
// Entities with BOTH Sprite AND Transform
VAR renderables = scene.queryAll("Sprite", "Transform")
```

#### `scene.update(deltaTime)` - Update Scene
```basic
VAR delta = Clock.getDelta()
scene.update(delta)
```

#### `scene.draw()` - Draw Scene
```basic
scene.draw()  // Draws all entities with Sprite/Model3D components
```

#### `scene.getEntityCount()` - Get Count
```basic
VAR count = scene.getEntityCount()
```

### Entity Component Functions

#### `entity.addComponent(componentType, [data])` - Add Component
```basic
VAR player = scene.createEntity("Player")

// Add Transform component
player.addComponent("Transform")

// Add Sprite with custom data
VAR spriteData = {
    textureId = 1,
    visible = TRUE,
    tint = "WHITE"
}
player.addComponent("Sprite", spriteData)

// Add Health component
VAR healthData = {
    maxHealth = 100,
    currentHealth = 100
}
player.addComponent("Health", healthData)
```

#### `entity.getComponent(componentType)` - Get Component
```basic
VAR transform = player.getComponent("Transform")
VAR x = transform.x
VAR y = transform.y
VAR z = transform.z
```

#### `entity.hasComponent(componentType)` - Check Component
```basic
IF player.hasComponent("Sprite") THEN
    PRINT "Player has sprite"
END IF
```

#### `entity.removeComponent(componentType)` - Remove Component
```basic
player.removeComponent("Sprite")
```

#### `entity.setComponentData(componentType, data)` - Update Component
```basic
VAR newData = {
    currentHealth = 50
}
player.setComponentData("Health", newData)
```

### Entity Property Functions

#### `entity.setPosition(x, y, [z])` - Set Position
```basic
player.setPosition(100, 200, 0)
```

#### `entity.getPosition()` - Get Position
```basic
VAR pos = player.getPosition()
PRINT "X: " + STR(pos.x) + ", Y: " + STR(pos.y)
```

#### `entity.setActive(active)` - Set Active State
```basic
player.setActive(FALSE)  // Hide/disable entity
```

#### `entity.setParent(parent)` - Set Parent
```basic
VAR weapon = scene.createEntity("Weapon")
weapon.setParent(player)  // Weapon follows player
```

## Built-in Component Types

### Transform Component
```basic
VAR transform = {
    x = 0.0,
    y = 0.0,
    z = 0.0,
    rotationX = 0.0,
    rotationY = 0.0,
    rotationZ = 0.0,
    scaleX = 1.0,
    scaleY = 1.0,
    scaleZ = 1.0
}
```

### Sprite Component
```basic
VAR sprite = {
    textureId = 0,
    visible = TRUE,
    tint = "WHITE"
}
```

### Model3D Component
```basic
VAR model = {
    modelId = 0,
    visible = TRUE,
    tint = "WHITE"
}
```

### RigidBody Component
```basic
VAR body = {
    bodyId = 0,
    mass = 1.0,
    friction = 0.5,
    restitution = 0.0
}
```

### Collider Component
```basic
VAR collider = {
    shape = "Box",  // "Box" or "Sphere"
    width = 1.0,
    height = 1.0,
    depth = 1.0,
    radius = 0.5,
    isTrigger = FALSE
}
```

### Health Component
```basic
VAR health = {
    maxHealth = 100.0,
    currentHealth = 100.0,
    isDead = FALSE
}
```

### AI Component
```basic
VAR ai = {
    behaviorTree = "",
    state = "Idle",
    targetId = 0
}
```

### Inventory Component
```basic
VAR inventory = {
    items = [],
    maxSize = 10
}
```

### Animation Component
```basic
VAR anim = {
    animationId = 0,
    playing = FALSE,
    loop = TRUE
}
```

### Light Component
```basic
VAR light = {
    type = "Point",  // "Point", "Directional", "Spot"
    color = "WHITE",
    intensity = 1.0,
    range = 10.0
}
```

### AudioSource Component
```basic
VAR audio = {
    soundId = 0,
    volume = 1.0,
    pitch = 1.0,
    loop = FALSE
}
```

### Script Component
```basic
VAR script = {
    scriptName = "",
    enabled = TRUE
}
```

## Complete Example

```basic
// Initialize game
GAME.init(800, 600, "ECS Demo", 60)

// Create scene
VAR scene = Scene("GameScene")

// Create player entity
VAR player = scene.createEntity("Player")
player.addComponent("Transform")
player.addComponent("Sprite", {textureId = 1, visible = TRUE})
player.addComponent("Health", {maxHealth = 100, currentHealth = 100})
player.addComponent("Collider", {shape = "Box", width = 32, height = 32})
player.setPosition(400, 300, 0)

// Create enemy entity
VAR enemy = scene.createEntity("Enemy")
enemy.addComponent("Transform")
enemy.addComponent("Sprite", {textureId = 2, visible = TRUE})
enemy.addComponent("Health", {maxHealth = 50, currentHealth = 50})
enemy.addComponent("AI", {state = "Patrol"})
enemy.setPosition(200, 200, 0)

// Create weapon as child of player
VAR weapon = scene.createEntity("Weapon")
weapon.addComponent("Transform")
weapon.addComponent("Sprite", {textureId = 3})
weapon.setParent(player)

// Game loop
WHILE NOT GAME.shouldClose()
    GAME.beginFrame()
    VAR delta = Clock.getDelta()
    
    // Update scene
    scene.update(delta)
    
    // Query all entities with Health component
    VAR healthEntities = scene.query("Health")
    FOR EACH entity IN healthEntities
        VAR health = entity.getComponent("Health")
        IF health.currentHealth <= 0 THEN
            entity.setActive(FALSE)
        END IF
    NEXT
    
    // Draw scene
    scene.draw()
    
    GAME.endFrame()
WEND

GAME.close()
```

## Advanced Features

### Component Queries
```basic
// Find all entities with Sprite AND Transform
VAR renderables = scene.queryAll("Sprite", "Transform")

// Find all entities with Health AND AI
VAR enemies = scene.queryAll("Health", "AI")
```

### Hierarchical Entities
```basic
VAR parent = scene.createEntity("Parent")
VAR child1 = scene.createEntity("Child1", parent)
VAR child2 = scene.createEntity("Child2", parent)

// When parent moves, children follow (automatic in update)
parent.setPosition(100, 100, 0)
```

### Dynamic Component Management
```basic
// Add component at runtime
IF player.hasComponent("Health") THEN
    VAR health = player.getComponent("Health")
    health.currentHealth = health.currentHealth - 10
    player.setComponentData("Health", health)
END IF

// Remove component
IF player.hasComponent("Sprite") THEN
    player.removeComponent("Sprite")
END IF
```

## Performance Considerations

- Component queries are O(n) where n is number of entities with that component
- Use `queryAll()` sparingly - it's more expensive than single `query()`
- Destroy entities when done to free memory
- Consider component pooling for frequently created/destroyed entities

## Future Enhancements

Planned features:
- Component systems (UpdateSystem, RenderSystem, etc.)
- Component events/callbacks
- Component serialization (save/load)
- Component archetypes/prefabs
- Component dependencies
- Component validation
- Component pooling
- Multi-threaded component updates

