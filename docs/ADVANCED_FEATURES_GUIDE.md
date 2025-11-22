# CyberBasic Advanced Features Guide

**Complete guide to enums, dot notation, state machines, ECS, coroutines, tuples, and advanced programming features**

## Table of Contents

1. [Enums](#enums)
2. [Dot Notation](#dot-notation)
3. [State Machine System](#state-machine-system)
4. [ECS (Entity-Component-System)](#ecs-entity-component-system)
5. [Coroutines and Async](#coroutines-and-async)
6. [Tuples](#tuples)
7. [Dictionaries](#dictionaries)
8. [Libraries and Modules](#libraries-and-modules)
9. [Advanced Type System](#advanced-type-system)
10. [Error Handling and Debugging](#error-handling-and-debugging)
11. [Performance Optimization](#performance-optimization)
12. [Complete Examples](#complete-examples)

---

## Enums

Enumerations provide a way to define a set of named constants, making code more readable and maintainable.

### Enum Declaration

Enums can be declared using the `ENUM` keyword with two syntaxes:

**Named Enums:**
```basic
ENUM Direction
    North, South, East, West
END ENUM

ENUM GameState
    Menu, Playing, Paused, GameOver
END ENUM
```

**Unnamed Enums:**
```basic
ENUM
    UNIT_NEUTRAL
    UNIT_FRIENDLY
    UNIT_ENEMY
END ENUM
```

**Custom Values:**
```basic
ENUM Status
    IDLE = 0
    WALKING = 1
    RUNNING = 2
    JUMPING = 10
    FALLING = 11
END ENUM
```

### Using Enums

Enums are automatically registered as constants:

```basic
ENUM Direction
    North, South, East, West
END ENUM

REM Use enum values directly
VAR dir = North  REM 0
VAR dir2 = South  REM 1

REM Enums work in expressions
IF dir == North THEN
    PRINT "Going north"
ENDIF

REM Enums in arrays
VAR directions = [North, South, East, West]
```

### Enum Functions

#### Enum.getValue()

```basic
VAR value = Enum.getValue("EnumName", "ValueName")
```

Returns the integer value associated with an enum value name.

#### Enum.getName()

```basic
VAR name = Enum.getName("EnumName", value)
```

Returns the name of an enum value given its integer value.

#### Enum.hasValue()

```basic
VAR exists = Enum.hasValue("EnumName", "ValueName")
```

Checks if an enum contains a specific value name.

### Enum Best Practices

```basic
REM Use enums for related constants
ENUM WeaponType
    Sword, Axe, Bow, Staff
END ENUM

ENUM ItemRarity
    Common, Uncommon, Rare, Epic, Legendary
END ENUM

REM Use enums in functions
FUNCTION getWeaponDamage(weaponType)
    SELECT CASE weaponType
        CASE Enum.getValue(weaponEnum, "Sword")
            RETURN 10
        CASE Enum.getValue(weaponEnum, "Axe")
            RETURN 15
        CASE Enum.getValue(weaponEnum, "Bow")
            RETURN 8
        CASE ELSE
            RETURN 5
    ENDSELECT
ENDFUNCTION
```

### Enum Example

```basic
REM Game state management with enums
ENUM State
    Menu, Playing, Paused, GameOver
END ENUM

VAR stateEnum = Enum("State", "Menu", "Playing", "Paused", "GameOver")
VAR currentState = Enum.getValue(stateEnum, "Menu")

FUNCTION changeState(newState$)
    currentState = Enum.getValue(stateEnum, newState$)
    PRINT "State changed to: " + newState$
ENDFUNCTION

FUNCTION isState(stateName$)
    RETURN currentState = Enum.getValue(stateEnum, stateName$)
ENDFUNCTION

REM Usage
IF isState("Menu") THEN
    PRINT "Showing menu"
ELSEIF isState("Playing") THEN
    PRINT "Game is running"
ENDIF
```

---

## Dictionary Literals

Dictionaries (also called maps or objects) are key-value data structures. CyberBasic supports two syntax styles for creating dictionaries.

### JSON-Style Syntax

Uses colons to separate keys and values, similar to JSON:

```basic
VAR config = {"width": 1024, "height": 768, "fps": 60}
VAR player = {"name": "Hero", "health": 100, "level": 5}
VAR mixed = {"string_key": "value", 2: 3, 10: 20}  REM Mixed key types
```

### BASIC-Style Syntax

Uses equals signs, more familiar to BASIC programmers:

```basic
VAR config = {width = 1024, height = 768, fps = 60}
VAR player = {name = "Hero", health = 100, level = 5}
VAR settings = {sound_volume = 0.8, music_enabled = TRUE}
```

### Key Types

Keys can be:
- **Identifiers** (in BASIC-style): `{key = value}` - treated as string "key"
- **Strings** (in JSON-style): `{"key": value}` - explicit string key
- **Numbers**: `{2: 3}` - number key converted to string "2"

```basic
VAR dict1 = {"name": "John", "age": 30}           REM String keys
VAR dict2 = {name = "John", age = 30}            REM Identifier keys (same as string)
VAR dict3 = {2: 3, 10: 20, "text": "hello"}      REM Mixed keys
```

### Accessing Dictionary Values

```basic
VAR dict = {"name": "Alice", "age": 25, "city": "New York"}

REM Bracket notation (always works)
PRINT dict["name"]   REM "Alice"
PRINT dict["age"]    REM 25
PRINT dict["city"]   REM "New York"

REM Dot notation (for valid identifier keys)
REM Note: May not work for all keys, bracket notation is more reliable
```

### Dictionary Operations

```basic
VAR dict = {"a": 1, "b": 2, "c": 3}

REM Add or update values
dict["d"] = 4
dict["a"] = 10

REM Check if key exists
IF Dictionary.has(dict, "a") THEN
    PRINT "Key 'a' exists"
ENDIF

REM Get all keys
VAR keys = Dictionary.keys(dict)  REM ["a", "b", "c", "d"]

REM Get all values
VAR values = Dictionary.values(dict)  REM [10, 2, 3, 4]

REM Get dictionary size
VAR size = Dictionary.size(dict)  REM 4

REM Remove key
dict = Dictionary.remove(dict, "b")

REM Clear dictionary
dict = Dictionary.clear(dict)
```

### Dictionary Methods

```basic
VAR dict1 = {"a": 1, "b": 2}
VAR dict2 = {"c": 3, "d": 4}

REM Merge dictionaries
VAR merged = Dictionary.merge(dict1, dict2)  REM {"a": 1, "b": 2, "c": 3, "d": 4}

REM Get value with default
VAR value = Dictionary.get(dict, "missing", 0)  REM Returns 0 if key doesn't exist
```

### Nested Dictionaries

```basic
VAR config = {
    "window": {"width": 1024, "height": 768},
    "player": {"name": "Hero", "health": 100},
    "settings": {"sound": TRUE, "music": TRUE}
}

PRINT config["window"]["width"]  REM 1024
PRINT config["player"]["name"]   REM "Hero"
```

### Dictionary with Expressions

```basic
VAR x = 10
VAR y = 20

VAR dict = {
    "sum": x + y,
    "product": x * y,
    "message": "Sum is " + STR(x + y)
}

PRINT dict["sum"]      REM 30
PRINT dict["product"]  REM 200
```

### Dictionary Best Practices

```basic
REM Use descriptive keys
VAR player = {name = "Hero", health = 100, score = 0}  REM Good
VAR p = {n = "Hero", h = 100, s = 0}                  REM Bad

REM Use constants for keys
CONST KEY_NAME$ = "name"
CONST KEY_HEALTH$ = "health"
VAR player = {KEY_NAME$: "Hero", KEY_HEALTH$: 100}

REM Use JSON-style for data exchange
VAR json_data = {"id": 123, "status": "active", "data": [1, 2, 3]}

REM Use BASIC-style for configuration
VAR config = {screen_width = 1024, screen_height = 768, fullscreen = FALSE}
```

## Dot Notation

Dot notation allows you to access properties and methods of objects in a clean, intuitive way.

### Basic Dot Notation

```basic
REM Access object properties
VAR pos = Vector2(100, 200)
PRINT pos.x  REM 100
PRINT pos.y  REM 200

pos.x = 150
pos.y = 250

REM Access nested properties
TYPE Player
    position AS Vector2
    health AS INTEGER
END TYPE

VAR player = Player()
player.position.x = 100
player.position.y = 200
player.health = 100
```

### Dot Notation with Types

```basic
TYPE Vector2
    x AS FLOAT
    y AS FLOAT
END TYPE

TYPE Color
    r AS INTEGER
    g AS INTEGER
    b AS INTEGER
    a AS INTEGER
END TYPE

VAR v = Vector2()
v.x = 10.5
v.y = 20.3

VAR c = Color()
c.r = 255
c.g = 100
c.b = 50
c.a = 255
```

### Dot Notation with Arrays

```basic
VAR arr = [1, 2, 3, 4, 5]
VAR len = arr.length  REM Get array length
arr.length = 10       REM Resize array

VAR matrix[5, 5]
matrix[0, 0] = 1
matrix[1, 1] = 2
```

### Dot Notation with Raylib Objects

```basic
REM Vector2 properties
VAR pos = Vector2(100, 200)
pos.x = pos.x + 5
pos.y = pos.y - 10

REM Color properties
VAR color = Color(255, 100, 100, 255)
color.r = 200
color.g = 150
color.b = 100

REM Access nested structures
TYPE Transform
    position AS Vector2
    rotation AS FLOAT
    scale AS Vector2
END TYPE

VAR transform = Transform()
transform.position.x = 100
transform.position.y = 200
transform.rotation = 45.0
transform.scale.x = 1.5
transform.scale.y = 1.5
```

### Case-Insensitive Property Access

Dot notation is case-insensitive for property names:

```basic
VAR pos = Vector2(100, 200)
PRINT pos.x    REM Works
PRINT pos.X    REM Also works
PRINT pos.Xx   REM Also works (case-insensitive)
```

### Deep Property Access

Access properties at any depth:

```basic
TYPE Level
    player AS Player
END TYPE

TYPE Player
    stats AS Stats
END TYPE

TYPE Stats
    health AS INTEGER
    mana AS INTEGER
END TYPE

VAR level = Level()
level.player.stats.health = 100
level.player.stats.mana = 50
```

### Dot Notation Best Practices

```basic
REM Use dot notation for clarity
VAR playerPos = Vector2(400, 300)
playerPos.x = playerPos.x + velocity.x
playerPos.y = playerPos.y + velocity.y

REM Better than:
REM playerPosX = playerPosX + velocityX
REM playerPosY = playerPosY + velocityY

REM Use with constants
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768

VAR center = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
```

---

## State Machine System

The state machine system provides a powerful way to manage game states, AI behavior, and UI flows using intuitive BASIC syntax.

### Basic State Declaration

```basic
STATE Idle
    PRINT "Player is idle"
    TRANSITION TO Walking WHEN isMoving()
END STATE

STATE Walking
    PRINT "Player is walking"
    TRANSITION TO Idle WHEN NOT isMoving()
    TRANSITION TO Running WHEN isRunning()
END STATE

STATE Running
    PRINT "Player is running"
    TRANSITION TO Walking WHEN NOT isRunning()
END STATE
```

### State Transitions

Transitions define when and how to move between states:

```basic
STATE Menu
    TRANSITION TO Playing WHEN isKeyPressed(KEY_SPACE)
END STATE

STATE Playing
    TRANSITION TO Paused WHEN isKeyPressed(KEY_ESCAPE)
    TRANSITION TO GameOver WHEN playerHealth <= 0
END STATE

STATE Paused
    TRANSITION TO Playing WHEN isKeyPressed(KEY_ESCAPE)
    TRANSITION TO Menu WHEN isKeyPressed(KEY_M)
END STATE
```

### State Hooks

Hooks execute code at specific points in a state's lifecycle:

```basic
STATE Jumping
    ON ENTER
        PRINT "Jump started"
        playSound("jump.wav")
        setVelocity(0, -10, 0)
    END ON
    
    ON UPDATE
        REM Update jump physics
        applyGravity()
        checkGroundCollision()
    END ON
    
    ON EXIT
        PRINT "Jump ended"
        stopSound("jump.wav")
    END ON
    
    TRANSITION TO Falling WHEN velocityY > 0
    TRANSITION TO Grounded WHEN isOnGround()
END STATE
```

### State Animations

Bind animations to states:

```basic
STATE Idle
    ANIMATION "idle_anim"
    TRANSITION TO Walking WHEN isMoving()
END STATE

STATE Walking
    ANIMATION "walk_anim", BLEND=0.2
    TRANSITION TO Running WHEN isRunning()
END STATE

STATE Running
    ANIMATION "run_anim", BLEND=0.3
    TRANSITION TO Walking WHEN NOT isRunning()
END STATE
```

### State Wait Times

Enforce minimum duration in a state:

```basic
STATE Attacking
    ANIMATION "attack_anim"
    WAIT 0.5  REM Must stay in state for at least 0.5 seconds
    TRANSITION TO Idle WHEN animationDone()
END STATE

STATE HitStun
    ANIMATION "hit_anim"
    WAIT 0.2  REM Invincibility period
    TRANSITION TO Idle WHEN waitTimeElapsed()
END STATE
```

### Parallel States

Run multiple states concurrently:

```basic
PARALLEL
    STATE Movement
        TRANSITION TO Walking WHEN isMoving()
        TRANSITION TO Idle WHEN NOT isMoving()
    END STATE
    
    STATE Emotion
        TRANSITION TO Happy WHEN health > 50
        TRANSITION TO Sad WHEN health <= 50
    END STATE
    
    STATE Action
        TRANSITION TO Attacking WHEN isAttacking()
        TRANSITION TO Idle WHEN NOT isAttacking()
    END STATE
END PARALLEL
```

### State Groups

Create hierarchical state structures:

```basic
GROUP Movement
    STATE Walking
        ANIMATION "walk_anim"
    END STATE
    
    STATE Running
        ANIMATION "run_anim"
    END STATE
    
    STATE Jumping
        ANIMATION "jump_anim"
    END STATE
END GROUP

GROUP Combat
    STATE Idle
        TRANSITION TO Attacking WHEN attackPressed()
    END STATE
    
    STATE Attacking
        ANIMATION "attack_anim"
        TRANSITION TO Idle WHEN attackComplete()
    END STATE
END GROUP
```

### State System Functions

#### State System Creation

```basic
VAR stateSystem = StateSystem("PlayerStates")
```

#### Attach State System to Entity

```basic
VAR player = Entity.create("Player")
StateSystem.attach(stateSystem, player)
```

#### Update State System

```basic
VAR deltaTime = getDeltaTime()
StateSystem.update(stateSystem, deltaTime)
```

#### Get Current State

```basic
VAR currentState$ = StateSystem.getCurrentState(stateSystem)
PRINT "Current state: " + currentState$
```

#### Force State Transition

```basic
StateSystem.transition(stateSystem, "NewState")
```

### Complete State Machine Example

```basic
REM Player state machine
VAR playerStateSystem = StateSystem("Player")

STATE Idle
    ON ENTER
        PRINT "Entering idle state"
    END ON
    
    ANIMATION "idle_anim"
    TRANSITION TO Walking WHEN isKeyDown(KEY_W) OR isKeyDown(KEY_A) OR isKeyDown(KEY_S) OR isKeyDown(KEY_D)
    TRANSITION TO Jumping WHEN isKeyPressed(KEY_SPACE)
END STATE

STATE Walking
    ON ENTER
        PRINT "Started walking"
    END ON
    
    ON UPDATE
        REM Handle movement
        VAR speed = 5.0
        IF isKeyDown(KEY_W) THEN movePlayer(0, -speed)
        IF isKeyDown(KEY_S) THEN movePlayer(0, speed)
        IF isKeyDown(KEY_A) THEN movePlayer(-speed, 0)
        IF isKeyDown(KEY_D) THEN movePlayer(speed, 0)
    END ON
    
    ANIMATION "walk_anim"
    TRANSITION TO Idle WHEN NOT (isKeyDown(KEY_W) OR isKeyDown(KEY_A) OR isKeyDown(KEY_S) OR isKeyDown(KEY_D))
    TRANSITION TO Running WHEN isKeyDown(KEY_LEFT_SHIFT)
    TRANSITION TO Jumping WHEN isKeyPressed(KEY_SPACE)
END STATE

STATE Running
    ON UPDATE
        VAR speed = 10.0
        IF isKeyDown(KEY_W) THEN movePlayer(0, -speed)
        IF isKeyDown(KEY_S) THEN movePlayer(0, speed)
        IF isKeyDown(KEY_A) THEN movePlayer(-speed, 0)
        IF isKeyDown(KEY_D) THEN movePlayer(speed, 0)
    END ON
    
    ANIMATION "run_anim"
    TRANSITION TO Walking WHEN NOT isKeyDown(KEY_LEFT_SHIFT)
    TRANSITION TO Idle WHEN NOT (isKeyDown(KEY_W) OR isKeyDown(KEY_A) OR isKeyDown(KEY_S) OR isKeyDown(KEY_D))
END STATE

STATE Jumping
    ON ENTER
        setVelocity(0, -15, 0)
        playSound("jump.wav")
    END ON
    
    ON UPDATE
        applyGravity()
        IF isOnGround() THEN
            StateSystem.transition(playerStateSystem, "Idle")
        ENDIF
    END ON
    
    ANIMATION "jump_anim"
    TRANSITION TO Idle WHEN isOnGround()
END STATE

REM Initialize state system
StateSystem.attach(playerStateSystem, playerEntity)
StateSystem.setInitialState(playerStateSystem, "Idle")

REM Update in game loop
WHILE NOT WindowShouldClose()
    VAR delta = getDeltaTime()
    StateSystem.update(playerStateSystem, delta)
    REM ... rest of game loop
WEND
```

---

## ECS (Entity-Component-System)

The Entity-Component-System architecture provides a flexible way to build games by separating data (components) from behavior (systems).

### Core Concepts

**Entities**: Containers that hold components (e.g., "Player", "Enemy", "Bullet")
**Components**: Data containers (e.g., Position, Health, Sprite)
**Systems**: Logic that operates on entities with specific components (e.g., RenderSystem, PhysicsSystem)

### Creating Scenes

Scenes contain and manage entities:

```basic
VAR mainScene = Scene("MainScene")
VAR menuScene = Scene("MenuScene")
VAR gameScene = Scene("GameScene")
```

### Creating Entities

```basic
REM Create entity in scene
VAR player = mainScene.createEntity("Player")
VAR enemy = mainScene.createEntity("Enemy")
VAR bullet = mainScene.createEntity("Bullet")

REM Create entity with parent
VAR weapon = mainScene.createEntity("Weapon", player)
VAR shield = mainScene.createEntity("Shield", player)
```

### Adding Components

```basic
REM Add Transform component
mainScene.addComponent(player, "Transform", {
    "x": 100,
    "y": 200,
    "rotation": 0,
    "scaleX": 1.0,
    "scaleY": 1.0
})

REM Add Sprite component
mainScene.addComponent(player, "Sprite", {
    "texture": "player.png",
    "width": 32,
    "height": 32
})

REM Add Health component
mainScene.addComponent(player, "Health", {
    "current": 100,
    "max": 100
})

REM Add RigidBody component for physics
mainScene.addComponent(player, "RigidBody", {
    "velocityX": 0,
    "velocityY": 0,
    "mass": 1.0
})
```

### Built-in Component Types

#### Transform

Position, rotation, and scale:

```basic
mainScene.addComponent(entity, "Transform", {
    "x": 0,
    "y": 0,
    "rotation": 0,
    "scaleX": 1.0,
    "scaleY": 1.0
})
```

#### Sprite

2D rendering:

```basic
mainScene.addComponent(entity, "Sprite", {
    "texture": "sprite.png",
    "width": 32,
    "height": 32,
    "color": [255, 255, 255, 255]
})
```

#### Model3D

3D rendering:

```basic
mainScene.addComponent(entity, "Model3D", {
    "model": "character.obj",
    "texture": "character.png"
})
```

#### RigidBody

Physics simulation:

```basic
mainScene.addComponent(entity, "RigidBody", {
    "velocityX": 0,
    "velocityY": 0,
    "mass": 1.0,
    "friction": 0.5
})
```

#### Collider

Collision detection:

```basic
mainScene.addComponent(entity, "Collider", {
    "type": "box",
    "width": 32,
    "height": 32
})
```

#### Health

Gameplay health:

```basic
mainScene.addComponent(entity, "Health", {
    "current": 100,
    "max": 100
})
```

#### AI

AI behavior:

```basic
mainScene.addComponent(entity, "AI", {
    "state": "patrol",
    "target": 0
})
```

#### Inventory

Item management:

```basic
mainScene.addComponent(entity, "Inventory", {
    "items": [],
    "maxSize": 10
})
```

#### Animation

Frame-based animation:

```basic
mainScene.addComponent(entity, "Animation", {
    "current": "idle",
    "frame": 0,
    "speed": 0.1
})
```

#### Light

Lighting:

```basic
mainScene.addComponent(entity, "Light", {
    "type": "point",
    "color": [255, 255, 255],
    "intensity": 1.0
})
```

#### AudioSource

Sound:

```basic
mainScene.addComponent(entity, "AudioSource", {
    "sound": "footstep.wav",
    "volume": 1.0,
    "loop": FALSE
})
```

#### Script

Custom logic:

```basic
mainScene.addComponent(entity, "Script", {
    "code": "custom_update_function",
    "data": {}
})
```

### Accessing Components

```basic
REM Get component
VAR transform = mainScene.getComponent(player, "Transform")
VAR x = transform["x"]
VAR y = transform["y"]

REM Check if entity has component
IF mainScene.hasComponent(player, "Health") THEN
    VAR health = mainScene.getComponent(player, "Health")
    PRINT "Health: " + STR(health["current"])
ENDIF

REM Update component data
VAR health = mainScene.getComponent(player, "Health")
health["current"] = health["current"] - 10
mainScene.setComponentData(player, "Health", health)
```

### Querying Entities

```basic
REM Find all entities with Sprite component
VAR sprites = mainScene.query("Sprite")
FOR EACH entity IN sprites
    VAR sprite = mainScene.getComponent(entity, "Sprite")
    REM Draw sprite
NEXT

REM Find entities with multiple components
VAR renderables = mainScene.queryAll("Transform", "Sprite")
FOR EACH entity IN renderables
    VAR transform = mainScene.getComponent(entity, "Transform")
    VAR sprite = mainScene.getComponent(entity, "Sprite")
    REM Render entity
NEXT

REM Find all enemies (entities with Health and AI components)
VAR enemies = mainScene.queryAll("Health", "AI")
FOR EACH enemy IN enemies
    REM Update enemy AI
NEXT
```

### Entity Hierarchy

```basic
REM Create parent entity
VAR player = mainScene.createEntity("Player")

REM Create child entities
VAR weapon = mainScene.createEntity("Weapon", player)
VAR shield = mainScene.createEntity("Shield", player)

REM Set parent
mainScene.setParent(weapon, player)

REM Get children
VAR children = mainScene.getChildren(player)
FOR EACH child IN children
    PRINT "Child: " + child.name
NEXT
```

### Scene Updates

```basic
REM Update scene (updates all entities)
VAR deltaTime = getDeltaTime()
mainScene.update(deltaTime)

REM Draw scene (draws all entities with Sprite/Model3D components)
mainScene.draw()
```

### ECS System Registration

Register custom systems that operate on entities:

```basic
REM Register a rendering system
ECS.registerSystem("RenderSystem", ["Transform", "Sprite"], 0)

REM Register a physics system
ECS.registerSystem("PhysicsSystem", ["Transform", "RigidBody"], 1)

REM Update all systems
VAR deltaTime = getDeltaTime()
ECS.updateSystems(deltaTime)
```

### Complete ECS Example

```basic
REM Create scene
VAR gameScene = Scene("Game")

REM Create player entity
VAR player = gameScene.createEntity("Player")

REM Add components to player
gameScene.addComponent(player, "Transform", {
    "x": 400,
    "y": 300,
    "rotation": 0,
    "scaleX": 1.0,
    "scaleY": 1.0
})

gameScene.addComponent(player, "Sprite", {
    "texture": "player.png",
    "width": 32,
    "height": 32
})

gameScene.addComponent(player, "Health", {
    "current": 100,
    "max": 100
})

gameScene.addComponent(player, "RigidBody", {
    "velocityX": 0,
    "velocityY": 0,
    "mass": 1.0
})

REM Create enemy entities
FOR i = 1 TO 10
    VAR enemy = gameScene.createEntity("Enemy" + STR(i))
    gameScene.addComponent(enemy, "Transform", {
        "x": RANDOM(800),
        "y": RANDOM(600),
        "rotation": 0,
        "scaleX": 1.0,
        "scaleY": 1.0
    })
    gameScene.addComponent(enemy, "Sprite", {
        "texture": "enemy.png",
        "width": 24,
        "height": 24
    })
    gameScene.addComponent(enemy, "Health", {
        "current": 50,
        "max": 50
    })
    gameScene.addComponent(enemy, "AI", {
        "state": "patrol",
        "target": player
    })
NEXT

REM Game loop
WHILE NOT WindowShouldClose()
    VAR delta = getDeltaTime()
    
    REM Update scene
    gameScene.update(delta)
    
    REM Query and process entities
    VAR enemies = gameScene.queryAll("Health", "AI")
    FOR EACH enemy IN enemies
        VAR health = gameScene.getComponent(enemy, "Health")
        IF health["current"] <= 0 THEN
            gameScene.destroyEntity(enemy)
        ENDIF
    NEXT
    
    REM Draw scene
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    gameScene.draw()
    ENDDRAW()
WEND
```

---

## Libraries and Modules

CyberBasic supports code organization through modules and includes.

### IMPORT Statement

Import other BASIC files as modules:

```basic
IMPORT "math_utils.bas"
IMPORT "graphics.bas"
IMPORT "game_objects.bas"

REM Use functions from imported modules
VAR result = math_utils.add(5, 3)
graphics.drawCircle(100, 100, 50)
```

### INCLUDE Statement

Include file contents directly:

```basic
INCLUDE "constants.bas"
INCLUDE "functions.bas"

REM Code from included files is inserted here
REM Can use constants and functions directly
```

### Module Organization

```basic
REM math_utils.bas
FUNCTION add(a, b)
    RETURN a + b
ENDFUNCTION

FUNCTION multiply(a, b)
    RETURN a * b
ENDFUNCTION

FUNCTION divide(a, b)
    IF b = 0 THEN
        RETURN 0
    ENDIF
    RETURN a / b
ENDFUNCTION

REM main.bas
IMPORT "math_utils.bas"

VAR sum = add(10, 5)
VAR product = multiply(4, 3)
VAR quotient = divide(20, 4)
```

### Constants Module

```basic
REM constants.bas
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768
CONST FPS = 60
CONST GRAVITY = 9.8

REM main.bas
INCLUDE "constants.bas"

INITWINDOW(SCREEN_WIDTH, SCREEN_HEIGHT, "Game")
SETTARGETFPS(FPS)
```

### Type Definitions Module

```basic
REM types.bas
TYPE Vector2
    x AS FLOAT
    y AS FLOAT
END TYPE

TYPE Color
    r AS INTEGER
    g AS INTEGER
    b AS INTEGER
    a AS INTEGER
END TYPE

REM main.bas
INCLUDE "types.bas"

VAR pos = Vector2()
pos.x = 100
pos.y = 200
```

### Game Object Module

```basic
REM game_objects.bas
TYPE Player
    position AS Vector2
    health AS INTEGER
    speed AS FLOAT
END TYPE

FUNCTION Player.create()
    VAR p = Player()
    p.position = Vector2(400, 300)
    p.health = 100
    p.speed = 5.0
    RETURN p
ENDFUNCTION

FUNCTION Player.update(player)
    REM Update player logic
ENDFUNCTION

REM main.bas
IMPORT "game_objects.bas"

VAR player = Player.create()
WHILE NOT WindowShouldClose()
    Player.update(player)
WEND
```

### Module Best Practices

1. **Single Responsibility**: Each module should have a clear purpose
2. **Clear Naming**: Use descriptive file names
3. **Documentation**: Include comments explaining module purpose
4. **Avoid Circular Dependencies**: Module A shouldn't import Module B if B imports A
5. **Organize by Feature**: Group related functionality together

---

## Advanced Type System

### Type Inheritance

Types can extend other types:

```basic
TYPE Entity
    x AS FLOAT
    y AS FLOAT
    active AS BOOLEAN
END TYPE

TYPE Player EXTENDS Entity
    health AS INTEGER
    score AS INTEGER
END TYPE

VAR player = Player()
player.x = 100        REM From Entity
player.y = 200        REM From Entity
player.active = TRUE  REM From Entity
player.health = 100   REM From Player
player.score = 0      REM From Player
```

### Type Methods

Define functions that operate on types:

```basic
TYPE Vector2
    x AS FLOAT
    y AS FLOAT
END TYPE

FUNCTION Vector2.length(v AS Vector2)
    RETURN SQRT(v.x * v.x + v.y * v.y)
ENDFUNCTION

FUNCTION Vector2.normalize(v AS Vector2)
    VAR len = Vector2.length(v)
    IF len > 0 THEN
        v.x = v.x / len
        v.y = v.y / len
    ENDIF
    RETURN v
ENDFUNCTION

VAR v = Vector2(3, 4)
VAR len = Vector2.length(v)  REM Returns 5.0
VAR normalized = Vector2.normalize(v)
```

### Type Arrays

Arrays of custom types:

```basic
TYPE Point
    x AS FLOAT
    y AS FLOAT
END TYPE

VAR points[100]
FOR i = 0 TO 99
    points[i] = Point()
    points[i].x = RANDOM(800)
    points[i].y = RANDOM(600)
NEXT
```

### Type JSON Serialization

Types can be serialized to and from JSON:

```basic
TYPE Player
    name AS STRING
    level AS INTEGER
    experience AS INTEGER
END TYPE

VAR player = Player()
player.name = "Hero"
player.level = 5
player.experience = 1250

REM Serialize to JSON
VAR json$ = player.toJSON()
WRITEFILE("player.json", json$)

REM Deserialize from JSON
VAR json_data$ = READFILE("player.json")
VAR loadedPlayer = Player()
loadedPlayer.fromJSON(json_data$)
```

---

## Error Handling and Debugging

### TRY-CATCH Blocks

Handle errors gracefully:

```basic
TRY
    VAR file$ = READFILE("data.txt")
    VAR data = VAL(file$)
    PRINT "Data: " + STR(data)
CATCH error
    PRINT "Error reading file: " + error
ENDTRY
```

### Nested TRY-CATCH

```basic
TRY
    VAR file$ = READFILE("config.json")
    TRY
        VAR config = Config()
        config.fromJSON(file$)
    CATCH parseError
        PRINT "JSON parse error: " + parseError
    ENDTRY
CATCH fileError
    PRINT "File error: " + fileError
ENDTRY
```

### FINALLY Blocks

Execute code regardless of errors:

```basic
VAR file = OPENFILE("data.txt", "r")
TRY
    VAR content$ = READFILE(file)
    PROCESS content$
CATCH error
    PRINT "Error: " + error
FINALLY
    CLOSEFILE(file)  REM Always executed
ENDTRY
```

### Assertions

Check conditions and fail if false:

```basic
VAR value = 10
ASSERT value > 0, "Value must be positive"

VAR index = 5
ASSERT index < array.length, "Index out of bounds"
```

### Debug Output

```basic
DEBUG PRINT "Current state: " + currentState$
DEBUG PRINT "Player position: " + STR(player.x) + ", " + STR(player.y)
```

---

## Performance Optimization

### Array Operations

Use efficient array operations:

```basic
REM Prefer direct access over function calls
VAR len = array.length  REM Fast
REM vs
VAR len = LEN(array)    REM Slightly slower

REM Cache array length in loops
VAR count = array.length
FOR i = 0 TO count - 1
    REM Process array[i]
NEXT
```

### Type Access

Direct property access is fast:

```basic
REM Fast: Direct property access
player.position.x = 100

REM Slower: Function calls
setPlayerPosition(player, 100, 200)
```

### Component Queries

Cache query results:

```basic
REM Query once per frame
VAR enemies = scene.queryAll("Health", "AI")

REM Use cached results
FOR EACH enemy IN enemies
    REM Process enemy
NEXT
```

### State Machine Optimization

```basic
REM Use state transitions efficiently
REM Avoid checking conditions every frame if possible
STATE Idle
    TRANSITION TO Walking WHEN isKeyDown(KEY_W)  REM Only checked on key events
END STATE
```

---

## Complete Examples

### Example 1: Game with ECS and State Machine

```basic
REM Complete game using ECS and state machine
INITWINDOW(1024, 768, "ECS Game")
SETTARGETFPS(60)

REM Create scene
VAR gameScene = Scene("Game")

REM Create player with state machine
VAR player = gameScene.createEntity("Player")
gameScene.addComponent(player, "Transform", {"x": 512, "y": 384})
gameScene.addComponent(player, "Sprite", {"texture": "player.png"})
gameScene.addComponent(player, "Health", {"current": 100, "max": 100})

VAR playerStateSystem = StateSystem("Player")

STATE Idle
    ANIMATION "idle_anim"
    TRANSITION TO Walking WHEN isKeyDown(KEY_W)
END STATE

STATE Walking
    ON UPDATE
        VAR transform = gameScene.getComponent(player, "Transform")
        transform["x"] = transform["x"] + 5
    END ON
    ANIMATION "walk_anim"
    TRANSITION TO Idle WHEN NOT isKeyDown(KEY_W)
END STATE

StateSystem.attach(playerStateSystem, player)

REM Game loop
WHILE NOT WindowShouldClose()
    VAR delta = getDeltaTime()
    
    StateSystem.update(playerStateSystem, delta)
    gameScene.update(delta)
    
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    gameScene.draw()
    ENDDRAW()
WEND
```

### Example 2: Enum-Based Game State

```basic
REM Game state management with enums
ENUM GameState
    Menu, Playing, Paused, GameOver
END ENUM

VAR stateEnum = Enum("GameState", "Menu", "Playing", "Paused", "GameOver")
VAR currentState = Enum.getValue(stateEnum, "Menu")

FUNCTION changeState(newState$)
    currentState = Enum.getValue(stateEnum, newState$)
ENDFUNCTION

FUNCTION isState(stateName$)
    RETURN currentState = Enum.getValue(stateEnum, stateName$)
ENDFUNCTION

WHILE NOT WindowShouldClose()
    IF isState("Menu") THEN
        REM Show menu
        IF isKeyPressed(KEY_SPACE) THEN
            changeState("Playing")
        ENDIF
    ELSEIF isState("Playing") THEN
        REM Game logic
        IF isKeyPressed(KEY_ESCAPE) THEN
            changeState("Paused")
        ENDIF
    ELSEIF isState("Paused") THEN
        REM Pause screen
        IF isKeyPressed(KEY_ESCAPE) THEN
            changeState("Playing")
        ENDIF
    ENDIF
    
    BEGINDRAW()
    CLEARBACKGROUND(0, 0, 0)
    ENDDRAW()
WEND
```

---

## Conclusion

This guide covers the advanced features of CyberBasic:
- **Enums**: Type-safe constants
- **Dot Notation**: Clean object access
- **State Machines**: Powerful state management
- **ECS**: Flexible game architecture
- **Modules**: Code organization
- **Advanced Types**: Inheritance and methods
- **Error Handling**: Robust error management

These features enable you to build complex, maintainable games and applications in CyberBasic.

