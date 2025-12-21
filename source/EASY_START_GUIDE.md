# CyberBasic - Easy Start Guide for Game Developers

**Make 2D and 3D games with simple BASIC syntax and full Raylib power.**

## Quick Start (5 Minutes)

### Step 1: Get CyberBasic

**Option A: Download Pre-built (Windows)**
```bash
# Download cyberbasic.exe from releases
# Place it anywhere (e.g., C:\Games\CyberBasic\)
```

**Option B: Build from Source**
```bash
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic/source
python build.py --static
# Executable: build/cyberbasic.exe
```

### Step 2: Create Your First Game

Create `my_game.bas`:

```basic
REM My First CyberBasic Game
INITWINDOW(800, 600, "My Awesome Game")
SETTARGETFPS(60)

VAR playerX = 400
VAR playerY = 300
VAR speed = 5

WHILE NOT WINDOWSHOULDCLOSE()
    REM Input handling
    IF ISKEYDOWN(KEY_W) THEN playerY = playerY - speed
    IF ISKEYDOWN(KEY_S) THEN playerY = playerY + speed
    IF ISKEYDOWN(KEY_A) THEN playerX = playerX - speed
    IF ISKEYDOWN(KEY_D) THEN playerX = playerX + speed
    
    REM Drawing
    BEGINDRAW()
    CLEARBACKGROUND(30, 30, 50)
    DRAWCIRCLE(playerX, playerY, 30, 255, 100, 100)
    DRAWTEXT("Use WASD to move", 10, 10, 20, 255, 255, 255)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

### Step 3: Run It!

```bash
cyberbasic.exe my_game.bas
```

**That's it!** You just made your first game!

## Making 2D Games

### Basic 2D Game Template

```basic
REM 2D Game Template
INITWINDOW(800, 600, "My 2D Game")
SETTARGETFPS(60)

REM Load assets
VAR playerTexture = LOADTEXTURE("player.png")
VAR background = LOADTEXTURE("bg.png")

REM Game state
VAR playerX = 400
VAR playerY = 300
VAR playerSpeed = 5
VAR score = 0

WHILE NOT WINDOWSHOULDCLOSE()
    REM Update
    IF ISKEYDOWN(KEY_W) THEN playerY = playerY - playerSpeed
    IF ISKEYDOWN(KEY_S) THEN playerY = playerY + playerSpeed
    IF ISKEYDOWN(KEY_A) THEN playerX = playerX - playerSpeed
    IF ISKEYDOWN(KEY_D) THEN playerX = playerX + playerSpeed
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    
    REM Draw background
    DRAWTEXTURE(background, 0, 0, 255, 255, 255)
    
    REM Draw player
    DRAWTEXTURE(playerTexture, playerX, playerY, 255, 255, 255)
    
    REM Draw UI
    DRAWTEXT("Score: " + STR(score), 10, 10, 20, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 35, 20, 255, 255, 255)
    
    ENDDRAW()
WEND

REM Cleanup
UNLOADTEXTURE(playerTexture)
UNLOADTEXTURE(background)
CLOSEWINDOW()
```

### 2D Features Available

**Graphics:**
- `DRAWCIRCLE(x, y, radius, r, g, b)` - Draw circles
- `DRAWRECTANGLE(x, y, width, height, r, g, b)` - Draw rectangles
- `DRAWTEXTURE(texture, x, y, r, g, b)` - Draw textures/sprites
- `DRAWTEXT(text, x, y, size, r, g, b)` - Draw text
- `DRAWLINE(x1, y1, x2, y2, r, g, b)` - Draw lines

**Input:**
- `ISKEYDOWN(key)` - Check if key is pressed
- `ISKEYPRESSED(key)` - Check if key just pressed
- `GETMOUSEPOSITION()` - Get mouse position
- `ISMOUSEBUTTONPRESSED(button)` - Check mouse button

**Audio:**
- `LOADSOUND("file.wav")` - Load sound effect
- `PLAYSOUND(sound)` - Play sound
- `LOADMUSIC("file.mp3")` - Load music
- `PLAYMUSIC(music)` - Play music

## 🌐 Making 3D Games

### Basic 3D Game Template

```basic
REM 3D Game Template
INITWINDOW(800, 600, "My 3D Game")
SETTARGETFPS(60)

REM Setup 3D camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

REM Load 3D model
VAR model = LOADMODEL("character.obj")

VAR rotation = 0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 1
    
    REM Update camera (example: orbit)
    VAR time = GETTIME()
    camera.position.x = COS(time) * 10
    camera.position.z = SIN(time) * 10
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(50, 50, 60)
    
    BEGINDRAW3D(camera)
        REM Draw ground
        DRAWCUBE(VECTOR3(0, -1, 0), 20, 1, 20, GRAY)
        
        REM Draw model with rotation
        PUSHMATRIX()
        ROTATE(rotation, 0, 1, 0)
        DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
        POPMATRIX()
        
        REM Draw primitives
        DRAWCUBE(VECTOR3(5, 1, 5), 1, 1, 1, RED)
        DRAWSPHERE(VECTOR3(-5, 1, -5), 1, BLUE)
    ENDDRAW3D()
    
    REM Draw UI overlay
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 20, 255, 255, 255)
    DRAWTEXT("3D Game", 10, 35, 20, 255, 255, 255)
    
    ENDDRAW()
WEND

