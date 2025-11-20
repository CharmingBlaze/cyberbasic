# Modern BASIC-Style State System Guide

## Overview

CyberBasic now includes a **modern BASIC-style state system** that provides intuitive, readable syntax for managing game state machines. The system uses traditional BASIC keywords and block structures while providing powerful state management capabilities.

## Core Syntax

### STATE ... END STATE

Declares a named state block with optional animation, hooks, and transitions.

```basic
STATE Idle
    ANIMATION "idle_anim"
    TRANSITION TO Walking WHEN INPUT("move")
END STATE
```

### TRANSITION

Defines a rule for moving between states.

```basic
TRANSITION Idle TO Walking WHEN INPUT("move")
TRANSITION Walking TO Idle WHEN NOT INPUT("move")
TRANSITION Walking TO Jumping WHEN INPUT("jump") PRIORITY=1
```

### ON ENTER / ON EXIT / ON UPDATE

Event hooks tied to a state's lifecycle.

```basic
STATE Jumping
    ON ENTER
        PLAY SOUND "jump.wav"
        SET VELOCITY 0, -10, 0
    END ON
    
    ON UPDATE
        MOVE PLAYER INPUT("direction"), SPEED=5
    END ON
    
    ON EXIT
        STOP ANIMATION "jump_anim"
    END ON
END STATE
```

### ANIMATION

Binds an animation to the state.

```basic
ANIMATION "run_anim", BLEND=0.2
```

### WAIT

Forces a minimum duration before leaving a state.

```basic
STATE Attacking
    ANIMATION "attack_anim"
    WAIT 0.3
    TRANSITION TO Idle WHEN ANIMATION DONE
END STATE
```

### PARALLEL ... END PARALLEL

Allows multiple states to run concurrently.

```basic
PARALLEL
    STATE Movement
        TRANSITION TO Walking WHEN INPUT("move")
    END STATE
    
    STATE Emotion
        TRANSITION TO Happy WHEN HEALTH > 50
    END STATE
END PARALLEL
```

### GROUP ... END GROUP

Creates hierarchical states.

```basic
GROUP Movement
    STATE Walking
        ANIMATION "walk_anim"
    END STATE
    
    STATE Running
        ANIMATION "run_anim"
    END STATE
END GROUP
```

### EVENT

Declares a trigger that can be listened to.

```basic
EVENT EnemySpotted
TRANSITION Idle TO Alert WHEN EVENT("EnemySpotted")
```

## State System Management

### DEFINE STATE SYSTEM ... END SYSTEM

Declares a custom state system container.

```basic
DEFINE STATE SYSTEM PlayerSystem
    STATE Idle
        ANIMATION "idle_anim"
    END STATE
    
    STATE Walking
        ANIMATION "walk_anim"
    END STATE
END SYSTEM
```

### ATTACH SYSTEM

Assigns a state system to an entity.

```basic
VAR player = scene.createEntity("Player")
ATTACH SYSTEM PlayerSystem TO player
```

### ADD STATE

Dynamically adds a new state to an existing system.

```basic
ADD STATE "Crouching" TO PlayerSystem
    ANIMATION "crouch_anim"
    TRANSITION TO Idle WHEN NOT INPUT("crouch")
END STATE
```

### REMOVE STATE

Removes a state from a system.

```basic
REMOVE STATE "Crouching" FROM PlayerSystem
```

### OVERRIDE STATE

Redefines behavior of a state without rewriting the whole system.

```basic
OVERRIDE STATE "Walking"
    ANIMATION "fast_walk_anim"
    SPEED = 6
END STATE
```

### SET TRANSITION RULE

Tweaks transition conditions globally.

```basic
SET TRANSITION RULE Idle TO Walking PRIORITY=1
```

### ENABLE / DISABLE STATE

Temporarily toggle states for debugging or special modes.

```basic
DISABLE STATE "Attacking"
ENABLE STATE "Jumping"
```

### DEBUG STATES

Prints or visualizes the current state system.

```basic
DEBUG STATES PlayerSystem
```

### EXPORT / IMPORT SYSTEM

Allows sharing or reusing state systems across projects.

```basic
EXPORT SYSTEM PlayerSystem TO "player_states.json"
IMPORT SYSTEM EnemySystem FROM "enemy_states.json"
```

## Complete Example

