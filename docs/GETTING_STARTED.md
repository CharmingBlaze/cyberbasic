# Getting Started with CyberBasic

**Welcome to CyberBasic!** This guide will help you get up and running in minutes.

## Table of Contents
1. [Installation](#installation)
2. [Finding the Executable](#finding-the-executable)
3. [Your First Program](#your-first-program)
4. [Running Your Code](#running-your-code)
5. [Next Steps](#next-steps)

---

## Installation

### Option 1: Build from Source (Recommended)

#### Windows

1. **Install Prerequisites:**
   - [MSYS2](https://www.msys2.org/) with MinGW-w64
   - [CMake](https://cmake.org/download/) 3.25 or higher
   - [Python](https://www.python.org/downloads/) 3.10+ with PyYAML

2. **Clone and Build:**
   ```bash
   git clone https://github.com/CharmingBlaze/cyberbasic.git
   cd cyberbasic
   mkdir build-mingw && cd build-mingw
   cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
   cmake --build .
   ```

3. **The executable will be at:** `build-mingw/cyberbasic.exe`

#### Linux/macOS

1. **Install Prerequisites:**
   ```bash
   # Linux (Ubuntu/Debian)
   sudo apt install cmake g++ python3 python3-pip
   pip3 install pyyaml
   
   # macOS
   brew install cmake python3
   pip3 install pyyaml
   ```

2. **Clone and Build:**
   ```bash
   git clone https://github.com/CharmingBlaze/cyberbasic.git
   cd cyberbasic
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   make -j$(nproc)  # Linux
   # or
   make -j$(sysctl -n hw.ncpu)  # macOS
   ```

3. **The executable will be at:** `build/cyberbasic`

### Option 2: Use Pre-built Binary (When Available)

Check the [Releases](https://github.com/CharmingBlaze/cyberbasic/releases) page for pre-built binaries.

---

## Finding the Executable

After building, the executable location depends on your build directory:

### Windows
- **Development build:** `build/cyberbasic.exe` or `build-mingw/cyberbasic.exe`
- **Distribution build (static):** `build-dist/cyberbasic.exe` (recommended for distribution)
- **Custom build:** Check your `build` directory

### Linux/macOS
- **Default build:** `build/cyberbasic`
- **Custom build:** Check your `build` directory

### Quick Test

```bash
# Windows - Development build
cd build  # or cd build-mingw
.\cyberbasic.exe examples\hello_text.bas

# Windows - Distribution build (static linking, no DLLs)
cd build-dist
.\cyberbasic.exe ..\examples\hello_text.bas

# Linux/macOS
cd build
./cyberbasic ../examples/hello_text.bas
```

**Note:** For distribution, use `build-dist/cyberbasic.exe` which is statically linked and requires no DLL files.

---

## Your First Program

Create a file called `hello.bas`:

```basic
REM My First CyberBasic Program
INITWINDOW(800, 600, "Hello, CyberBasic!")
SETTARGETFPS(60)

WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(30, 30, 50)
    DRAWTEXT("Hello, World!", 300, 280, 40, 255, 255, 255)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

### What This Does

1. **`INITWINDOW(800, 600, "Hello, CyberBasic!")`** - Creates an 800x600 window
2. **`SETTARGETFPS(60)`** - Sets target frame rate to 60 FPS
3. **`WHILE NOT WINDOWSHOULDCLOSE()`** - Main game loop (runs until window closes)
4. **`BEGINDRAW()` / `ENDDRAW()`** - Start and end drawing frame
5. **`CLEARBACKGROUND(30, 30, 50)`** - Clear screen with dark blue color
6. **`DRAWTEXT(...)`** - Draw text on screen
7. **`CLOSEWINDOW()`** - Clean up and close window

---

## Running Your Code

### Basic Usage

```bash
# Windows
cyberbasic.exe your_program.bas

# Linux/macOS
./cyberbasic your_program.bas
```

### Running from Different Directories

```bash
# Windows - From project root
.\build-dist\cyberbasic.exe examples\hello_graphics.bas

# Windows - From build directory
cd build-dist
.\cyberbasic.exe ..\examples\hello_graphics.bas

# Linux/macOS - From project root
./build/cyberbasic examples/hello_graphics.bas

# Linux/macOS - From build directory
cd build
./cyberbasic ../examples/hello_graphics.bas
```

### Running Examples

The repository includes 60+ examples in the `examples/` directory:

```bash
# Windows - Using distribution build (recommended)
.\build-dist\cyberbasic.exe examples\hello_text.bas
.\build-dist\cyberbasic.exe examples\hello_graphics.bas
.\build-dist\cyberbasic.exe examples\simple_pong.bas
.\build-dist\cyberbasic.exe examples\ai_pong.bas
.\build-dist\cyberbasic.exe examples\space_invaders.bas
.\build-dist\cyberbasic.exe examples\model_viewer.bas

# Windows - Using development build
.\build\cyberbasic.exe examples\hello_graphics.bas
.\build\cyberbasic.exe examples\simple_pong.bas

# Linux/macOS
./build/cyberbasic examples/hello_text.bas
./build/cyberbasic examples/hello_graphics.bas
./build/cyberbasic examples/simple_pong.bas
./build/cyberbasic examples/space_invaders.bas
```

### Popular Examples to Try

- **`hello_text.bas`** - Simple text output
- **`hello_graphics.bas`** - Basic graphics demo
- **`simple_pong.bas`** - Classic Pong game
- **`ai_pong.bas`** - Pong with AI opponent
- **`space_invaders.bas`** - Complete Space Invaders game
- **`model_viewer.bas`** - 3D model viewer with mouse controls
- **`collision_demo.bas`** - Collision detection demo
- **`minimal_test.bas`** - Minimal window test

---

## Your First Game

Let's create a simple bouncing ball game:

```basic
REM Bouncing Ball Game
INITWINDOW(800, 600, "Bouncing Ball")
SETTARGETFPS(60)

REM Game variables
VAR ballX = 400
VAR ballY = 300
VAR speedX = 5
VAR speedY = 5

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Update ball position
    ballX = ballX + speedX
    ballY = ballY + speedY
    
    REM Bounce off walls
    IF ballX < 0 OR ballX > 790 THEN speedX = -speedX
    IF ballY < 0 OR ballY > 590 THEN speedY = -speedY
    
    REM Draw everything
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 30)
    DRAWCIRCLE(ballX, ballY, 20, 255, 100, 100)
    ENDDRAW()
WEND

CLOSEWINDOW()
```

Save this as `bouncing_ball.bas` and run it!

---

## Next Steps

Now that you're up and running, explore these resources:

### Essential Guides
- **[Game Development Guide](GAME_DEVELOPMENT_GUIDE.md)** - Complete guide to making games
- **[2D Graphics API Guide](2D_GRAPHICS_GUIDE.md)** - Learn 2D rendering
- **[3D Graphics API Guide](3D_GRAPHICS_GUIDE.md)** - Learn 3D rendering
- **[Quick Reference](QUICK_REFERENCE.md)** - Quick syntax reference

### Advanced Features
- **[Advanced Features Guide](ADVANCED_FEATURES_GUIDE.md)** - Enums, dot notation, state machines, ECS
- **[ECS System Guide](ECS_SYSTEM_GUIDE.md)** - Entity-Component-System architecture
- **[Modern State System Guide](MODERN_STATE_SYSTEM_GUIDE.md)** - State machine system

### Examples
- Check the `examples/` directory for 69+ example programs
- Try `pong_game.bas` for a complete game example
- Explore `space_defender.bas` for an advanced game

### Distribution
- **[Distribution Guide](DISTRIBUTION_GUIDE.md)** - How to distribute your games

---

## Common Issues

### "cyberbasic.exe not found"
- Make sure you're in the correct directory
- Check that the build completed successfully
- Use the full path to the executable

### "File not found"
- Check the file path is correct
- Use forward slashes `/` or double backslashes `\\` on Windows
- Make sure the `.bas` file exists

### Window doesn't appear
- Check for error messages in the console
- Make sure `INITWINDOW()` is called before the game loop
- Verify graphics drivers are up to date

### Performance issues
- Use `SETTARGETFPS(60)` to limit frame rate
- Reduce the number of draw calls
- Optimize your game loop

---

## Getting Help

- **Documentation:** Check the `docs/` folder for comprehensive guides
- **Examples:** Browse the `examples/` directory
- **Issues:** Open an issue on [GitHub](https://github.com/CharmingBlaze/cyberbasic/issues)
- **Community:** Join discussions and ask questions

---

**Happy coding!** Start simple, experiment, and build amazing games with CyberBasic! 🎮

