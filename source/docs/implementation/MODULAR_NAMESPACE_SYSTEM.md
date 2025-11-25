# Modular Namespace System for Dot Notation

## Overview

The namespace system provides a clean, modular way to organize raylib functions into logical groups accessible via dot notation. This makes the language more intuitive and easier to use, similar to modern game development languages.

## Architecture

### Components

1. **NamespaceRegistry** (`include/bas/namespace_registry.hpp`)
   - Central registry for all namespaces
   - Maps namespace.method() to actual function names
   - Creates namespace objects dynamically

2. **Namespace Registration** (`src/core/namespace_registry.cpp`)
   - `register_raylib_namespaces()` - Sets up all raylib namespaces
   - Easy to extend with new namespaces

3. **Interpreter Integration** (`src/core/interpreter.cpp`)
   - Resolves `Window.init()` to `INITWINDOW`
   - Handles namespace objects and method calls
   - Supports both old and new syntax

## Available Namespaces

### Window
```basic
Window.init(800, 600, "My Game")
Window.setTitle("New Title")
Window.setTargetFPS(60)
LET fps = Window.getFPS()
```

### Graphics (2D Drawing)
```basic
Graphics.beginDrawing()
Graphics.clearBackground(Color(0, 0, 0, 255))
Graphics.drawRectangle(100, 100, 200, 150, Color(255, 0, 0, 255))
Graphics.drawCircle(400, 300, 50, Color(0, 255, 0, 255))
Graphics.endDrawing()
```

### Graphics3D (3D Drawing)
```basic
Graphics3D.beginMode3D(camera)
Graphics3D.drawCube(Vector3(0, 0, 0), 2, 2, 2, Color(255, 0, 0, 255))
Graphics3D.drawSphere(Vector3(5, 0, 0), 1, Color(0, 255, 0, 255))
Graphics3D.endMode3D()
```

### Audio
```basic
Audio.init()
LET sound = Audio.loadSound("sound.wav")
Audio.playSound(sound)
Audio.setMasterVolume(0.5)
```

### Input
```basic
IF Input.isKeyPressed(KEY_SPACE) THEN
    PRINT "Space pressed!"
ENDIF

LET mouseX = Input.getMouseX()
LET mouseY = Input.getMouseY()
```

### Math
```basic
LET value = Math.clamp(x, 0, 100)
LET lerped = Math.lerp(start, end, 0.5)
```

### File
```basic
LET data = File.loadFileText("config.txt")
File.saveFileText("output.txt", data)
```

## Adding New Namespaces

To add a new namespace, simply update `src/core/namespace_registry.cpp`:

```cpp
void bas::register_raylib_namespaces(NamespaceRegistry& registry) {
    // ... existing namespaces ...
    
    // Add your new namespace
    registry.register_namespace("MyNamespace", {
        {"method1", "ACTUAL_FUNCTION_NAME1"},
        {"method2", "ACTUAL_FUNCTION_NAME2"},
        // ... more methods ...
    });
}
```

## Backward Compatibility

All old function names still work! Both syntaxes are supported:

```basic
REM Old syntax (still works)
INITWINDOW(800, 600, "Game")
DRAWRECTANGLE(100, 100, 200, 150, RED)

REM New syntax (preferred)
Window.init(800, 600, "Game")
Graphics.drawRectangle(100, 100, 200, 150, Color.red)
```

## Example: Complete Game Loop

```basic
REM Initialize window
Window.init(800, 600, "My Game")
Window.setTargetFPS(60)

REM Create camera
LET camera = Camera3D()
camera.position = Vector3(0, 10, 10)
camera.target = Vector3(0, 0, 0)
camera.up = Vector3(0, 1, 0)

REM Main loop
WHILE NOT Window.shouldClose()
    Graphics.beginDrawing()
    Graphics.clearBackground(Color(50, 50, 50, 255))
    
    REM 3D drawing
    Graphics3D.beginMode3D(camera)
    Graphics3D.drawCube(Vector3(0, 0, 0), 2, 2, 2, Color(255, 0, 0, 255))
    Graphics3D.endMode3D()
    
    REM 2D overlay
    Graphics.drawText("FPS: " + STR(Window.getFPS()), 10, 10, 20, Color(255, 255, 255, 255))
    
    Graphics.endDrawing()
WEND

Window.close()
```

## Benefits

1. **Modular**: Easy to add new namespaces
2. **Intuitive**: Clear organization of functions
3. **Backward Compatible**: Old code still works
4. **Extensible**: Simple to add new methods
5. **Clean**: No code duplication

## Implementation Details

- Namespaces are created as Value objects with `_type: "Namespace"`
- Methods are resolved at runtime via the registry
- No performance overhead - just a lookup table
- All existing functions remain accessible

