# CyberBasic Game Development Guide

**Complete guide to making games with CyberBasic and Raylib**

## Table of Contents
1. [Quick Start](#quick-start)
2. [Core Concepts](#core-concepts)
3. [2D Graphics](#2d-graphics)
4. [3D Graphics](#3d-graphics)
5. [Game Systems](#game-systems)
6. [Best Practices](#best-practices)
7. [Examples](#examples)

## Related Guides
- **[2D Graphics API Guide](2D_GRAPHICS_GUIDE.md)** - Complete 2D rendering reference
- **[3D Graphics API Guide](3D_GRAPHICS_GUIDE.md)** - Complete 3D rendering reference
- **[Getting Started Guide](GETTING_STARTED.md)** - Installation and setup
- **[Distribution Guide](DISTRIBUTION_GUIDE.md)** - How to distribute your games

---

## Quick Start

### Your First Game (30 seconds)

```basic
REM Initialize window
INITWINDOW(800, 600, "My First Game")
SETWINDOWICON("icon.png")  REM Set window icon (optional)
SETTARGETFPS(60)

REM Game variables
VAR x = 400
VAR y = 300
VAR speed = 5

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Input
    IF ISKEYDOWN(KEY_W) THEN y = y - speed
    IF ISKEYDOWN(KEY_S) THEN y = y + speed
    IF ISKEYDOWN(KEY_A) THEN x = x - speed
    IF ISKEYDOWN(KEY_D) THEN x = x + speed
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    DRAWCIRCLE(x, y, 30, 255, 100, 100)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

---

## Core Concepts

### 1. Variables and Constants

**Modern BASIC supports:**
- `VAR` - Modern variable declaration (can be used anywhere)
- `LET` - Classic variable declaration
- `CONST` - Immutable constants

```basic
REM Constants for game configuration
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768
CONST PLAYER_SPEED = 5.0

REM Game state variables
VAR score = 0
VAR lives = 3
VAR gameRunning = TRUE
```

### 2. Dot Notation

Access object properties and methods:

```basic
REM Create objects
VAR player = Vector2(400, 300)
VAR color = Color(255, 100, 100, 255)

REM Access properties
player.x = player.x + 5
PRINT "Player X: " + STR(player.x)
PRINT "Color R: " + STR(color.r)
```

### 3. Arrays

**Declaration:**
```basic
DIM scores[10]           REM Classic style
VAR enemies[5]           REM Modern style
VAR matrix[5, 10]        REM 2D array
```

**Advanced Operations:**
```basic
VAR arr = [1, 2, 3, 4, 5]

REM Array methods
arr.insert(6)           REM Add element
arr.remove()            REM Remove last
arr.sort()              REM Sort ascending
VAR index = arr.find(3) REM Binary search (returns 2)
arr.reverse()           REM Reverse array
arr.swap(0, 4)          REM Swap elements

REM Array properties
VAR len = arr.length     REM Get length
arr.length = 10         REM Resize array
```

**JSON Support:**
```basic
VAR json = arr.toJSON()           REM Convert to JSON string
arr.fromJSON("[1,2,3,4,5]")      REM Load from JSON
arr.save("data.json")            REM Save to file
arr.load("data.json")            REM Load from file
```

### 4. User-Defined Types

Create custom data structures:

```basic
TYPE Player
    x AS FLOAT
    y AS FLOAT
    health AS INTEGER
    name AS STRING
    inventory AS INTEGER[10]
END TYPE

VAR player = Player()
player.x = 100
player.y = 200
player.health = 100
player.name = "Hero"
player.inventory[0] = 1
```

### 5. Functions

**Simple Function:**
```basic
FUNCTION add(a, b)
    RETURN a + b
ENDFUNCTION

VAR result = add(5, 3)
PRINT result  REM Prints 8
```

**Function with Return Value:**
```basic
FUNCTION half(value)
    value = value / 2
ENDFUNCTION value  REM Return value directly

VAR result = half(100)
PRINT result  REM Prints 50
```

**Function as Command:**
```basic
FUNCTION printDecorated(text$)
    PRINTC("*** ")
    PRINTC(text$)
    PRINT(" ***")
ENDFUNCTION

printDecorated("Hello")  REM Prints: *** Hello ***
```

---

## Window Management

CyberBasic provides complete control over your game window, including size, position, title, icon, and state.

### Basic Window Operations

```basic
REM Create window
INITWINDOW(800, 600, "My Game")

REM Set window icon (loads from image file)
SETWINDOWICON("icon.png")  REM Supports PNG, JPG, etc.

REM Set window title
SETWINDOWTITLE("New Title")

REM Set window position on screen
SETWINDOWPOSITION(100, 100)

REM Resize window
SETWINDOWSIZE(1024, 768)

REM Set minimum and maximum window size
SETWINDOWMINIMUMSIZE(400, 300)
SETWINDOWMAXSIZE(1920, 1080)

REM Set window opacity (0.0 = transparent, 1.0 = opaque)
SETWINDOWOPACITY(0.9)
```

### Window State Control

```basic
REM Maximize, minimize, or restore window
MAXIMIZEWINDOW()
MINIMIZEWINDOW()
RESTOREWINDOW()

REM Toggle fullscreen or borderless mode
TOGGLEFULLSCREEN()
TOGGLEBORDERLESSWINDOWED()

REM Check window state
IF ISWINDOWMAXIMIZED() THEN PRINT "Window is maximized"
IF ISWINDOWMINIMIZED() THEN PRINT "Window is minimized"
IF ISWINDOWFOCUSED() THEN PRINT "Window has focus"
IF ISWINDOWRESIZED() THEN PRINT "Window was resized"
```

### Window Icon

The `SETWINDOWICON()` function loads an image from a file path and sets it as the window icon. This is useful for branding your game.

```basic
REM Set window icon (must be called after INITWINDOW)
INITWINDOW(800, 600, "My Game")
SETWINDOWICON("icon.png")  REM Loads and sets icon

REM The function:
REM 1. Loads the image from the file path
REM 2. Sets it as the window icon
REM 3. Automatically unloads the image (memory managed)
```

**Supported Image Formats:**
- PNG (recommended for icons with transparency)
- JPG/JPEG
- BMP
- Other formats supported by Raylib

**Best Practices:**
- Use square icons (e.g., 32x32, 64x64, 128x128 pixels)
- PNG format works best for icons with transparency
- Place icon file in the same directory as your game or use full path

### Complete Window Example

```basic
REM Complete window setup example
INITWINDOW(1024, 768, "My Awesome Game")
SETWINDOWICON("game_icon.png")
SETWINDOWPOSITION(100, 100)
SETWINDOWMINIMUMSIZE(640, 480)
SETTARGETFPS(60)

WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle window state changes
    IF ISKEYPRESSED(KEY_F11) THEN TOGGLEFULLSCREEN()
    IF ISKEYPRESSED(KEY_M) THEN MAXIMIZEWINDOW()
    
    REM Game loop
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    DRAWTEXT("Press F11 for fullscreen, M to maximize", 10, 10, 20, 255, 255, 255)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

## 2D Graphics

For complete 2D graphics documentation, see the **[2D Graphics API Guide](2D_GRAPHICS_GUIDE.md)**.

### Quick 2D Example

```basic
REM 2D Graphics Example
INITWINDOW(800, 600, "2D Game")
SETTARGETFPS(60)

VAR playerTexture = LOADTEXTURE("player.png")

WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(30, 30, 50)
    DRAWTEXTURE(playerTexture, 400, 300, 255, 255, 255)
    ENDDRAW()
WEND

UNLOADTEXTURE(playerTexture)
CLOSEWINDOW()
```

### Key 2D Functions

- **Primitives**: `DRAWRECTANGLE()`, `DRAWCIRCLE()`, `DRAWLINE()`, `DRAWTRIANGLE()`
- **Textures**: `LOADTEXTURE()`, `DRAWTEXTURE()`, `DRAWTEXTUREEX()`
- **Text**: `DRAWTEXT()`, `LOADFONT()`, `DRAWTEXTEX()`
- **Camera**: `BEGINDRAW2D()`, `ENDDRAW2D()`

See **[2D Graphics API Guide](2D_GRAPHICS_GUIDE.md)** for complete reference.

## 3D Graphics

For complete 3D graphics documentation, see the **[3D Graphics API Guide](3D_GRAPHICS_GUIDE.md)**.

### Quick 3D Example

```basic
REM 3D Graphics Example
INITWINDOW(800, 600, "3D Game")
SETTARGETFPS(60)

REM Setup 3D camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

VAR model = LOADMODEL("character.obj")

WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(50, 50, 60)
    
    BEGINDRAW3D(camera)
        DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
        DRAWCUBE(VECTOR3(5, 1, 5), 1, 1, 1, RED)
        DRAWSPHERE(VECTOR3(-5, 1, -5), 1, BLUE)
    ENDDRAW3D()
    
    ENDDRAW()
WEND

UNLOADMODEL(model)
CLOSEWINDOW()
```

### Key 3D Functions

- **Primitives**: `DRAWCUBE()`, `DRAWSPHERE()`, `DRAWPLANE()`, `DRAWGRID()`
- **Models**: `LOADMODEL()`, `DRAWMODEL()`, `DRAWMODELEX()`
- **Camera**: `CAMERA3D()`, `BEGINDRAW3D()`, `ENDDRAW3D()`
- **Lighting**: `ENABLELIGHTING()`, `SETAMBIENTLIGHT()`
- **Materials**: `SETMATERIALTEXTURE()`, `SETMATERIALCOLOR()`

See **[3D Graphics API Guide](3D_GRAPHICS_GUIDE.md)** for complete reference.

## Game Systems

### 1. ECS (Entity-Component-System)

**Create Entities:**
```basic
VAR player = ENTITY_CREATE()
ENTITY_SETPOSITION(player, 400, 300)
ENTITY_ADDCOMPONENT(player, "health", 100)
ENTITY_ADDCOMPONENT(player, "speed", 5.0)
```

**Query Components:**
```basic
VAR health = ENTITY_GETCOMPONENT(player, "health")
VAR speed = ENTITY_GETCOMPONENT(player, "speed")
```

### 2. State Machine

**Define States:**
```basic
STATE Menu
    PRINT "Press SPACE to start"
    IF ISKEYPRESSED(KEY_SPACE) THEN
        TRANSITION TO Game
    ENDIF
END STATE

STATE Game
    REM Game logic here
    IF ISKEYPRESSED(KEY_ESCAPE) THEN
        TRANSITION TO Menu
    ENDIF
END STATE
```

### 3. Sprite System

```basic
VAR sprite = SPRITE_CREATE("player.png", 32, 32)
SPRITE_SETPOSITION(sprite, 400, 300)
SPRITE_DRAW(sprite)
```

### 4. Animation System

```basic
VAR anim = ANIMATION_CREATE("walk", 4, 0.1)
ANIMATION_PLAY(anim)
ANIMATION_UPDATE(anim, DELTATIME())
```

### 5. Collision Detection

```basic
IF COLLISION_CHECK(player, enemy) THEN
    REM Handle collision
    lives = lives - 1
ENDIF
```

### 6. Camera System

```basic
VAR camera = CAMERA_CREATE()
CAMERA_SETTARGET(camera, player.x, player.y)
CAMERA_UPDATE(camera)
CAMERA_BEGIN(camera)
REM Draw world
CAMERA_END()
```

---

## Best Practices

### 1. Use CONST for Configuration
```basic
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768
CONST FPS = 60
CONST GRAVITY = 9.8
```

### 2. Use VAR for Game State
```basic
VAR score = 0
VAR lives = 3
VAR gameOver = FALSE
```

### 3. Use Dot Notation for Objects
```basic
VAR pos = Vector2(100, 200)
VAR vel = Vector2(5, 0)
pos.x = pos.x + vel.x
pos.y = pos.y + vel.y
```

### 4. Organize with Functions
```basic
FUNCTION updatePlayer()
    REM Player update logic
ENDFUNCTION

FUNCTION drawPlayer()
    REM Player drawing logic
ENDFUNCTION

WHILE NOT WINDOWSHOULDCLOSE()
    updatePlayer()
    BEGINDRAW()
    drawPlayer()
    ENDDRAW()
WEND
```

### 5. Use Arrays for Collections
```basic
VAR enemies[10]
FOR i = 0 TO 9
    enemies[i] = ENEMY_CREATE()
NEXT
```

### 6. Error Handling
```basic
TRY
    VAR data = LOADFILE("save.dat")
CATCH error
    PRINT "Error loading file: " + error
ENDTRY
```

---

## Complete Game Example

```basic
REM Space Shooter Game
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768
CONST PLAYER_SPEED = 5.0
CONST BULLET_SPEED = 10.0

REM Game state
VAR score = 0
VAR lives = 3
VAR gameOver = FALSE

REM Player
VAR player = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100)
VAR playerColor = Color(100, 200, 255, 255)

REM Bullets
VAR bullets[10]
VAR bulletCount = 0

REM Initialize
INITWINDOW(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Shooter")
SETTARGETFPS(60)

REM Main loop
WHILE NOT WINDOWSHOULDCLOSE() AND NOT gameOver
    REM Input
    IF ISKEYDOWN(KEY_A) THEN player.x = player.x - PLAYER_SPEED
    IF ISKEYDOWN(KEY_D) THEN player.x = player.x + PLAYER_SPEED
    IF ISKEYPRESSED(KEY_SPACE) THEN
        IF bulletCount < 10 THEN
            bullets[bulletCount] = Vector2(player.x, player.y)
            bulletCount = bulletCount + 1
        ENDIF
    ENDIF
    
    REM Update bullets
    FOR i = 0 TO bulletCount - 1
        bullets[i].y = bullets[i].y - BULLET_SPEED
        IF bullets[i].y < 0 THEN
            REM Remove bullet
            FOR j = i TO bulletCount - 2
                bullets[j] = bullets[j + 1]
            NEXT
            bulletCount = bulletCount - 1
        ENDIF
    NEXT
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(10, 10, 20)
    
    REM Draw player
    DRAWCIRCLE(player.x, player.y, 20, playerColor.r, playerColor.g, playerColor.b)
    
    REM Draw bullets
    FOR i = 0 TO bulletCount - 1
        DRAWCIRCLE(bullets[i].x, bullets[i].y, 5, 255, 255, 100)
    NEXT
    
    REM Draw UI
    DRAWTEXT("Score: " + STR(score), 10, 10, 24, 255, 255, 255)
    DRAWTEXT("Lives: " + STR(lives), 10, 40, 24, 255, 255, 255)
    
    ENDDRAW()
WEND

CLOSEWINDOW()
```

---

## Advanced Features

### Bitwise Operations
```basic
VAR flags = 5
VAR mask = 2
VAR result = flags && mask  REM Bitwise AND
VAR shifted = flags << 1    REM Left shift
VAR inverted = !flags       REM Bitwise NOT
```

### Array Sorting and Searching
```basic
VAR scores = [100, 50, 75, 25, 90]
scores.sort()              REM [25, 50, 75, 90, 100]
VAR index = scores.find(75) REM Returns 2
```

### JSON Data Persistence
```basic
TYPE GameData
    score AS INTEGER
    level AS INTEGER
    playerName AS STRING
END TYPE

VAR data = GameData()
data.score = 1000
data.level = 5
data.playerName = "Player1"

VAR json = data.toJSON()
data.save("save.json")
```

---

## Resources

- **Examples:** See `examples/` directory
- **Raylib Functions:** All 527+ functions available
- **Game Templates:** `examples/game_template.bas`
- **3D Games:** `examples/3d_game_template.bas`

---

## Tips for Game Developers

1. **Start Simple:** Get a basic game loop working first
2. **Use CONST:** Makes configuration easy to change
3. **Organize Code:** Use functions for different game systems
4. **Leverage Raylib:** Full access to all graphics/audio functions
5. **Use ECS for Complex Games:** Better organization
6. **State Machines:** Perfect for menu systems and game states
7. **Arrays are Powerful:** Use advanced operations for game logic

---

**Happy Game Making!**

