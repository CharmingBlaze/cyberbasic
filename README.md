# CyberBasic

![CyberBasic Logo](images/logo.png)

**The Modern BASIC Language for Game Development**

CyberBasic combines the simplicity and elegance of classic BASIC programming with the full power of modern game development. Write games, graphics applications, and interactive programs using familiar BASIC syntax while leveraging the complete Raylib graphics library.

##  Quick Start

### 1. Build CyberBasic

**Windows:**
```bash
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic
mkdir build-mingw && cd build-mingw
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

**Linux/macOS:**
```bash
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)  # Linux
# or make -j$(sysctl -n hw.ncpu)  # macOS
```

**The executable will be at:** `build-mingw/cyberbasic.exe` (Windows) or `build/cyberbasic` (Linux/macOS)

### 2. Run Your First Game

```bash
# Windows
cd build-mingw
./cyberbasic.exe ../pong_game.bas

# Linux/macOS
cd build
./cyberbasic ../pong_game.bas
```

### 3. Create Your Own Game

Create `my_game.bas`:

```basic
REM My First CyberBasic Game
INITWINDOW(800, 600, "My Game")
SETTARGETFPS(60)

VAR x = 400
VAR y = 300
VAR speed = 5

WHILE NOT WINDOWSHOULDCLOSE()
    IF ISKEYDOWN(KEY_W) THEN y = y - speed
    IF ISKEYDOWN(KEY_S) THEN y = y + speed
    IF ISKEYDOWN(KEY_A) THEN x = x - speed
    IF ISKEYDOWN(KEY_D) THEN x = x + speed
    
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    DRAWCIRCLE(x, y, 30, 255, 100, 100)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

Run it: `cyberbasic.exe my_game.bas`

---

##  Documentation

### Essential Guides
- **[Getting Started Guide](docs/GETTING_STARTED.md)** - Installation, finding the executable, first steps
- **[Game Development Guide](docs/GAME_DEVELOPMENT_GUIDE.md)** - Complete guide to making games
- **[2D Graphics API Guide](docs/2D_GRAPHICS_GUIDE.md)** - Learn 2D rendering, sprites, textures
- **[3D Graphics API Guide](docs/3D_GRAPHICS_GUIDE.md)** - Learn 3D rendering, models, cameras, lighting
- **[Distribution Guide](docs/DISTRIBUTION_GUIDE.md)** - How to distribute your games

### Quick References
- **[Quick Reference](docs/QUICK_REFERENCE.md)** - Quick syntax reference
- **[BASIC Programming Guide](docs/BASIC_PROGRAMMING_GUIDE.md)** - Complete language reference

### Advanced Features
- **[Advanced Features Guide](docs/ADVANCED_FEATURES_GUIDE.md)** - Enums, dot notation, state machines, ECS, coroutines, tuples
- **[ECS System Guide](docs/ECS_SYSTEM_GUIDE.md)** - Entity-Component-System architecture
- **[Modern State System Guide](docs/MODERN_STATE_SYSTEM_GUIDE.md)** - State machine system

---

##  What CyberBasic Offers

### Complete Modern BASIC Language

**Case-Insensitive:** CyberBasic is case-insensitive, just like classic BASIC. Keywords and identifiers can be written in any case:
```basic
VAR x = 10              REM All of these work the same:
var y = 20              REM VAR, var, Var are equivalent
Var z = 30              REM myVar, MyVar, MYVAR refer to same variable
```

**Variables & Constants:**
```basic
VAR x = 10              REM Modern variable (can be used anywhere)
LET y = 20              REM Classic variable
CONST PI = 3.14159      REM Immutable constant
```

**Arrays:**
```basic
DIM arr[10]             REM Classic style
VAR items = []           REM Modern empty array
VAR matrix[5, 10]       REM 2D array

REM Advanced operations
arr.insert(6)           REM Insert element
arr.remove()            REM Remove last
arr.sort()              REM Sort array
arr.find(5)             REM Find element
```

**Dictionaries:**
```basic
VAR dict = {"key": "value", 2: 3}           REM JSON-style
VAR other = {key = "value", other_key = 2}   REM BASIC-style
```

