# 2D Graphics API Guide

**Complete guide to 2D graphics in CyberBasic**

## Table of Contents
1. [Getting Started](#getting-started)
2. [Drawing Primitives](#drawing-primitives)
3. [Textures and Images](#textures-and-images)
4. [Text Rendering](#text-rendering)
5. [Colors](#colors)
6. [Transformations](#transformations)
7. [Best Practices](#best-practices)

---

## Getting Started

### Basic Setup

Every 2D graphics program needs:

```basic
REM Initialize window
INITWINDOW(800, 600, "My 2D Game")
SETTARGETFPS(60)

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    
    REM Your drawing code here
    
    ENDDRAW()
WEND

CLOSEWINDOW()
```

### Drawing Frame

All drawing must happen between `BEGINDRAW()` and `ENDDRAW()`:

```basic
BEGINDRAW()
    REM All drawing commands go here
ENDDRAW()
```

---

## Drawing Primitives

### Rectangles

```basic
REM Draw filled rectangle
DRAWRECTANGLE(100, 100, 200, 150, 255, 100, 100)

REM Draw rectangle outline
DRAWRECTANGLES(100, 100, 200, 150, 255, 255, 255)

REM Draw rounded rectangle
DRAWRECTANGLEROUNDED(100, 100, 200, 150, 10, 255, 100, 100)
```

**Parameters:** `x, y, width, height, r, g, b`

### Circles

```basic
REM Draw filled circle
DRAWCIRCLE(400, 300, 50, 100, 200, 255)

REM Draw circle outline
DRAWCIRCLES(400, 300, 50, 255, 255, 255)

REM Draw circle sector (pie slice)
DRAWCIRCLESECTOR(400, 300, 50, 0, 90, 100, 200, 255)
```

**Parameters:** `x, y, radius, r, g, b`

### Lines

```basic
REM Draw line
DRAWLINE(100, 100, 500, 400, 255, 255, 255)

REM Draw line with thickness
DRAWLINEEX(100, 100, 500, 400, 5, 255, 255, 255)

REM Draw bezier curve
DRAWLINEBEZIER(100, 100, 200, 200, 300, 100, 400, 200, 5, 255, 255, 255)
```

### Polygons

```basic
REM Draw triangle
DRAWTRIANGLE(400, 100, 300, 200, 500, 200, 255, 100, 100)

REM Draw polygon
VAR points = [Vector2(400, 100), Vector2(300, 200), Vector2(500, 200), Vector2(400, 150)]
DRAWPOLYGON(points, 255, 200, 100)
```

### Pixels

```basic
REM Draw single pixel
DRAWPIXEL(100, 100, 255, 255, 255)

REM Draw pixel with Vector2
VAR pos = Vector2(100, 100)
DRAWPIXELV(pos, 255, 255, 255)
```

---

## Textures and Images

### Loading Images

```basic
REM Load image from file
VAR image = LOADIMAGE("sprite.png")

REM Load texture from image
VAR texture = LOADTEXTUREFROMIMAGE(image)

REM Or load texture directly
VAR texture = LOADTEXTURE("sprite.png")
```

### Drawing Textures

```basic
REM Draw texture at position
DRAWTEXTURE(texture, 100, 100, 255, 255, 255)

REM Draw texture with rotation and scale
DRAWTEXTUREEX(texture, Vector2(100, 100), 45, 2.0, 255, 255, 255)

REM Draw part of texture (sprite sheet)
VAR source = Rectangle(0, 0, 32, 32)  REM x, y, width, height
VAR dest = Rectangle(100, 100, 64, 64)  REM Scaled destination
DRAWTEXTUREREC(texture, source, dest, 0, 0, 255, 255, 255)
```

### Sprite Sheets

```basic
REM Load sprite sheet
VAR spriteSheet = LOADTEXTURE("sprites.png")

REM Draw specific sprite from sheet
VAR spriteX = 0      REM Sprite X position in sheet
VAR spriteY = 0      REM Sprite Y position in sheet
VAR spriteWidth = 32
VAR spriteHeight = 32

VAR source = Rectangle(spriteX, spriteY, spriteWidth, spriteHeight)
VAR dest = Rectangle(100, 100, spriteWidth, spriteHeight)
DRAWTEXTUREREC(spriteSheet, source, dest, 0, 0, 255, 255, 255)
```

### Image Manipulation

```basic
REM Flip image
VAR flipped = IMAGEFLIPVERTICAL(image)

REM Rotate image
VAR rotated = IMAGEROTATE(image, 90)

REM Resize image
VAR resized = IMAGERESIZE(image, 200, 200)

REM Crop image
VAR cropped = IMAGECROP(image, Rectangle(10, 10, 100, 100))
```

---

## Text Rendering

### Basic Text

```basic
REM Draw text
DRAWTEXT("Hello, World!", 10, 10, 20, 255, 255, 255)

REM Draw text with position Vector2
VAR pos = Vector2(10, 10)
DRAWTEXTV("Hello, World!", pos, 20, 255, 255, 255)
```

**Parameters:** `text, x, y, fontSize, r, g, b`

### Text Formatting

```basic
REM Draw formatted text
VAR score = 100
DRAWTEXTFORMATTED("Score: %d", 10, 10, 20, 255, 255, 255, score)

REM Measure text width
VAR textWidth = MEASURETEXT("Hello", 20)
```

### Loading Fonts

```basic
REM Load font from file
VAR font = LOADFONT("font.ttf")

REM Draw text with custom font
DRAWTEXTEX(font, "Hello", Vector2(10, 10), 20, 2, 255, 255, 255)

REM Unload font when done
UNLOADFONT(font)
```

---

## Colors

### Color Constants

```basic
REM Predefined colors
DRAWRECTANGLE(100, 100, 50, 50, RED)
DRAWRECTANGLE(200, 100, 50, 50, GREEN)
DRAWRECTANGLE(300, 100, 50, 50, BLUE)
DRAWRECTANGLE(400, 100, 50, 50, WHITE)
DRAWRECTANGLE(500, 100, 50, 50, BLACK)
```

### Creating Colors

```basic
REM Using Color object
VAR myColor = Color(255, 100, 100, 255)  REM r, g, b, a
DRAWRECTANGLE(100, 100, 50, 50, myColor.r, myColor.g, myColor.b)

REM Direct RGB values
DRAWRECTANGLE(100, 100, 50, 50, 255, 100, 100)

REM Color from hex
VAR hexColor = ColorFromHex("#FF6464")
```

### Color Manipulation

```basic
REM Fade color
VAR faded = FADE(Color(255, 100, 100, 255), 0.5)  REM 50% opacity

REM Color brightness
VAR bright = ColorBrightness(Color(100, 100, 100, 255), 0.5)
```

---

## Transformations

### 2D Camera

```basic
REM Create 2D camera
VAR camera = Camera2D()
camera.target = Vector2(400, 300)  REM What to look at
camera.offset = Vector2(400, 300)  REM Screen offset
camera.rotation = 0
camera.zoom = 1.0

REM Use camera for drawing
BEGINDRAW()
BEGINDRAW2D(camera)
    REM Draw world coordinates here
ENDDRAW2D()
ENDDRAW()
```

### Transformations

```basic
REM Push transformation matrix
PUSHMATRIX()

REM Translate (move)
TRANSLATE(100, 100)

REM Rotate
ROTATE(45)

REM Scale
SCALE(2.0, 2.0)

REM Draw (will be transformed)
DRAWRECTANGLE(0, 0, 50, 50, 255, 255, 255)

REM Pop transformation
POPMATRIX()
```

---

## Complete 2D Game Example

```basic
REM Complete 2D Game Example
INITWINDOW(800, 600, "2D Game")
SETTARGETFPS(60)

REM Load assets
VAR playerTexture = LOADTEXTURE("player.png")
VAR bgTexture = LOADTEXTURE("background.png")

REM Game state
VAR playerX = 400
VAR playerY = 300
VAR playerSpeed = 5

WHILE NOT WINDOWSHOULDCLOSE()
    REM Input
    IF ISKEYDOWN(KEY_W) THEN playerY = playerY - playerSpeed
    IF ISKEYDOWN(KEY_S) THEN playerY = playerY + playerSpeed
    IF ISKEYDOWN(KEY_A) THEN playerX = playerX - playerSpeed
    IF ISKEYDOWN(KEY_D) THEN playerX = playerX + playerSpeed
    
    REM Draw
    BEGINDRAW()
    CLEARBACKGROUND(30, 30, 50)
    
    REM Draw background
    DRAWTEXTURE(bgTexture, 0, 0, 255, 255, 255)
    
    REM Draw player
    DRAWTEXTURE(playerTexture, playerX, playerY, 255, 255, 255)
    
    REM Draw UI
    DRAWTEXT("Position: " + STR(playerX) + ", " + STR(playerY), 10, 10, 20, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 35, 20, 255, 255, 255)
    
    ENDDRAW()
WEND

REM Cleanup
UNLOADTEXTURE(playerTexture)
UNLOADTEXTURE(bgTexture)
CLOSEWINDOW()
```

---

## Best Practices

### 1. Batch Drawing

Group similar draw calls together for better performance.

### 2. Reuse Textures

Load textures once, reuse them throughout the game.

### 3. Use Sprite Sheets

Combine multiple sprites into one texture for efficiency.

### 4. Limit Draw Calls

Minimize the number of draw operations per frame.

### 5. Clear Background

Always clear the background at the start of each frame.

### 6. Coordinate System

Remember: (0, 0) is top-left corner, Y increases downward.

### 7. Texture Sizes

Use power-of-2 texture sizes (32, 64, 128, 256, 512, 1024) when possible.

### 8. Unload Resources

Always unload textures and fonts when done.

---

## Common Patterns

### Sprite Animation

```basic
VAR frame = 0
VAR frameTime = 0
VAR frameDelay = 0.1  REM seconds per frame

WHILE NOT WINDOWSHOULDCLOSE()
    VAR deltaTime = GETDELTATIME()
    frameTime = frameTime + deltaTime
    
    IF frameTime >= frameDelay THEN
        frame = (frame + 1) MOD 4  REM 4 frames
        frameTime = 0
    ENDIF
    
    VAR spriteX = frame * 32  REM 32 pixels per frame
    VAR source = Rectangle(spriteX, 0, 32, 32)
    VAR dest = Rectangle(100, 100, 32, 32)
    DRAWTEXTUREREC(spriteSheet, source, dest, 0, 0, 255, 255, 255)
WEND
```

### Parallax Scrolling

```basic
VAR bgOffset1 = 0
VAR bgOffset2 = 0

WHILE NOT WINDOWSHOULDCLOSE()
    bgOffset1 = bgOffset1 - 1  REM Slow layer
    bgOffset2 = bgOffset2 - 2  REM Fast layer
    
    IF bgOffset1 < -800 THEN bgOffset1 = 0
    IF bgOffset2 < -800 THEN bgOffset2 = 0
    
    DRAWTEXTURE(bgLayer1, bgOffset1, 0, 255, 255, 255)
    DRAWTEXTURE(bgLayer1, bgOffset1 + 800, 0, 255, 255, 255)
    DRAWTEXTURE(bgLayer2, bgOffset2, 0, 255, 255, 255)
    DRAWTEXTURE(bgLayer2, bgOffset2 + 800, 0, 255, 255, 255)
WEND
```

---

## API Reference

### Drawing Functions

- `DRAWRECTANGLE(x, y, width, height, r, g, b)` - Draw filled rectangle
- `DRAWRECTANGLES(x, y, width, height, r, g, b)` - Draw rectangle outline
- `DRAWCIRCLE(x, y, radius, r, g, b)` - Draw filled circle
- `DRAWCIRCLES(x, y, radius, r, g, b)` - Draw circle outline
- `DRAWLINE(x1, y1, x2, y2, r, g, b)` - Draw line
- `DRAWTRIANGLE(x1, y1, x2, y2, x3, y3, r, g, b)` - Draw triangle
- `DRAWPIXEL(x, y, r, g, b)` - Draw pixel

### Texture Functions

- `LOADTEXTURE(filename)` - Load texture from file
- `DRAWTEXTURE(texture, x, y, r, g, b)` - Draw texture
- `DRAWTEXTUREEX(texture, position, rotation, scale, r, g, b)` - Draw with transform
- `UNLOADTEXTURE(texture)` - Free texture memory

### Text Functions

- `DRAWTEXT(text, x, y, fontSize, r, g, b)` - Draw text
- `MEASURETEXT(text, fontSize)` - Get text width
- `LOADFONT(filename)` - Load custom font
- `DRAWTEXTEX(font, text, position, fontSize, spacing, r, g, b)` - Draw with font

### Camera Functions

- `BEGINDRAW2D(camera)` - Start 2D camera mode
- `ENDDRAW2D()` - End 2D camera mode

---

For more examples, check the `examples/` directory!