```basic
// Define player state system
DEFINE STATE SYSTEM PlayerSystem
    STATE Idle
        ANIMATION "idle_anim"
        TRANSITION TO Walking WHEN INPUT("move")
        TRANSITION TO Jumping WHEN INPUT("jump")
    END STATE
    
    STATE Walking
        ANIMATION "walk_anim"
        ON UPDATE
            MOVE PLAYER INPUT("direction"), SPEED=3
        END ON
        TRANSITION TO Idle WHEN NOT INPUT("move")
        TRANSITION TO Jumping WHEN INPUT("jump")
    END STATE
    
    STATE Jumping
        ANIMATION "jump_anim"
        ON ENTER
            PLAY SOUND "jump.wav"
            SET VELOCITY 0, -10, 0
        END ON
        ON UPDATE
            MOVE PLAYER INPUT("direction"), SPEED=5
        END ON
        WAIT 0.5
        TRANSITION TO Idle WHEN GROUNDED()
    END STATE
    
    STATE Attacking
        ANIMATION "attack_anim"
        ON ENTER
            PLAY SOUND "attack.wav"
        END ON
        WAIT 0.3
        TRANSITION TO Idle WHEN ANIMATION DONE
    END STATE
END SYSTEM

// Create player entity
VAR scene = Scene("Game")
VAR player = scene.createEntity("Player")
player.addComponent("Transform")
player.addComponent("Sprite", {textureId = 1})

// Attach state system to player
ATTACH SYSTEM PlayerSystem TO player

// Game loop
WHILE NOT GAME.shouldClose()
    GAME.beginFrame()
    VAR delta = Clock.getDelta()
    
    // Update state system
    UPDATE_STATE_SYSTEM("PlayerSystem", delta)
    
    // Update scene
    scene.update(delta)
    
    // Draw scene
    scene.draw()
    
    GAME.endFrame()
WEND
```

## Advanced Features

### Parallel States

```basic
DEFINE STATE SYSTEM ComplexSystem
    PARALLEL
        STATE Movement
            TRANSITION TO Walking WHEN INPUT("move")
        END STATE
        
        STATE Combat
            TRANSITION TO Attacking WHEN INPUT("attack")
        END STATE
    END PARALLEL
END SYSTEM
```

### State Groups

```basic
DEFINE STATE SYSTEM MovementSystem
    GROUP GroundMovement
        STATE Walking
        STATE Running
        STATE Crouching
    END GROUP
    
    GROUP AirMovement
        STATE Jumping
        STATE Falling
    END GROUP
END SYSTEM
```

### Dynamic State Management

```basic
// Add state at runtime
ADD STATE "Dashing" TO PlayerSystem
    ANIMATION "dash_anim"
    TRANSITION TO Idle WHEN ANIMATION DONE
END STATE

// Override existing state
OVERRIDE STATE "Walking"
    SPEED = 8
    ANIMATION "sprint_anim"
END STATE

// Disable state temporarily
DISABLE STATE "Attacking"

// Re-enable
ENABLE STATE "Attacking"
```

### Event-Driven Transitions

```basic
EVENT EnemySpotted
EVENT LowHealth
EVENT PowerUpCollected

DEFINE STATE SYSTEM AISystem
    STATE Patrol
        TRANSITION TO Alert WHEN EVENT("EnemySpotted")
    END STATE
    
    STATE Alert
        TRANSITION TO Flee WHEN EVENT("LowHealth")
    END STATE
    
    STATE Flee
        TRANSITION TO Patrol WHEN NOT EVENT("EnemySpotted")
    END STATE
END SYSTEM
```

## Best Practices

1. **Use meaningful state names** - `Idle`, `Walking`, `Jumping` are better than `State1`, `State2`
2. **Keep transitions simple** - Complex conditions should be in functions
3. **Use WAIT for animation timing** - Prevents state changes during animations
4. **Group related states** - Use GROUPs for organization
5. **Debug frequently** - Use `DEBUG STATES` to visualize state flow
6. **Export reusable systems** - Save common state systems for reuse

## Integration with ECS

The state system integrates seamlessly with the ECS system:

```basic
VAR player = scene.createEntity("Player")
player.addComponent("Transform")
player.addComponent("Sprite")
player.addComponent("Health", {maxHealth = 100, currentHealth = 100})

ATTACH SYSTEM PlayerSystem TO player

// State system can access entity components
STATE LowHealth
    ON ENTER
        VAR health = player.getComponent("Health")
        IF health.currentHealth < 20 THEN
            PLAY SOUND "low_health.wav"
        END IF
    END ON
END STATE
```

## Performance Considerations

- State transitions are O(n) where n is number of transitions
- Use PRIORITY to optimize transition checking
- Disable unused states to reduce overhead
- Parallel states run concurrently but share update time

## Future Enhancements

Planned features:
- State history/undo
- State blending
- State machine visualization
- Hot-reload state systems
- State machine templates
- Conditional state groups

