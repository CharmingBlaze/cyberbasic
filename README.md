# CyberBasic

A fully functional, modular BASIC programming language interpreter with 100% Raylib integration for modern game development.

## Features

- **Complete BASIC Language**: Variables, arrays, functions, control flow (IF/THEN/ENDIF, WHILE/WEND, FOR/NEXT, etc.)
- **517 Raylib Functions**: Full graphics, audio, input, 3D models, shaders, and physics integration
- **Modern C++17 Architecture**: Clean, modular design with strict error handling
- **Zero-Duplication Policy**: All Raylib bindings auto-generated from YAML specification
- **Multiplatform Support**: Windows (tested), Linux, macOS ready
- **Professional Build System**: CMake ≥3.20 with MinGW-w64 GCC 13+

## Quick Start

### Prerequisites

- CMake ≥3.20
- MinGW-w64 GCC 13+ (Windows) or GCC/Clang (Linux/macOS)
- Python 3.10+ (for code generation)
- PyYAML

### Building

```bash
# Clone the repository
git clone https://github.com/yourusername/cyberbasic.git
cd cyberbasic

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..

# Build
ninja

# Run
./cyberbasic.exe test.bas
```

### Windows Build (MinGW-w64)

```bash
mkdir build_mingw && cd build_mingw
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./cyberbasic.exe demo.bas
```

## Example Programs

### Basic Math and Variables
```basic
PRINT "Hello, CyberBasic!"
LET x = 10
LET y = 20
LET sum = x + y
PRINT "10 + 20 = 30"
```

### Control Flow
```basic
LET score = 0
FOR i = 1 TO 5
    LET score = score + 10
NEXT i

IF score > 30 THEN
    PRINT "High score achieved!"
ELSE
    PRINT "Keep trying!"
ENDIF
```

### Raylib Graphics
```basic
PRINT "Creating graphics window..."
INITWINDOW(800, 600, "CyberBasic Demo")
PRINT "Window created successfully!"
CLOSEWINDOW()
PRINT "Graphics system ready!"
```

## Game Development Features

- **Graphics**: 2D/3D rendering, textures, models, shaders
- **Audio**: Sound effects, music, 3D audio
- **Input**: Keyboard, mouse, gamepad support
- **Physics**: Collision detection, rigid body physics
- **Networking**: Multiplayer support
- **File I/O**: Save/load game data

## Architecture

### Core Components

- **Lexer** (`src/lexer.cpp`): Tokenizes BASIC source code
- **Parser** (`src/parser.cpp`): Builds Abstract Syntax Tree (AST)
- **Interpreter** (`src/interpreter.cpp`): Executes AST with runtime environment
- **Builtins** (`src/builtins.cpp`): Core BASIC functions and Raylib wrappers
- **Code Generator** (`tools/gen_raylib_bindings.py`): Auto-generates Raylib bindings

### File Structure

```
cyberbasic/
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── lexer.cpp          # Tokenizer
│   ├── parser.cpp         # AST builder
│   ├── interpreter.cpp    # Runtime engine
│   ├── builtins.cpp       # Built-in functions
│   ├── runtime.cpp        # Runtime utilities
│   ├── input.cpp          # Input handling
│   └── rt_raylib.gen.cpp  # Generated Raylib bindings
├── include/bas/           # Headers
├── specs/                 # YAML specifications
├── tools/                 # Build tools
├── examples/              # Sample programs
└── CMakeLists.txt         # Build configuration
```

## Testing

Run the included test programs:

```bash
./cyberbasic.exe test.bas          # Basic functionality
./cyberbasic.exe demo.bas          # Complete demonstration
./cyberbasic.exe advanced_test.bas # Advanced features
./cyberbasic.exe raylib_test.bas   # Raylib integration
```

## Supported BASIC Features

- **Variables**: `LET x = 10` or `x = 10`
- **Arrays**: `DIM arr(10)`, `arr(5) = 100`
- **Functions**: `SUB`, `FUNCTION`, `RETURN`
- **Control Flow**: `IF/THEN/ENDIF`, `WHILE/WEND`, `FOR/NEXT`, `DO/LOOP`
- **File I/O**: `OPEN`, `CLOSE`, `READ`, `WRITE`, `INPUT`
- **Built-in Functions**: Math, string manipulation, file operations

### Scoping and Strict Mode (Optional)

CyberBasic supports optional strict declaration and explicit scope controls. These are opt-in and maintain backward compatibility:

- OPTION EXPLICIT: Enable strict undeclared-variable checks per file.
- LOCAL: Declare names as locals within the current SUB/FUNCTION (shadows outer/global).
- GLOBAL: Within a SUB/FUNCTION, bind names to the program's root/global environment.

Example:

```basic
OPTION EXPLICIT

GLOBAL score
LET score = 0

SUB AddPoints(p)
  LOCAL temp
  LET temp = p
  GLOBAL score
  LET score = score + temp
ENDSUB

CALL AddPoints(10)
PRINT score  ' 10
```

See `examples/local_global_demo.bas` for a complete demo.

## Raylib Integration

All 517 Raylib functions are available:

- **Window Management**: `INITWINDOW`, `CLOSEWINDOW`, `SETWINDOWTITLE`
- **Graphics**: `DRAWCIRCLE`, `DRAWRECTANGLE`, `DRAWTEXTURE`
- **3D Graphics**: `DRAWMODEL`, `DRAWCUBE`, `DRAWSPHERE`
- **Audio**: `LOADSOUND`, `PLAYSOUND`, `STOPSOUND`
- **Input**: `ISKEYDOWN`, `GETMOUSEX`, `ISGAMEPADBUTTONPRESSED`
- **Physics**: `LOADPHYSICSBODY`, `PHYSICSBODYFORCE`

## Development

### Adding New Raylib Functions

1. Add function specification to `specs/raylib_api.yaml`
2. Run `python tools/gen_raylib_bindings.py`
3. Rebuild with `ninja`

### Code Style

- C++17 standard
- Strict warnings (`-Wall -Wextra -Werror -Wpedantic`)
- Files <250 LOC where feasible
- Single responsibility principle
- No global static initialization

## License

This project is open source. See LICENSE file for details.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests
5. Submit a pull request

## Roadmap

- [ ] Enhanced error messages
- [ ] Debugger support
- [ ] IDE integration
- [ ] More BASIC dialects
- [ ] Performance optimizations

## Support

For questions, issues, or contributions, please open an issue on GitHub.

---

**CyberBasic** - Bringing the simplicity of BASIC to modern game development!