**Types:**
```basic
TYPE Player
    x AS NUMBER
    y AS NUMBER
    health AS INTEGER
END TYPE

VAR player = Player()
player.x = 100
player.y = 200
```

**Functions:**
```basic
FUNCTION add(a, b) AS INTEGER
    RETURN a + b
END FUNCTION

FUNCTION getSize() AS tuple
    RETURN (800, 600)
END FUNCTION

VAR (w, h) = getSize()  REM Tuple destructuring
```

**Control Flow:**
```basic
IF condition THEN
    REM code
ELSEIF other THEN
    REM code
ELSE
    REM code
ENDIF

WHILE condition
    REM code
WEND

FOR i = 1 TO 10
    REM code
NEXT

SELECT CASE value
    CASE 1
        REM code
    CASE 2
        REM code
END SELECT
```

**Dot Notation:**
```basic
VAR pos = Vector2(100, 200)
pos.x = pos.x + 5
PRINT "X: " + STR(pos.x)

VAR color = Color(255, 100, 100, 255)
PRINT "R: " + STR(color.r)
```

### 527 Raylib Functions

**2D Graphics:**
- Primitives: rectangles, circles, lines, triangles, polygons
- Textures: loading, drawing, sprite sheets
- Text: fonts, formatting, measurements
- Camera: 2D camera system

**3D Graphics:**
- Primitives: cubes, spheres, planes, grids
- Models: loading, animation, transformations
- Camera: first-person, third-person, orthographic
- Lighting: directional, point, spot lights
- Materials: textures, PBR materials

**Audio:**
- Sound effects: loading, playing, 3D spatial audio
- Music: streaming, looping, volume control

**Input:**
- Keyboard: key states, key codes
- Mouse: position, buttons, wheel
- Gamepad: controller support

**Window Management:**
- Window creation, resizing, fullscreen
- Multi-monitor support
- Window icons and titles

### Advanced Game Development Features

**ECS (Entity-Component-System):**
```basic
COMPONENT Position {x = 0, y = 0}
COMPONENT Velocity {dx = 0, dy = 0}

VAR player = ENTITY()
player.ADD(Position(100, 200))
player.ADD(Velocity(2, 0))

SYSTEM Movement(Position, Velocity)
    Position.x += Velocity.dx
    Position.y += Velocity.dy
END SYSTEM

RUN Movement
```

**State Machines:**
```basic
STATE Idle
    ON UPDATE
        REM idle logic
    END
    TRANSITION TO Walking WHEN keyPressed
END STATE
```

**Coroutines:**
```basic
FUNCTION animate() AS coroutine
    FOR i = 1 TO 10
        PRINT "Step " + i
        YIELD
    NEXT
END FUNCTION
```

**Timers:**
```basic
TIMER(1000, repeat = true) -> PRINT "Tick every second"
WAIT(2.0)  REM Pause for 2 seconds
```

**Tuples:**
```basic
VAR pos = (100, 200)
VAR (x, y) = pos  REM Destructuring
```

**Enums:**
```basic
ENUM Direction
    UP
    DOWN
    LEFT
    RIGHT
END ENUM

ENUM Status
    IDLE = 0
    WALKING = 1
    RUNNING = 2
END ENUM
```

---

##  Examples

The repository includes **69+ example programs** in the `examples/` directory:

### Game Examples
- `pong_game.bas` - Complete Pong game with AI
- `space_defender.bas` - Space shooter with AI enemies
- `2d_game_template.bas` - 2D game template
- `3d_game_template.bas` - 3D game template
- `complete_game_demo.bas` - Full game example

### Feature Examples
- `hello_graphics.bas` - Basic graphics
- `smooth_animation_demo.bas` - Smooth animations
- `audio_demo.bas` - Audio system
- `gui_demo.bas` - GUI system
- `physics_demo.bas` - Physics simulation
- `models3d_demo.bas` - 3D models
- `complete_3d_game_demo.bas` - Complete 3D game

### Language Examples
- `var_const_demo.bas` - Variables and constants
- `dot_notation_demo.bas` - Dot notation
- `dictionary_literal_demo.bas` - Dictionary syntax
- `select_case_demo.bas` - Control flow

Run any example:
```bash
cyberbasic.exe examples/hello_graphics.bas
```

---

##  Architecture

