# Game Development Features

## Overview
CyberBasic is designed specifically for 2D and 3D game development. All features are optimized for game programming workflows.

## Complete Game Systems

### 1. Game Loop System
**File**: `src/modules/game/game_loop.cpp`

Essential game loop management:
- `GAME.init(width, height, title, [fps])` - Initialize game window
- `GAME.close()` - Close game window
- `GAME.shouldClose()` - Check if window should close
- `GAME.beginFrame()` - Begin frame (clears screen, updates delta time)
- `GAME.endFrame()` - End frame (presents to screen)
- `GAME.setBackgroundColor(r, g, b, [a])` - Set background color
- `GAME.getWidth()` / `GAME.getHeight()` - Get window dimensions
- `GAME.setTargetFPS(fps)` - Set target framerate

**Usage**:
```basic
GAME.init(800, 600, "My Game", 60)
WHILE NOT GAME.shouldClose()
    GAME.beginFrame()
    // Game logic here
    GAME.endFrame()
WEND
GAME.close()
```

### 2. Sprite System
**File**: `src/modules/game/sprite_system.cpp`

2D sprite management:
- `Sprite(imagePath)` - Create sprite from image
- `sprite.draw()` - Draw sprite to screen
- `sprite.setPosition(x, y)` - Set position
- `sprite.setScale(scale)` - Set scale
- `sprite.setRotation(angle)` - Set rotation
- Properties: `x`, `y`, `scale`, `rotation`, `visible`, `width`, `height`

**Usage**:
```basic
VAR player = Sprite("player.png")
player.x = 100
player.y = 200
player.scale = 2.0
player.draw()
```

### 3. Animation System
**File**: `src/modules/game/animation_system.cpp`

Frame-based animations:
- `Animation(name, frameCount, frameWidth, frameHeight, frameDuration)` - Create animation
- `animation.play()` - Start playing
- `animation.stop()` - Stop and reset
- `animation.pause()` - Pause playback
- `animation.setSpeed(speed)` - Set playback speed
- `animation.setLooping(looping)` - Enable/disable looping
- `animation.update(deltaTime)` - Update animation (call each frame)
- `animation.getCurrentFrame()` - Get current frame index
- `animation.getFrameRect()` - Get source rectangle for sprite sheet

**Usage**:
```basic
VAR walkAnim = Animation("walk", 8, 32, 32, 0.1)
walkAnim.setLooping(true)
walkAnim.play()

// In game loop
VAR delta = Clock.getDelta()
walkAnim.update(delta)
VAR frame = walkAnim.getCurrentFrame()
```

### 4. Scene/Entity System
**File**: `src/modules/game/scene_entity_system.cpp`

ECS (Entity-Component-System) architecture:
- `Scene(name)` - Create scene
- `scene.createEntity(name)` - Create entity in scene
- `scene.update()` - Update all entities
- `scene.draw()` - Draw all entities
- `scene.getEntityCount()` - Get number of entities
- `entity.addComponent(componentType)` - Add component to entity
- `entity.setPosition(x, y, [z])` - Set entity position
- Components: Transform, Sprite, Model3D, Physics, Collider

**Usage**:
```basic
VAR scene = Scene("MainScene")
VAR player = scene.createEntity("Player")
player.setPosition(0, 0, 0)
player.addComponent("Sprite")

// In game loop
scene.update()
scene.draw()
```

### 5. Camera System
**File**: `src/modules/game/camera_system.cpp`

2D and 3D camera management:

**2D Camera**:
- `Camera2D([targetX, targetY, offsetX, offsetY, rotation, zoom])` - Create 2D camera
- `camera.setTarget(x, y)` - Set camera target
- `camera.setZoom(zoom)` - Set zoom level
- `camera.begin()` - Begin 2D camera mode
- `camera.end()` - End 2D camera mode

**3D Camera**:
- `Camera3D([posX, posY, posZ, targetX, targetY, targetZ, upX, upY, upZ, fovy])` - Create 3D camera
- `camera.setPosition(x, y, z)` - Set camera position
- `camera.setTarget(x, y, z)` - Set camera target
- `camera.begin()` - Begin 3D camera mode
- `camera.end()` - End 3D camera mode

