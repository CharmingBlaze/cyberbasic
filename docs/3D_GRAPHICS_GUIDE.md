# 3D Graphics API Guide

**Complete guide to 3D graphics in CyberBasic**

## Table of Contents
1. [Getting Started](#getting-started)
2. [3D Camera System](#3d-camera-system)
3. [3D Primitives](#3d-primitives)
4. [Models and Meshes](#models-and-meshes)
5. [Lighting](#lighting)
6. [Materials and Textures](#materials-and-textures)
7. [Transformations](#transformations)
8. [Best Practices](#best-practices)

---

## Getting Started

### Basic 3D Setup

Every 3D graphics program needs:

```basic
REM Initialize window
INITWINDOW(800, 600, "My 3D Game")
SETTARGETFPS(60)

REM Create 3D camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    
    REM Start 3D drawing
    BEGINDRAW3D(camera)
    
    REM Your 3D drawing code here
    
    ENDDRAW3D()
    
    REM 2D overlay (UI, text, etc.)
    DRAWTEXT("3D Scene", 10, 10, 20, 255, 255, 255)
    
    ENDDRAW()
WEND

CLOSEWINDOW()
```

### 3D Drawing Frame

All 3D drawing must happen between `BEGINDRAW3D(camera)` and `ENDDRAW3D()`:

```basic
BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    
    BEGINDRAW3D(camera)
        REM All 3D drawing commands go here
    ENDDRAW3D()
    
    REM 2D UI can go here
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 20, 255, 255, 255)
    
ENDDRAW()
```

---

## 3D Camera System

### Creating a Camera

```basic
REM Create perspective camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)    REM Camera position
camera.target = VECTOR3(0, 0, 0)        REM Where camera looks
camera.up = VECTOR3(0, 1, 0)            REM Up vector
camera.fovy = 45                        REM Field of view (degrees)
camera.projection = CAMERA_PERSPECTIVE  REM Perspective projection
```

### Camera Types

```basic
REM First-person camera
VAR fpsCamera = CAMERA3D()
fpsCamera.position = VECTOR3(0, 2, 0)
fpsCamera.target = VECTOR3(0, 2, -1)
fpsCamera.up = VECTOR3(0, 1, 0)
fpsCamera.fovy = 60
fpsCamera.projection = CAMERA_PERSPECTIVE

REM Third-person camera
VAR tpsCamera = CAMERA3D()
tpsCamera.position = VECTOR3(0, 5, 10)
tpsCamera.target = VECTOR3(0, 0, 0)
tpsCamera.up = VECTOR3(0, 1, 0)
tpsCamera.fovy = 45
tpsCamera.projection = CAMERA_PERSPECTIVE

REM Orthographic camera (no perspective)
VAR orthoCamera = CAMERA3D()
orthoCamera.position = VECTOR3(0, 10, 0)
orthoCamera.target = VECTOR3(0, 0, 0)
orthoCamera.up = VECTOR3(0, 0, 1)
orthoCamera.fovy = 10
orthoCamera.projection = CAMERA_ORTHOGRAPHIC
```

### Camera Movement

```basic
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

VAR cameraSpeed = 0.1
VAR rotation = 0

WHILE NOT WINDOWSHOULDCLOSE()
    REM Rotate camera around origin
    rotation = rotation + 1
    VAR angle = rotation * 0.0174533  REM Convert to radians
    camera.position.x = SIN(angle) * 10
    camera.position.z = COS(angle) * 10
    camera.target = VECTOR3(0, 0, 0)
    
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    BEGINDRAW3D(camera)
        REM Draw scene
    ENDDRAW3D()
    ENDDRAW()
WEND
```

### Camera Controls

```basic
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 5, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

VAR moveSpeed = 0.1

WHILE NOT WINDOWSHOULDCLOSE()
    REM WASD movement
    IF ISKEYDOWN(KEY_W) THEN camera.position.z = camera.position.z - moveSpeed
    IF ISKEYDOWN(KEY_S) THEN camera.position.z = camera.position.z + moveSpeed
    IF ISKEYDOWN(KEY_A) THEN camera.position.x = camera.position.x - moveSpeed
    IF ISKEYDOWN(KEY_D) THEN camera.position.x = camera.position.x + moveSpeed
    
    REM QE for up/down
    IF ISKEYDOWN(KEY_Q) THEN camera.position.y = camera.position.y + moveSpeed
    IF ISKEYDOWN(KEY_E) THEN camera.position.y = camera.position.y - moveSpeed
    
    REM Update camera target to look forward
    camera.target = VECTOR3(camera.position.x, camera.position.y, camera.position.z - 1)
    
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    BEGINDRAW3D(camera)
        REM Draw scene
    ENDDRAW3D()
    ENDDRAW()
WEND
```

---

## 3D Primitives

### Cubes

```basic
REM Draw filled cube
DRAWCUBE(VECTOR3(0, 0, 0), 2, 2, 2, RED)

REM Draw cube wireframe
DRAWCUBEWIRES(VECTOR3(0, 0, 0), 2, 2, 2, WHITE)

REM Draw cube with rotation
DRAWCUBEV(VECTOR3(0, 0, 0), VECTOR3(2, 2, 2), RED)
```

**Parameters:** `position, width, height, length, color`

### Spheres

```basic
REM Draw filled sphere
DRAWSPHERE(VECTOR3(0, 0, 0), 1, GREEN)

REM Draw sphere wireframe
DRAWSPHEREWIRES(VECTOR3(0, 0, 0), 1, WHITE)

REM Draw sphere with rings and slices
DRAWSPHEREEX(VECTOR3(0, 0, 0), 1, 16, 16, BLUE)
```

**Parameters:** `position, radius, color`

### Planes

```basic
REM Draw plane (ground)
DRAWPLANE(VECTOR3(0, 0, 0), VECTOR2(10, 10), GRAY)
```

### Grid

```basic
REM Draw grid
DRAWGRID(10, 1)  REM 10 subdivisions, 1 unit spacing
```

### Lines

```basic
REM Draw 3D line
DRAWLINE3D(VECTOR3(0, 0, 0), VECTOR3(5, 5, 5), RED)

REM Draw point
DRAWPOINT3D(VECTOR3(0, 0, 0), RED)
```

---

## Models and Meshes

### Loading Models

```basic
REM Load model from file
VAR model = LOADMODEL("character.obj")

REM Draw model
DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)

REM Unload when done
UNLOADMODEL(model)
```

**Supported formats:** `.obj`, `.gltf`, `.glb`, `.iqm`

### Model Transformations

```basic
VAR model = LOADMODEL("character.obj")
VAR position = VECTOR3(0, 0, 0)
VAR scale = 1.0
VAR rotation = 0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 1
    
    REM Draw with rotation
    DRAWMODELEX(model, position, VECTOR3(0, 1, 0), rotation, VECTOR3(scale, scale, scale), WHITE)
    
    REM Or use separate transform
    PUSHMATRIX()
    TRANSLATE(position.x, position.y, position.z)
    ROTATE(rotation, 0, 1, 0)
    SCALE(scale, scale, scale)
    DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
    POPMATRIX()
WEND
```

### Model Animation

```basic
VAR model = LOADMODEL("animated_character.glb")
VAR anims = LOADMODELANIMATIONS("animated_character.glb")

VAR animFrameCounter = 0
VAR currentAnim = 0

WHILE NOT WINDOWSHOULDCLOSE()
    animFrameCounter = animFrameCounter + 1
    
    REM Update animation
    UPDATEMODELANIMATION(model, anims[currentAnim], animFrameCounter)
    
    REM Draw animated model
    DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
WEND
```

### Creating Meshes

```basic
REM Generate mesh
VAR cubeMesh = GENMESHCUBE(2, 2, 2)
VAR sphereMesh = GENMESHSPHERE(1, 16, 16)
VAR planeMesh = GENMESHPLANE(10, 10, 1, 1)

REM Load mesh as model
VAR cubeModel = LOADMODELFROMMESH(cubeMesh)
VAR sphereModel = LOADMODELFROMMESH(sphereMesh)
VAR planeModel = LOADMODELFROMMESH(planeMesh)

REM Draw
DRAWMODEL(cubeModel, VECTOR3(0, 0, 0), 1, RED)
DRAWMODEL(sphereModel, VECTOR3(5, 0, 0), 1, GREEN)
DRAWMODEL(planeModel, VECTOR3(0, -1, 0), 1, GRAY)
```

---

## Lighting

### Basic Lighting

```basic
REM Enable lighting
ENABLELIGHTING()

REM Create light
VAR light = LIGHT()
light.type = LIGHT_DIRECTIONAL
light.position = VECTOR3(0, 10, 0)
light.target = VECTOR3(0, 0, 0)
light.color = Color(255, 255, 255, 255)
light.intensity = 1.0

REM Set ambient light
SETAMBIENTLIGHT(0.2, 0.2, 0.2)  REM r, g, b

REM Draw with lighting
DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
```

### Light Types

```basic
REM Directional light (sun)
VAR sun = LIGHT()
sun.type = LIGHT_DIRECTIONAL
sun.position = VECTOR3(0, 10, 0)
sun.target = VECTOR3(0, 0, 0)
sun.color = Color(255, 255, 200, 255)

REM Point light (lamp)
VAR lamp = LIGHT()
lamp.type = LIGHT_POINT
lamp.position = VECTOR3(5, 2, 5)
lamp.color = Color(255, 200, 100, 255)
lamp.intensity = 1.0
lamp.range = 10.0

REM Spot light (flashlight)
VAR spotlight = LIGHT()
spotlight.type = LIGHT_SPOT
spotlight.position = VECTOR3(0, 5, 0)
spotlight.target = VECTOR3(0, 0, 0)
spotlight.color = Color(255, 255, 255, 255)
spotlight.angle = 45
spotlight.range = 20.0
```

---

## Materials and Textures

### Applying Textures to Models

```basic
REM Load model
VAR model = LOADMODEL("character.obj")

REM Load texture
VAR texture = LOADTEXTURE("character_diffuse.png")

REM Set model material texture
SETMATERIALTEXTURE(model, MATERIAL_MAP_DIFFUSE, texture)

REM Draw model
DRAWMODEL(model, VECTOR3(0, 0, 0), 1, WHITE)
```

### Material Properties

```basic
VAR model = LOADMODEL("object.obj")

REM Set material properties
SETMATERIALSHININESS(model, 32)  REM Shine
SETMATERIALMETALLIC(model, 0.5)  REM Metallic (0-1)
SETMATERIALROUGHNESS(model, 0.3) REM Roughness (0-1)

REM Set material colors
SETMATERIALCOLOR(model, MATERIAL_MAP_DIFFUSE, Color(255, 200, 100, 255))
SETMATERIALCOLOR(model, MATERIAL_MAP_SPECULAR, Color(255, 255, 255, 255))
```

### Multiple Textures

```basic
VAR model = LOADMODEL("pbr_model.glb")

REM Load PBR textures
VAR albedo = LOADTEXTURE("albedo.png")
VAR normal = LOADTEXTURE("normal.png")
VAR metallic = LOADTEXTURE("metallic.png")
VAR roughness = LOADTEXTURE("roughness.png")

REM Apply textures
SETMATERIALTEXTURE(model, MATERIAL_MAP_DIFFUSE, albedo)
SETMATERIALTEXTURE(model, MATERIAL_MAP_NORMAL, normal)
SETMATERIALTEXTURE(model, MATERIAL_MAP_METALNESS, metallic)
SETMATERIALTEXTURE(model, MATERIAL_MAP_ROUGHNESS, roughness)
```

---

## Transformations

### Basic Transformations

```basic
REM Push transformation matrix
PUSHMATRIX()

REM Translate (move)
TRANSLATE(5, 0, 0)

REM Rotate (degrees)
ROTATE(45, 0, 1, 0)  REM Rotate 45 degrees around Y axis

REM Scale
SCALE(2, 2, 2)

REM Draw (will be transformed)
DRAWCUBE(VECTOR3(0, 0, 0), 1, 1, 1, RED)

REM Pop transformation
POPMATRIX()
```

### Complex Transformations

```basic
VAR rotation = 0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 1
    
    REM Rotate around origin
    PUSHMATRIX()
    ROTATE(rotation, 0, 1, 0)
    TRANSLATE(5, 0, 0)  REM Move 5 units along X
    ROTATE(rotation * 2, 1, 0, 0)  REM Rotate on X axis
    DRAWCUBE(VECTOR3(0, 0, 0), 1, 1, 1, RED)
    POPMATRIX()
WEND
```

---

## Complete 3D Game Example

```basic
REM Complete 3D Game Example
INITWINDOW(800, 600, "3D Game")
SETTARGETFPS(60)

REM Setup camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

REM Load models
VAR playerModel = LOADMODEL("player.obj")
VAR groundModel = LOADMODELFROMMESH(GENMESHPLANE(20, 20, 1, 1))

REM Game state
VAR playerPos = VECTOR3(0, 1, 0)
VAR playerRotation = 0
VAR moveSpeed = 0.1

WHILE NOT WINDOWSHOULDCLOSE()
    REM Input
    IF ISKEYDOWN(KEY_W) THEN playerPos.z = playerPos.z - moveSpeed
    IF ISKEYDOWN(KEY_S) THEN playerPos.z = playerPos.z + moveSpeed
    IF ISKEYDOWN(KEY_A) THEN playerPos.x = playerPos.x - moveSpeed
    IF ISKEYDOWN(KEY_D) THEN playerPos.x = playerPos.x + moveSpeed
    
    REM Update camera to follow player
    camera.target = playerPos
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(50, 50, 60)
    
    BEGINDRAW3D(camera)
        REM Draw ground
        DRAWMODEL(groundModel, VECTOR3(0, 0, 0), 1, GRAY)
        
        REM Draw player
        PUSHMATRIX()
        TRANSLATE(playerPos.x, playerPos.y, playerPos.z)
        ROTATE(playerRotation, 0, 1, 0)
        DRAWMODEL(playerModel, VECTOR3(0, 0, 0), 1, WHITE)
        POPMATRIX()
        
        REM Draw some cubes
        DRAWCUBE(VECTOR3(5, 1, 5), 1, 1, 1, RED)
        DRAWCUBE(VECTOR3(-5, 1, -5), 1, 1, 1, BLUE)
        DRAWSPHERE(VECTOR3(0, 2, 5), 0.5, GREEN)
    ENDDRAW3D()
    
    REM Draw UI
    DRAWTEXT("Position: " + STR(playerPos.x) + ", " + STR(playerPos.y) + ", " + STR(playerPos.z), 10, 10, 20, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 35, 20, 255, 255, 255)
    
    ENDDRAW()
WEND

REM Cleanup
UNLOADMODEL(playerModel)
UNLOADMODEL(groundModel)
CLOSEWINDOW()
```

---

## Best Practices

### 1. Camera Management

- Keep camera updates in the game loop
- Use appropriate field of view (45-60 degrees)
- Update camera target smoothly

### 2. Model Loading

- Load models once, reuse them
- Unload models when done
- Use appropriate model formats (.glb recommended)

### 3. Lighting

- Use ambient light for base illumination
- Add directional lights for main lighting
- Use point/spot lights sparingly

### 4. Performance

- Limit number of draw calls
- Use instancing for repeated objects
- Cull objects outside camera view

### 5. Coordinate System

- Y is up, Z is forward (right-handed)
- Remember camera position and target
- Use appropriate scale (1 unit = 1 meter recommended)

### 6. Transformations

- Always push/pop matrices
- Apply transformations in order: Scale → Rotate → Translate
- Use local coordinates when possible

---

## Common Patterns

### Orbiting Camera

```basic
VAR camera = CAMERA3D()
VAR angle = 0
VAR distance = 10
VAR height = 5

WHILE NOT WINDOWSHOULDCLOSE()
    angle = angle + 1
    VAR rad = angle * 0.0174533
    
    camera.position.x = SIN(rad) * distance
    camera.position.z = COS(rad) * distance
    camera.position.y = height
    camera.target = VECTOR3(0, 0, 0)
    
    BEGINDRAW()
    BEGINDRAW3D(camera)
        REM Draw scene
    ENDDRAW3D()
    ENDDRAW()
WEND
```

### First-Person Camera

```basic
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 2, 0)
camera.target = VECTOR3(0, 2, -1)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 60
camera.projection = CAMERA_PERSPECTIVE

VAR yaw = 0
VAR pitch = 0
VAR moveSpeed = 0.1

WHILE NOT WINDOWSHOULDCLOSE()
    REM Mouse look
    VAR mouseDelta = GETMOUSEDELTA()
    yaw = yaw - mouseDelta.x * 0.1
    pitch = pitch - mouseDelta.y * 0.1
    IF pitch > 89 THEN pitch = 89
    IF pitch < -89 THEN pitch = -89
    
    REM Calculate forward direction
    VAR yawRad = yaw * 0.0174533
    VAR pitchRad = pitch * 0.0174533
    VAR forward = VECTOR3(SIN(yawRad) * COS(pitchRad), SIN(pitchRad), -COS(yawRad) * COS(pitchRad))
    
    REM WASD movement
    VAR right = VECTOR3(SIN(yawRad - 1.5708), 0, -COS(yawRad - 1.5708))
    
    IF ISKEYDOWN(KEY_W) THEN camera.position = VECTOR3ADD(camera.position, VECTOR3SCALE(forward, moveSpeed))
    IF ISKEYDOWN(KEY_S) THEN camera.position = VECTOR3SUBTRACT(camera.position, VECTOR3SCALE(forward, moveSpeed))
    IF ISKEYDOWN(KEY_A) THEN camera.position = VECTOR3SUBTRACT(camera.position, VECTOR3SCALE(right, moveSpeed))
    IF ISKEYDOWN(KEY_D) THEN camera.position = VECTOR3ADD(camera.position, VECTOR3SCALE(right, moveSpeed))
    
    camera.target = VECTOR3ADD(camera.position, forward)
    
    BEGINDRAW()
    BEGINDRAW3D(camera)
        REM Draw scene
    ENDDRAW3D()
    ENDDRAW()
WEND
```

---

## API Reference

### Camera Functions

- `CAMERA3D()` - Create 3D camera
- `BEGINDRAW3D(camera)` - Start 3D drawing mode
- `ENDDRAW3D()` - End 3D drawing mode

### Primitive Functions

- `DRAWCUBE(position, width, height, length, color)` - Draw cube
- `DRAWCUBEWIRES(position, width, height, length, color)` - Draw cube wireframe
- `DRAWSPHERE(position, radius, color)` - Draw sphere
- `DRAWSPHEREWIRES(position, radius, color)` - Draw sphere wireframe
- `DRAWPLANE(position, size, color)` - Draw plane
- `DRAWGRID(slices, spacing)` - Draw grid

### Model Functions

- `LOADMODEL(filename)` - Load model from file
- `DRAWMODEL(model, position, scale, color)` - Draw model
- `DRAWMODELEX(model, position, rotationAxis, rotationAngle, scale, color)` - Draw with transform
- `UNLOADMODEL(model)` - Free model memory

### Mesh Functions

- `GENMESHCUBE(width, height, length)` - Generate cube mesh
- `GENMESHSPHERE(radius, rings, slices)` - Generate sphere mesh
- `GENMESHPLANE(width, length, resX, resZ)` - Generate plane mesh
- `LOADMODELFROMMESH(mesh)` - Create model from mesh

### Lighting Functions

- `ENABLELIGHTING()` - Enable lighting
- `DISABLELIGHTING()` - Disable lighting
- `SETAMBIENTLIGHT(r, g, b)` - Set ambient light

### Transformation Functions

- `PUSHMATRIX()` - Push transformation matrix
- `POPMATRIX()` - Pop transformation matrix
- `TRANSLATE(x, y, z)` - Translate
- `ROTATE(angle, x, y, z)` - Rotate
- `SCALE(x, y, z)` - Scale

---

For more examples, check the `examples/` directory, especially:
- `3d_game_template.bas`
- `complete_3d_game_demo.bas`
- `models3d_demo.bas`