REM Cleanup
UNLOADMODEL(model)
CLOSEWINDOW()
```

### 3D Features Available

**3D Primitives:**
- `DRAWCUBE(position, width, height, length, color)` - Draw cube
- `DRAWSPHERE(position, radius, color)` - Draw sphere
- `DRAWPLANE(position, width, length, color)` - Draw plane
- `DRAWCYLINDER(position, radiusTop, radiusBottom, height, color)` - Draw cylinder

**3D Models:**
- `LOADMODEL("file.obj")` - Load 3D model
- `DRAWMODEL(model, position, scale, color)` - Draw model
- `UNLOADMODEL(model)` - Unload model

**Camera:**
- `CAMERA3D()` - Create 3D camera
- `BEGINDRAW3D(camera)` - Start 3D rendering
- `ENDDRAW3D()` - End 3D rendering

**Lighting:**
- `ENABLELIGHTING()` - Enable lighting
- `SETLIGHTDIRECTION(direction, color)` - Set directional light
- `SETLIGHTPOSITION(position, color, intensity)` - Set point light

## Learning Resources

### Examples Directory

The `examples/` folder contains 69+ working examples:

**2D Games:**
- `pong_game.bas` - Complete Pong with AI
- `space_invaders.bas` - Classic Space Invaders
- `2d_game_template.bas` - 2D game starter

**3D Games:**
- `model_viewer.bas` - 3D model viewer
- `3d_game_template.bas` - 3D game starter
- `complete_3d_game_demo.bas` - Full 3D game example

**Features:**
- `hello_graphics.bas` - Basic graphics
- `audio_demo.bas` - Audio system
- `physics_demo.bas` - Physics simulation
- `gui_demo.bas` - GUI system

**Run any example:**
```bash
cyberbasic.exe examples/pong_game.bas
```

### Documentation

- **Game Development Guide:** `docs/GAME_DEVELOPMENT_GUIDE.md`
- **2D Graphics Guide:** `docs/2D_GRAPHICS_GUIDE.md`
- **3D Graphics Guide:** `docs/3D_GRAPHICS_GUIDE.md`
- **Quick Reference:** `docs/QUICK_REFERENCE.md`

## Common Patterns

### Game Loop Pattern

```basic
INITWINDOW(800, 600, "Game")
SETTARGETFPS(60)

WHILE NOT WINDOWSHOULDCLOSE()
    REM 1. Handle Input
    IF ISKEYDOWN(KEY_SPACE) THEN
        REM Do something
    ENDIF
    
    REM 2. Update Game State
    playerX = playerX + velocityX
    
    REM 3. Draw Everything
    BEGINDRAW()
    CLEARBACKGROUND(0, 0, 0)
    REM Draw game objects
    ENDDRAW()
WEND

CLOSEWINDOW()
```

### Sprite Animation Pattern

```basic
VAR frame = 0
VAR frameTime = 0
VAR currentFrame = 0

WHILE NOT WINDOWSHOULDCLOSE()
    frameTime = frameTime + GETFRAMETIME()
    
    REM Change frame every 0.1 seconds
    IF frameTime >= 0.1 THEN
        currentFrame = (currentFrame + 1) MOD 4
        frameTime = 0
    ENDIF
    
    BEGINDRAW()
    REM Draw sprite at currentFrame
    DRAWTEXTUREREC(texture, RECTANGLE(currentFrame * 32, 0, 32, 32), x, y, WHITE)
    ENDDRAW()
WEND
```

### Collision Detection Pattern

```basic
FUNCTION CHECKCOLLISION(x1, y1, w1, h1, x2, y2, w2, h2) AS BOOLEAN
    IF x1 < x2 + w2 AND x1 + w1 > x2 AND y1 < y2 + h2 AND y1 + h1 > y2 THEN
        RETURN TRUE
    ENDIF
    RETURN FALSE
END FUNCTION

REM Usage
IF CHECKCOLLISION(playerX, playerY, 32, 32, enemyX, enemyY, 32, 32) THEN
    REM Collision!
ENDIF
```

## 🛠️ Tips & Tricks

### Performance

- **Use `SETTARGETFPS(60)`** - Limits frame rate, saves CPU
- **Batch texture loading** - Load all textures at start
- **Reuse objects** - Don't create objects in game loop

### Debugging

- **Print values:** `PRINT "X: " + STR(playerX)`
- **Show FPS:** `DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 20, 255, 255, 255)`
- **Draw debug info:** Use `DRAWRECTANGLE` to visualize collision boxes

### Organization

- **Use functions** - Break code into reusable functions
- **Use variables** - Store magic numbers in named variables
- **Add comments** - Use `REM` to document your code

## 🚢 Distributing Your Game

### Static Build

Build with static linking for zero dependencies:

```bash
cd source
python build.py --static
```

### Package Your Game

1. **Include executable:** `cyberbasic.exe`
2. **Include game files:** All `.bas` files
3. **Include assets:** Textures, sounds, models, etc.
4. **Create launcher:** Simple batch file that runs your main game

**Example launcher.bat:**
```batch
@echo off
cyberbasic.exe main_game.bas
pause
```

### Test Distribution

Test on a clean system (no development tools installed) to ensure it works.

## 🎓 Next Steps

1. **Run Examples** - See what's possible
2. **Modify Examples** - Change colors, speeds, add features
3. **Create Your Own** - Start with template, add your ideas
4. **Read Docs** - Learn advanced features
5. **Share Your Game** - Show off what you made!

## Remember

- **CyberBasic is case-insensitive** - `VAR`, `var`, `Var` all work
- **All Raylib functions available** - 527+ functions for graphics, audio, input
- **Simple syntax** - BASIC is easy to learn
- **Powerful features** - Full 2D/3D game development

**Start coding, start creating, start having fun!**

---

**Questions?** Check `docs/` folder or open an issue on GitHub.

**Ready to make games?** Start with `examples/2d_game_template.bas` and modify it!