**Usage**:
```basic
// 2D
VAR camera2d = Camera2D()
camera2d.setTarget(player.x, player.y)
camera2d.begin()
// Draw 2D objects
camera2d.end()

// 3D
VAR camera3d = Camera3D()
camera3d.setPosition(0, 10, 10)
camera3d.setTarget(0, 0, 0)
camera3d.begin()
// Draw 3D objects
camera3d.end()
```

### 6. Collision System
**File**: `src/modules/game/collision_system.cpp`

2D and 3D collision detection:
- `Collision.checkRectRect(rect1, rect2)` - Rectangle vs Rectangle
- `Collision.checkRectPoint(rect, x, y)` - Rectangle vs Point
- `Collision.checkCircleCircle(x1, y1, r1, x2, y2, r2)` - Circle vs Circle
- `Collision.checkCirclePoint(cx, cy, radius, px, py)` - Circle vs Point
- `Collision.checkSphereSphere(x1, y1, z1, r1, x2, y2, z2, r2)` - Sphere vs Sphere (3D)
- `Collision.distance2D(x1, y1, x2, y2)` - Distance between 2D points
- `Collision.distance3D(x1, y1, z1, x2, y2, z2)` - Distance between 3D points

**Usage**:
```basic
VAR rect1 = Rectangle(10, 10, 50, 50)
VAR rect2 = Rectangle(30, 30, 50, 50)
IF Collision.checkRectRect(rect1, rect2) THEN
    PRINT "Collision!"
END IF

VAR dist = Collision.distance2D(player.x, player.y, enemy.x, enemy.y)
```

### 7. Timer/Clock System
**File**: `src/modules/game/timer_system.cpp`

Timing utilities:
- `Timer()` - Create timer
- `timer.start()` - Start timer
- `timer.stop()` - Stop timer
- `timer.pause()` - Pause timer
- `timer.elapsed()` - Get elapsed time in seconds
- `Clock.getTime()` - Get current time as string
- `Clock.getDelta()` - Get frame delta time
- `Clock.getFPS()` - Get current FPS

**Usage**:
```basic
VAR timer = Timer()
timer.start()
// ... do work ...
VAR elapsed = timer.elapsed()

VAR delta = Clock.getDelta()
VAR fps = Clock.getFPS()
```

### 8. Input Events System
**File**: `src/modules/game/input_events.cpp`

Input handling:
- `Input.getKey(keyName)` - Check if key is down
- `Input.getKeyPressed(keyName)` - Check if key was just pressed
- `Input.getMouseX()` - Get mouse X position
- `Input.getMouseY()` - Get mouse Y position
- `Input.getMouseButton(button)` - Check if mouse button is down

**Usage**:
```basic
IF Input.getKeyPressed("SPACE") THEN
    PRINT "Jump!"
END IF

VAR mouseX = Input.getMouseX()
VAR mouseY = Input.getMouseY()
```

### 9. Game Helpers
**File**: `src/modules/game/game_helpers.cpp`

Math and drawing utilities:

**Math Functions**:
- `Math.clamp(value, min, max)` - Clamp value
- `Math.lerp(a, b, t)` - Linear interpolation
- `Math.lerpAngle(a, b, t)` - Angle interpolation
- `Math.moveTowards(current, target, maxDelta)` - Move towards target
- `Math.smoothDamp(...)` - Smooth damping

**2D Drawing**:
- `Draw.drawRectangle(x, y, width, height, color)`
- `Draw.drawCircle(x, y, radius, color)`
- `Draw.drawLine(x1, y1, x2, y2, color)`
- `Draw.drawText(text, x, y, fontSize, color)`

**3D Drawing**:
- `Draw3D.drawCube(position, width, height, length, color)`
- `Draw3D.drawSphere(position, radius, color)`
- `Draw3D.drawGrid(slices, spacing)`