CyberBasic is built with modern C++20 and follows clean architecture principles:

### Core Components
- **Lexer**: Converts BASIC source code into tokens
- **Parser**: Builds an Abstract Syntax Tree (AST)
- **Interpreter**: Executes the AST with full runtime support
- **Function Registry**: Manages built-in and user-defined functions
- **Code Generator**: Auto-generates Raylib bindings from YAML specifications

### File Structure
```
cyberbasic/
├── src/                    # Core interpreter implementation
│   ├── core/              # Lexer, parser, interpreter
│   ├── modules/           # Feature modules (ECS, graphics, audio, etc.)
│   └── bindings/          # Generated Raylib bindings
├── include/bas/           # Public headers
├── specs/                 # YAML function specifications
├── tools/                 # Build and code generation tools
├── examples/              # 69+ example programs
├── docs/                  # Comprehensive documentation
└── images/                # Logo and screenshots
```

---

##  Building from Source

### Prerequisites
- **CMake** 3.25 or higher
- **C++20 compiler**: MinGW-w64 GCC 13+ (Windows) or GCC/Clang (Linux/macOS)
- **Python** 3.10+ with PyYAML

### Windows Build
```bash
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic
mkdir build-mingw && cd build-mingw
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Linux/macOS Build
```bash
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)  # Linux
# or make -j$(sysctl -n hw.ncpu)  # macOS
```

### Static Linking (Windows)
For distribution without DLL dependencies:
```bash
cmake .. -DBASIC_STATIC_LINK=ON -DCMAKE_BUILD_TYPE=Release
```

---

##  Language Features

### Core Language
- **Case-Insensitive**: Keywords and identifiers can be written in any case (`VAR`, `var`, `Var` all work)
- **Variables**: `VAR`, `LET`, and `CONST` declarations
- **Arrays**: Classic `DIM` and modern `VAR arr[10]` syntax
- **Array Operations**: `.length`, `.insert()`, `.remove()`, `.sort()`, `.find()`, `.reverse()`, `.swap()`
- **Dictionaries**: `{"key": "value"}` or `{key = "value"}` syntax
- **Types**: User-defined types with `TYPE ... ENDTYPE` and JSON serialization
- **Functions**: Parameters, return values, `ENDFUNCTION value`, tuple returns
- **Control Flow**: IF/THEN/ELSE/ELSEIF, WHILE/WEND, FOR/NEXT, SELECT CASE, GOSUB/GOTO/RETURN
- **Operators**: Arithmetic, relational, boolean, and bitwise operators
- **Dot Notation**: Access object properties and methods
- **Module System**: `IMPORT "file.bas"` or `INCLUDE "file.bas"`
- **Error Handling**: Comprehensive error messages with line numbers

### Game Development Features
- **ECS System**: Entity-Component-System for game architecture
- **State Machine**: Modern BASIC-style state system
- **Sprite System**: Sprite creation, animation, and management
- **Collision Detection**: Built-in collision checking
- **Camera System**: 2D and 3D camera management
- **Input Events**: Keyboard, mouse, and gamepad handling
- **Timer System**: Game timing, delta time, scheduled timers
- **Animation System**: Sprite and object animation

### Advanced Features
- **Coroutines**: Suspension and resumption with `YIELD` and `AWAIT`
- **Tuples**: Lightweight grouped values with destructuring
- **Enums**: Named and unnamed enums with custom values
- **Component Defaults**: Default values in component declarations
- **System Priority**: Priority-based system execution
- **JSON Support**: Serialize/deserialize arrays and types
- **Debug Support**: Built-in debugging capabilities

---

##  Use Cases

**Perfect for:**
- Learning programming fundamentals
- Rapid game prototyping
- Educational projects
- Retro-style game development
- Anyone who wants to code without complexity
- 2D and 3D game development
- Interactive applications

---

##  Distribution

See the **[Distribution Guide](docs/DISTRIBUTION_GUIDE.md)** for complete instructions on distributing your games.

**Quick summary:**
1. Build with static linking: `cmake .. -DBASIC_STATIC_LINK=ON`
2. Package executable, `.bas` files, and assets
3. Create launcher script
4. Test on clean system
5. Distribute as zip or installer

---

##  Performance

CyberBasic is designed for both learning and production use:
- **Fast startup**: Programs begin execution immediately
- **Efficient execution**: Optimized interpreter with minimal overhead
- **Memory safe**: Automatic memory management with no leaks
- **Cross-platform**: Consistent performance across Windows, Linux, and macOS

---

##  Contributing

We welcome contributions! Whether you're fixing bugs, adding features, or improving documentation:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

See [CONTRIBUTING.md](docs/CONTRIBUTING.md) for details.

---

##  License

This project is open source. See the LICENSE file for details.

---

##  Links

- **Documentation**: [docs/](docs/)
- **Examples**: [examples/](examples/)
- **Issues**: [GitHub Issues](https://github.com/CharmingBlaze/cyberbasic/issues)
- **Releases**: [GitHub Releases](https://github.com/CharmingBlaze/cyberbasic/releases)

---

## 📚 Documentation Index

### Getting Started
- [Getting Started Guide](docs/GETTING_STARTED.md) - Installation and first steps
- [Quick Reference](docs/QUICK_REFERENCE.md) - Quick syntax reference

### Game Development
- [Game Development Guide](docs/GAME_DEVELOPMENT_GUIDE.md) - Complete game development guide
- [2D Graphics API Guide](docs/2D_GRAPHICS_GUIDE.md) - 2D rendering, sprites, textures
- [3D Graphics API Guide](docs/3D_GRAPHICS_GUIDE.md) - 3D rendering, models, cameras, lighting
- [Distribution Guide](docs/DISTRIBUTION_GUIDE.md) - How to distribute your games

### Language Reference
- [BASIC Programming Guide](docs/BASIC_PROGRAMMING_GUIDE.md) - Complete language reference
- [Advanced Features Guide](docs/ADVANCED_FEATURES_GUIDE.md) - Enums, dot notation, state machines, ECS, coroutines

### Systems
- [ECS System Guide](docs/ECS_SYSTEM_GUIDE.md) - Entity-Component-System architecture
- [Modern State System Guide](docs/MODERN_STATE_SYSTEM_GUIDE.md) - State machine system

---

## 🎮 Example: Complete 2D Game

```basic
REM Complete 2D Game Example
INITWINDOW(800, 600, "My 2D Game")
SETTARGETFPS(60)

