# CyberBasic Game Programming Guide

This guide covers all the essential game programming systems available in CyberBasic, designed to give you everything you need to create modern games.

## Table of Contents

1. [System Overview](#system-overview)
2. [Game Systems Management](#game-systems-management)
3. [Game Object System](#game-object-system)
4. [Scene Management](#scene-management)
5. [Game State Management](#game-state-management)
6. [Graphics System](#graphics-system)
7. [Audio System](#audio-system)
8. [Input System](#input-system)
9. [Physics System](#physics-system)
10. [AI System](#ai-system)
11. [Navigation System](#navigation-system)
12. [Networking System](#networking-system)
13. [Collision Detection](#collision-detection)
14. [Game Loop Helpers](#game-loop-helpers)
15. [Utility Functions](#utility-functions)
16. [Complete Example](#complete-example)

## System Overview

CyberBasic provides a comprehensive set of game programming systems:

- **Graphics**: 2D/3D rendering, textures, models, shaders
- **Audio**: Sound effects, music, 3D audio
- **Input**: Keyboard, mouse, gamepad support
- **Physics**: Collision detection, rigid body physics, joints
- **AI**: Pathfinding, behavior trees, decision making
- **Navigation**: Waypoint systems, pathfinding algorithms
- **Networking**: Multiplayer support, client-server architecture
- **Game Objects**: Entity management, component system
- **Scenes**: Level management, object organization
- **Game State**: State machines, data persistence

## Game Systems Management

### Initialize All Systems
```basic
INITGAMESYSTEMS()
```
Initializes all game programming systems (graphics, audio, input, physics, AI, navigation, networking).

### Check System Status
```basic
LET graphics_ready = ISSYSTEMINITIALIZED("graphics")
LET audio_ready = ISSYSTEMINITIALIZED("audio")
LET physics_ready = ISSYSTEMINITIALIZED("physics")
```

### Shutdown Systems
```basic
SHUTDOWNGAMESYSTEMS()
```
Properly shuts down all game systems and cleans up resources.

## Game Object System

### Create Game Objects
```basic
LET player_id = CREATEGAMEOBJECT("Player")
LET enemy_id = CREATEGAMEOBJECT("Enemy")
LET collectible_id = CREATEGAMEOBJECT("Collectible")
```

### Position Management
```basic
REM Set position (x, y, z)
SETGAMEOBJECTPOSITION(player_id, 100, 200, 0)

REM Get position
LET pos = GETGAMEOBJECTPOSITION(player_id)
LET x = pos(0)
LET y = pos(1)
LET z = pos(2)
```

### Rotation Management
```basic
REM Set rotation (x, y, z in radians)
SETGAMEOBJECTROTATION(player_id, 0, 0, 1.57)

REM Get rotation
LET rot = GETGAMEOBJECTROTATION(player_id)
```

### Scale Management
```basic
REM Set scale (x, y, z)
SETGAMEOBJECTSCALE(player_id, 2.0, 2.0, 1.0)

REM Get scale
LET scale = GETGAMEOBJECTSCALE(player_id)
```

### Visibility and Activity
```basic
REM Control visibility
SETGAMEOBJECTVISIBLE(player_id, TRUE)
SETGAMEOBJECTVISIBLE(player_id, FALSE)

REM Control activity
SETGAMEOBJECTACTIVE(player_id, TRUE)
SETGAMEOBJECTACTIVE(player_id, FALSE)
```

### Object Management
```basic
REM Get total object count
LET object_count = GETGAMEOBJECTCOUNT()

REM Destroy object
DESTROYGAMEOBJECT(player_id)
```

## Scene Management

### Create and Load Scenes
```basic
REM Create a new scene
CREATESCENE("MainMenu")
CREATESCENE("Level1")
CREATESCENE("GameOver")

REM Load a scene
LOADSCENE("Level1")
```

### Manage Scene Objects
```basic
REM Add objects to scene
ADDOBJECTTOSCENE("Level1", player_id)
ADDOBJECTTOSCENE("Level1", enemy_id)

REM Remove objects from scene
REMOVEOBJECTFROMSCENE("Level1", enemy_id)

REM Get current scene
LET current_scene = GETCURRENTSCENE()
```

## Game State Management

### State Control
```basic
REM Set game state
SETGAMESTATE("playing")
SETGAMESTATE("paused")
SETGAMESTATE("gameover")

REM Get current state
LET state = GETGAMESTATE()
```

### State Data
```basic
REM Store data in game state
SETGAMESTATEDATA("score", 1000)
SETGAMESTATEDATA("level", 5)
SETGAMESTATEDATA("player_name", "Hero")

REM Retrieve data
LET score = GETGAMESTATEDATA("score")
LET level = GETGAMESTATEDATA("level")
LET name = GETGAMESTATEDATA("player_name")
```

### Game Loop Updates
```basic
REM Update game state (call each frame)
GAMELOOPUPDATE()

REM Get frame information
LET frame_count = GETFRAMECOUNT()
LET delta_time = GETDELTATIME()
```

## Graphics System

### Window Management
```basic
REM Create window
INITWINDOW(1024, 768, "My Game")

REM Window properties
SETWINDOWTITLE("New Title")
SETWINDOWSIZE(800, 600)
TOGGLEFULLSCREEN()

REM Screen information
LET width = GETSCREENWIDTH()
LET height = GETSCREENHEIGHT()
```

### Drawing Functions
```basic
REM Basic shapes
DRAWCIRCLE(100, 100, 50, 255, 0, 0)
DRAWRECTANGLE(200, 200, 100, 100, 0, 255, 0)
DRAWLINE(0, 0, 100, 100, 255, 255, 255)

REM Text
DRAWTEXT("Hello World", 10, 10, 20, 255, 255, 255)

REM Clear screen
CLEARBACKGROUND(0, 0, 0)
```

### 3D Graphics
```basic
REM 3D shapes
DRAWCUBE(0, 0, 0, 2, 2, 2, 255, 0, 0)
DRAWSPHERE(0, 0, 0, 1, 0, 255, 0)

REM 3D models (requires model loading)
DRAWMODEL(model_id, 0, 0, 0, 1, 255, 255, 255)
```

## Audio System

### Sound Effects
```basic
REM Load and play sounds
LET sound_id = LOADSOUND("explosion.wav")
PLAYSOUND(sound_id)
STOPSOUND(sound_id)
UNLOADSOUND(sound_id)
```

### Music
```basic
REM Load and play music
LET music_id = LOADMUSIC("background.mp3")
PLAYMUSIC(music_id)
PAUSEMUSIC()
RESUMEMUSIC()
STOPMUSIC()
UNLOADMUSIC(music_id)
```

### Audio Control
```basic
REM Volume control
SETSOUNDVOLUME(sound_id, 0.5)
SETMUSICVOLUME(music_id, 0.7)

REM 3D audio
SETSOUNDPOSITION(sound_id, x, y, z)
```

## Input System

### Keyboard Input
```basic
REM Key states
IF ISKEYDOWN(KEY_W) THEN
    REM Move forward
ENDIF

IF ISKEYPRESSED(KEY_SPACE) THEN
    REM Jump (once per press)
ENDIF

IF ISKEYRELEASED(KEY_ESC) THEN
    REM Exit game
ENDIF
```

### Mouse Input
```basic
REM Mouse position
LET mouse_x = GETMOUSEX()
LET mouse_y = GETMOUSEY()

REM Mouse buttons
IF ISMOUSEBUTTONDOWN(MOUSE_BUTTON_LEFT) THEN
    REM Left mouse button held
ENDIF

IF ISMOUSEBUTTONPRESSED(MOUSE_BUTTON_RIGHT) THEN
    REM Right mouse button clicked
ENDIF

REM Mouse wheel
LET wheel_delta = GETMOUSEWHEELMOVE()
```

### Gamepad Input
```basic
REM Gamepad detection
LET gamepad_count = GETGAMEPADCOUNT()
LET gamepad_name = GETGAMEPADNAME(0)

REM Gamepad buttons
IF ISGAMEPADBUTTONDOWN(0, GAMEPAD_BUTTON_A) THEN
    REM A button held
ENDIF

REM Gamepad axes
LET left_x = GETGAMEPADAXIS(0, GAMEPAD_AXIS_LEFT_X)
LET left_y = GETGAMEPADAXIS(0, GAMEPAD_AXIS_LEFT_Y)
```

## Physics System

### Physics World
```basic
REM Initialize physics
INITPHYSICS()

REM Set gravity
SETPHYSICSGRAVITY(0, 9.81)

REM Physics step
PHYSICSSTEP()
```

### Physics Bodies
```basic
REM Create bodies
LET static_body = CREATEPHYSICSBODY(0, 0, 0)  REM Static
LET dynamic_body = CREATEPHYSICSBODY(1, 100, 100)  REM Dynamic

REM Set shapes
SETPHYSICSCIRCLESHAPE(dynamic_body, 25)
SETPHYSICSRECTANGLESHAPE(static_body, 100, 20)

REM Apply forces
APPLYPHYSICSFORCE(dynamic_body, 100, 0)
APPLYPHYSICSIMPULSE(dynamic_body, 0, -200)
```

### Collision Detection
```basic
REM Check collision
IF CHECKPHYSICSCOLLISION(body1, body2) THEN
    REM Handle collision
ENDIF
```

## AI System

### Pathfinding
```basic
REM Create pathfinding grid
LET grid_id = CREATEPATHFINDINGGRID(100, 100)

REM Find path
LET path = FINDPATH(grid_id, start_x, start_y, end_x, end_y)

REM Follow path
FOLLOWPATH(entity_id, path)
```

### Behavior Trees
```basic
REM Create behavior tree
LET tree_id = CREATEBEHAVIORTREE()

REM Add behaviors
ADDSEQUENCE(tree_id, "patrol")
ADDACTION(tree_id, "move_to_waypoint")
ADDACTION(tree_id, "wait")

REM Execute tree
EXECUTEBEHAVIORTREE(tree_id, entity_id)
```

## Navigation System

### Waypoints
```basic
REM Create waypoint system
LET waypoint_id = CREATEWAYPOINTSYSTEM()

REM Add waypoints
ADDWAYPOINT(waypoint_id, 100, 100)
ADDWAYPOINT(waypoint_id, 200, 200)
ADDWAYPOINT(waypoint_id, 300, 100)

REM Navigate between waypoints
NAVIGATETOWAYPOINT(entity_id, waypoint_id, 0)
```

## Networking System

### Client-Server
```basic
REM Start server
LET server_id = STARTSERVER(7777)

REM Connect to server
LET client_id = CONNECTTOSERVER("localhost", 7777)

REM Send data
SENDDATA(client_id, "Hello Server")

REM Receive data
LET data = RECEIVEDATA(client_id)
```

## Collision Detection

### Basic Collision
```basic
REM Check collision between game objects
IF CHECKCOLLISION(object1_id, object2_id) THEN
    REM Handle collision
ENDIF
```

### Physics Collision
```basic
REM Check physics body collision
IF CHECKPHYSICSCOLLISION(body1_id, body2_id) THEN
    REM Handle physics collision
ENDIF
```

## Game Loop Helpers

### Standard Game Loop
```basic
WHILE NOT WINDOWSHOULDCLOSE()
    GAMELOOPSTART()
    
    REM Update game logic
    GAMELOOPUPDATE()
    
    REM Handle input
    REM Update objects
    REM Check collisions
    REM Render graphics
    
    GAMELOOPEND()
WEND
```

## Utility Functions

### Math Utilities
```basic
REM Distance calculation
LET dist = DISTANCE(x1, y1, x2, y2)

REM Angle calculation
LET angle = ANGLE(x1, y1, x2, y2)

REM Linear interpolation
LET result = LERP(start, end, 0.5)

REM Clamp values
LET clamped = CLAMP(value, min, max)
```

### Time Utilities
```basic
REM Frame timing
LET fps = GETFPS()
LET frame_time = GETFRAMETIME()
LET total_time = GETTIME()

REM Frame counting
LET frames = GETFRAMECOUNT()
```

## Complete Example

Here's a complete game example that demonstrates all systems:

```basic
REM Complete Game Example
INITGAMESYSTEMS()
INITWINDOW(1024, 768, "Complete Game Demo")
SETTARGETFPS(60)

REM Create game objects
LET player_id = CREATEGAMEOBJECT("Player")
LET enemy_id = CREATEGAMEOBJECT("Enemy")

REM Set up objects
SETGAMEOBJECTPOSITION(player_id, 100, 100, 0)
SETGAMEOBJECTPOSITION(enemy_id, 500, 300, 0)

REM Create scene
CREATESCENE("MainGame")
ADDOBJECTTOSCENE("MainGame", player_id)
ADDOBJECTTOSCENE("MainGame", enemy_id)
LOADSCENE("MainGame")

REM Set game state
SETGAMESTATE("playing")
SETGAMESTATEDATA("score", 0)

REM Main game loop
WHILE NOT WINDOWSHOULDCLOSE()
    GAMELOOPSTART()
    GAMELOOPUPDATE()
    
    REM Handle input
    LET pos = GETGAMEOBJECTPOSITION(player_id)
    IF ISKEYDOWN(KEY_W) THEN
        SETGAMEOBJECTPOSITION(player_id, pos(0), pos(1) - 200 * GETDELTATIME(), 0)
    ENDIF
    
    REM Check collisions
    IF CHECKCOLLISION(player_id, enemy_id) THEN
        LET score = GETGAMESTATEDATA("score")
        SETGAMESTATEDATA("score", score + 10)
    ENDIF
    
    REM Render
    CLEARBACKGROUND(0, 0, 50)
    LET player_pos = GETGAMEOBJECTPOSITION(player_id)
    DRAWCIRCLE(INT(player_pos(0)), INT(player_pos(1)), 25, 0, 255, 0)
    
    LET enemy_pos = GETGAMEOBJECTPOSITION(enemy_id)
    DRAWCIRCLE(INT(enemy_pos(0)), INT(enemy_pos(1)), 20, 255, 0, 0)
    
    DRAWTEXT("Score: " + STR(GETGAMESTATEDATA("score")), 10, 10, 20, 255, 255, 255)
    
    GAMELOOPEND()
WEND

SHUTDOWNGAMESYSTEMS()
CLOSEWINDOW()
```

## Best Practices

1. **Always initialize systems** before using them
2. **Use game objects** for all entities in your game
3. **Organize with scenes** for different levels/menus
4. **Manage game state** properly for save/load functionality
5. **Handle input** in a centralized way
6. **Use physics** for realistic movement and collisions
7. **Implement proper game loops** with consistent timing
8. **Clean up resources** when shutting down

## Performance Tips

1. **Batch similar operations** together
2. **Use object pooling** for frequently created/destroyed objects
3. **Optimize collision detection** with spatial partitioning
4. **Limit physics updates** to necessary objects
5. **Use appropriate data structures** for your game's needs
6. **Profile your game** to identify bottlenecks

This comprehensive system gives you everything needed to create modern games in CyberBasic!