**Usage**:
```basic
// Math
VAR value = Math.clamp(player.x, 0, 800)
VAR smooth = Math.lerp(current, target, 0.1)

// 2D Drawing
Draw.drawRectangle(10, 10, 100, 100, 255, 0, 0)
Draw.drawCircle(400, 300, 50, 0, 255, 0)
Draw.drawText("Hello", 10, 10, 20, 255, 255, 255)

// 3D Drawing
Draw3D.drawCube(0, 0, 0, 2, 2, 2, 255, 0, 0)
Draw3D.drawSphere(0, 5, 0, 1, 0, 255, 0)
Draw3D.drawGrid(10, 1.0)
```

## Complete Game Templates

### 2D Game Template
**File**: `examples/2d_game_template.bas`

Complete template showing:
- Game initialization
- 2D camera setup
- Sprite creation and drawing
- Animation system
- Input handling
- Game loop structure

### 3D Game Template
**File**: `examples/3d_game_template.bas`

Complete template showing:
- 3D game initialization
- 3D camera setup
- Scene/Entity system
- 3D rendering
- Game loop structure

## Architecture for Game Development

### Modular Design
- Each game system is in its own module
- Easy to enable/disable features
- Clean separation of concerns
- All systems integrate with Raylib

### Performance Optimized
- Efficient sprite rendering
- Delta time-based updates
- Frame rate control
- Collision detection optimized

### Game-Focused Features
- All features designed for game development
- 2D and 3D support from the start
- ECS architecture for scalability
- Animation system for sprites
- Complete input handling

## Game Development Status

- **Game Loop**: Complete
- **Sprite System**: Complete
- **Animation System**: Complete
- **Scene/Entity System**: Complete
- **Camera System**: Complete (2D & 3D)
- **Collision System**: Complete (2D & 3D)
- **Timer/Clock**: Complete
- **Input Events**: Complete
- **Game Helpers**: Complete (Math & Drawing)

## Ready for Game Development

CyberBasic is now fully equipped for 2D and 3D game development with:
- Complete game loop management
- Sprite and animation systems
- Scene/Entity architecture
- Camera systems for 2D and 3D
- Collision detection
- Input handling
- Timing utilities
- Drawing helpers
- Math utilities

All systems are modular, well-documented, and ready for production game development!
CyberBasic is designed specifically for 2D and 3D game development. All features are optimized for game programming workflows.
**File**: `src/modules/game/game_loop.cpp`

Essential game loop management:
- `GAME.init(width, height, title, [fps])` - Initialize game window
- `GAME.close()` - Close game window
- `GAME.shouldClose()` - Check if window should close
- `GAME.beginFrame()` - Begin frame (clears screen, updates delta time)
- `GAME.endFrame()` - End frame (presents to screen)
- `GAME.setBackgroundColor(r, g, b, [a])` - Set background color
- `GAME.getWidth()` / `GAME.getHeight()` - Get window dimensions
- `GAME.setTargetFPS(fps)` - Set target framerate

**Usage**:
```basic
GAME.init(800, 600, "My Game", 60)
WHILE NOT GAME.shouldClose()
    GAME.beginFrame()
    // Game logic here
    GAME.endFrame()
WEND
GAME.close()
```
**File**: `src/modules/game/sprite_system.cpp`

2D sprite management:
- `Sprite(imagePath)` - Create sprite from image
- `sprite.draw()` - Draw sprite to screen
- `sprite.setPosition(x, y)` - Set position
- `sprite.setScale(scale)` - Set scale
- `sprite.setRotation(angle)` - Set rotation
- Properties: `x`, `y`, `scale`, `rotation`, `visible`, `width`, `height`

**Usage**:
```basic
VAR player = Sprite("player.png")
player.x = 100
player.y = 200
player.scale = 2.0
player.draw()
```
**File**: `src/modules/game/animation_system.cpp`