REM Load assets
VAR playerTexture = LOADTEXTURE("player.png")

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
    DRAWTEXTURE(playerTexture, playerX, playerY, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 20, 255, 255, 255)
    ENDDRAW()
WEND

UNLOADTEXTURE(playerTexture)
CLOSEWINDOW()
```

---

## 🎮 Example: Complete 3D Game

```basic
REM Complete 3D Game Example
INITWINDOW(800, 600, "My 3D Game")
SETTARGETFPS(60)

REM Setup 3D camera
VAR camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
camera.target = VECTOR3(0, 0, 0)
camera.up = VECTOR3(0, 1, 0)
camera.fovy = 45
camera.projection = CAMERA_PERSPECTIVE

REM Load model
VAR model = LOADMODEL("character.obj")

VAR rotation = 0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 1
    
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
    
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 20, 255, 255, 255)
    ENDDRAW()
WEND

UNLOADMODEL(model)
CLOSEWINDOW()
```

---

## 🎯 Roadmap

- Enhanced error messages with line numbers and suggestions
- Integrated debugger with breakpoints and variable inspection
- IDE integration with syntax highlighting and autocomplete
- Additional BASIC dialects (QBASIC, Visual Basic compatibility)
- Performance optimizations and JIT compilation
- Web-based editor and runtime

---

## 💬 Support

For questions, bug reports, or feature requests:
- **Issues**: [Open an issue on GitHub](https://github.com/CharmingBlaze/cyberbasic/issues)
- **Documentation**: Check the [docs/](docs/) folder
- **Examples**: Browse the [examples/](examples/) directory

---

**CyberBasic** - Where classic programming meets modern game development. Start coding games today, not tomorrow. 🎮

---

<div align="center">

**Made with ❤️ for game developers**

[Getting Started](docs/GETTING_STARTED.md) • [Documentation](docs/) • [Examples](examples/) • [Contributing](docs/CONTRIBUTING.md)

</div>