Frame-based animations:
- `Animation(name, frameCount, frameWidth, frameHeight, frameDuration)` - Create animation
- `animation.play()` - Start playing
- `animation.stop()` - Stop and reset
- `animation.pause()` - Pause playback
- `animation.setSpeed(speed)` - Set playback speed
- `animation.setLooping(looping)` - Enable/disable looping
- `animation.update(deltaTime)` - Update animation (call each frame)
- `animation.getCurrentFrame()` - Get current frame index
- `animation.getFrameRect()` - Get source rectangle for sprite sheet

**Usage**:
```basic
VAR walkAnim = Animation("walk", 8, 32, 32, 0.1)
walkAnim.setLooping(true)
walkAnim.play()

// In game loop
VAR delta = Clock.getDelta()
walkAnim.update(delta)
VAR frame = walkAnim.getCurrentFrame()
```
**File**: `src/modules/game/scene_entity_system.cpp`

ECS (Entity-Component-System) architecture:
- `Scene(name)` - Create scene
- `scene.createEntity(name)` - Create entity in scene
- `scene.update()` - Update all entities
- `scene.draw()` - Draw all entities
- `scene.getEntityCount()` - Get number of entities
- `entity.addComponent(componentType)` - Add component to entity
- `entity.setPosition(x, y, [z])` - Set entity position
- Components: Transform, Sprite, Model3D, Physics, Collider

**Usage**:
```basic
VAR scene = Scene("MainScene")
VAR player = scene.createEntity("Player")
player.setPosition(0, 0, 0)
player.addComponent("Sprite")

// In game loop
scene.update()
scene.draw()
```
**File**: `src/modules/game/camera_system.cpp`

2D and 3D camera management:

**2D Camera**:
- `Camera2D([targetX, targetY, offsetX, offsetY, rotation, zoom])` - Create 2D camera
- `camera.setTarget(x, y)` - Set camera target
- `camera.setZoom(zoom)` - Set zoom level
- `camera.begin()` - Begin 2D camera mode
- `camera.end()` - End 2D camera mode

**3D Camera**:
- `Camera3D([posX, posY, posZ, targetX, targetY, targetZ, upX, upY, upZ, fovy])` - Create 3D camera
- `camera.setPosition(x, y, z)` - Set camera position
- `camera.setTarget(x, y, z)` - Set camera target
- `camera.begin()` - Begin 3D camera mode
- `camera.end()` - End 3D camera mode

**Usage**:
```basic
// 2D
VAR camera2d = Camera2D()
camera2d.setTarget(player.x, player.y)
camera2d.begin()
// Draw 2D objects
camera2d.end()

// 3D
VAR camera3d = Camera3D()
camera3d.setPosition(0, 10, 10)
camera3d.setTarget(0, 0, 0)
camera3d.begin()
// Draw 3D objects
camera3d.end()
```
**File**: `src/modules/game/collision_system.cpp`

2D and 3D collision detection:
- `Collision.checkRectRect(rect1, rect2)` - Rectangle vs Rectangle
- `Collision.checkRectPoint(rect, x, y)` - Rectangle vs Point
- `Collision.checkCircleCircle(x1, y1, r1, x2, y2, r2)` - Circle vs Circle
- `Collision.checkCirclePoint(cx, cy, radius, px, py)` - Circle vs Point
- `Collision.checkSphereSphere(x1, y1, z1, r1, x2, y2, z2, r2)` - Sphere vs Sphere (3D)
- `Collision.distance2D(x1, y1, x2, y2)` - Distance between 2D points
- `Collision.distance3D(x1, y1, z1, x2, y2, z2)` - Distance between 3D points

**Usage**:
```basic
VAR rect1 = Rectangle(10, 10, 50, 50)
VAR rect2 = Rectangle(30, 30, 50, 50)
IF Collision.checkRectRect(rect1, rect2) THEN
    PRINT "Collision!"
END IF

VAR dist = Collision.distance2D(player.x, player.y, enemy.x, enemy.y)
```
**File**: `src/modules/game/timer_system.cpp`

Timing utilities:
- `Timer()` - Create timer
- `timer.start()` - Start timer
- `timer.stop()` - Stop timer
- `timer.pause()` - Pause timer
- `timer.elapsed()` - Get elapsed time in seconds
- `Clock.getTime()` - Get current time as string
- `Clock.getDelta()` - Get frame delta time
- `Clock.getFPS()` - Get current FPS

**Usage**:
```basic
VAR timer = Timer()
timer.start()
// ... do work ...
VAR elapsed = timer.elapsed()

VAR delta = Clock.getDelta()
VAR fps = Clock.getFPS()
```
**File**: `src/modules/game/input_events.cpp`

Input handling:
- `Input.getKey(keyName)` - Check if key is down
- `Input.getKeyPressed(keyName)` - Check if key was just pressed
- `Input.getMouseX()` - Get mouse X position
- `Input.getMouseY()` - Get mouse Y position
- `Input.getMouseButton(button)` - Check if mouse button is down

**Usage**:
```basic
IF Input.getKeyPressed("SPACE") THEN
    PRINT "Jump!"
END IF

VAR mouseX = Input.getMouseX()
VAR mouseY = Input.getMouseY()
```
**File**: `src/modules/game/game_helpers.cpp`

Math and drawing utilities:

**Math Functions**:
- `Math.clamp(value, min, max)` - Clamp value
- `Math.lerp(a, b, t)` - Linear interpolation
- `Math.lerpAngle(a, b, t)` - Angle interpolation
- `Math.moveTowards(current, target, maxDelta)` - Move towards target
- `Math.smoothDamp(...)` - Smooth damping

**2D Drawing**:
- `Draw.drawRectangle(x, y, width, height, color)`
- `Draw.drawCircle(x, y, radius, color)`
- `Draw.drawLine(x1, y1, x2, y2, color)`
- `Draw.drawText(text, x, y, fontSize, color)`

**3D Drawing**:
- `Draw3D.drawCube(position, width, height, length, color)`
- `Draw3D.drawSphere(position, radius, color)`
- `Draw3D.drawGrid(slices, spacing)`

**Usage**:
```basic
// Math
VAR value = Math.clamp(player.x, 0, 800)
VAR smooth = Math.lerp(current, target, 0.1)

// 2D Drawing
Draw.drawRectangle(10, 10, 100, 100, 255, 0, 0)
Draw.drawCircle(400, 300, 50, 0, 255, 0)
Draw.drawText("Hello", 10, 10, 20, 255, 255, 255)

// 3D Drawing
Draw3D.drawCube(0, 0, 0, 2, 2, 2, 255, 0, 0)
Draw3D.drawSphere(0, 5, 0, 1, 0, 255, 0)
Draw3D.drawGrid(10, 1.0)
```
**File**: `examples/2d_game_template.bas`

Complete template showing:
- Game initialization
- 2D camera setup
- Sprite creation and drawing
- Animation system
- Input handling
- Game loop structure
**File**: `examples/3d_game_template.bas`

Complete template showing:
- 3D game initialization
- 3D camera setup
- Scene/Entity system
- 3D rendering
- Game loop structure
- Each game system is in its own module
- Easy to enable/disable features
- Clean separation of concerns
- All systems integrate with Raylib
- Efficient sprite rendering
- Delta time-based updates
- Frame rate control
- Collision detection optimized
- All features designed for game development
- 2D and 3D support from the start
- ECS architecture for scalability
- Animation system for sprites
- Complete input handling

- **Game Loop**: Complete
- **Sprite System**: Complete
- **Animation System**: Complete
- **Scene/Entity System**: Complete
- **Camera System**: Complete (2D & 3D)
- **Collision System**: Complete (2D & 3D)
- **Timer/Clock**: Complete
- **Input Events**: Complete
- **Game Helpers**: Complete (Math & Drawing)

CyberBasic is now fully equipped for 2D and 3D game development with:
- Complete game loop management
- Sprite and animation systems
- Scene/Entity architecture
- Camera systems for 2D and 3D
- Collision detection
- Input handling
- Timing utilities
- Drawing helpers
- Math utilities

All systems are modular, well-documented, and ready for production game development!
