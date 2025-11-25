# How to Use CyberBasic

**Complete guide to using the CyberBasic interpreter**

## Table of Contents
1. [Running Programs](#running-programs)
2. [Running Examples](#running-examples)
3. [Command Line Options](#command-line-options)
4. [File Paths](#file-paths)
5. [Distribution Build](#distribution-build)
6. [Troubleshooting](#troubleshooting)

---

> **Need the interpreter without compiling?** Download the latest `cyberbasic-windows` or `cyberbasic-linux` artifact from the [Build CyberBasic workflow](https://github.com/CharmingBlaze/cyberbasic/actions/workflows/build.yml). Each artifact contains the binary along with this guide.

## Running Programs

### Basic Syntax

```bash
cyberbasic.exe your_program.bas
```

### Windows Examples

```bash
# From project root using distribution build (recommended)
.\build-dist\cyberbasic.exe my_game.bas

# From project root using development build
.\build\cyberbasic.exe my_game.bas

# From build directory
cd build-dist
.\cyberbasic.exe ..\my_game.bas
```

### Linux/macOS Examples

```bash
# From project root
./build/cyberbasic my_game.bas

# From build directory
cd build
./cyberbasic ../my_game.bas

# Make executable and run directly
chmod +x build/cyberbasic
build/cyberbasic my_game.bas
```

---

## Running Examples

The `examples/` directory contains 60+ example programs demonstrating various features.

### Quick Start Examples

```bash
# Windows
.\build-dist\cyberbasic.exe examples\hello_text.bas
.\build-dist\cyberbasic.exe examples\hello_graphics.bas
.\build-dist\cyberbasic.exe examples\minimal_test.bas

# Linux/macOS
./build/cyberbasic examples/hello_text.bas
./build/cyberbasic examples/hello_graphics.bas
./build/cyberbasic examples/minimal_test.bas
```

### Game Examples

```bash
# Windows
.\build-dist\cyberbasic.exe examples\simple_pong.bas
.\build-dist\cyberbasic.exe examples\ai_pong.bas
.\build-dist\cyberbasic.exe examples\space_invaders.bas

# Linux/macOS
./build/cyberbasic examples/simple_pong.bas
./build/cyberbasic examples/ai_pong.bas
./build/cyberbasic examples/space_invaders.bas
```

### 3D Examples

```bash
# Windows
.\build-dist\cyberbasic.exe examples\model_viewer.bas

# Linux/macOS
./build/cyberbasic examples/model_viewer.bas
```

**Note:** The 3D model viewer requires a model file. Make sure `3Dmodels/object.obj` exists or modify the path in the code.

### Feature Examples

```bash
# Graphics and animation
.\build-dist\cyberbasic.exe examples\graphics_test.bas
.\build-dist\cyberbasic.exe examples\collision_demo.bas
.\build-dist\cyberbasic.exe examples\smooth_animation_demo.bas

# Language features
.\build-dist\cyberbasic.exe examples\var_const_demo.bas
.\build-dist\cyberbasic.exe examples\select_case_demo.bas
.\build-dist\cyberbasic.exe examples\raymath_test.bas
```

---

## Command Line Options

Currently, CyberBasic accepts one argument: the path to the `.bas` file to run.

```bash
cyberbasic.exe program.bas
```

Future versions may support:
- `--version` - Show version information
- `--help` - Show help message
- `--verbose` - Enable verbose output
- `--debug` - Enable debug mode

---

## File Paths

### Relative Paths

Paths are relative to the current working directory:

```bash
# If you're in the project root
.\build-dist\cyberbasic.exe examples\hello_text.bas

# If you're in the build-dist directory
.\cyberbasic.exe ..\examples\hello_text.bas
```

### Absolute Paths

You can also use absolute paths:

```bash
# Windows
C:\Users\YourName\CyberBasic\build-dist\cyberbasic.exe C:\Users\YourName\my_game.bas

# Linux/macOS
/home/username/CyberBasic/build/cyberbasic /home/username/my_game.bas
```

### Path Separators

- **Windows:** Use backslashes `\` or forward slashes `/`
- **Linux/macOS:** Use forward slashes `/`

```bash
# Windows - both work
.\build-dist\cyberbasic.exe examples\hello.bas
.\build-dist/cyberbasic.exe examples/hello.bas
```

---

## Distribution Build

### What is a Distribution Build?

A distribution build is a statically linked version of CyberBasic that includes all dependencies. This means:
- ✅ No DLL files required
- ✅ No Visual C++ Redistributables needed
- ✅ Works on any Windows system
- ✅ Ready to distribute with your games

### Creating a Distribution Build

**Windows:**
```bash
# Use the distribution build script
.\build-dist.bat

# The executable will be at: build-dist\cyberbasic.exe
```

**Linux/macOS:**
```bash
cmake -S . -B build-dist -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON
cmake --build build-dist

# The executable will be at: build-dist/cyberbasic
```

### Using Distribution Build

```bash
# Windows
.\build-dist\cyberbasic.exe your_game.bas

# Linux/macOS
./build-dist/cyberbasic your_game.bas
```

**Size:** The distribution build is approximately 10-11 MB (Windows) and includes all dependencies.

---

## Troubleshooting

### "cyberbasic.exe not found"

**Problem:** The executable cannot be found.

**Solutions:**
1. Check you're in the correct directory
2. Verify the build completed successfully
3. Use the full path to the executable
4. Check if you're using the correct build directory (`build/`, `build-dist/`, or `build-mingw/`)

```bash
# Windows - check if file exists
dir build-dist\cyberbasic.exe

# Linux/macOS - check if file exists
ls -lh build/cyberbasic
```

### "File not found" or "Cannot open file"

**Problem:** The `.bas` file cannot be found.

**Solutions:**
1. Check the file path is correct
2. Use forward slashes `/` or double backslashes `\\` on Windows
3. Make sure the `.bas` file exists
4. Use relative paths from the current directory

```bash
# Windows - check if file exists
dir examples\hello_text.bas

# Linux/macOS - check if file exists
ls examples/hello_text.bas
```

### "Permission denied" (Linux/macOS)

**Problem:** The executable doesn't have execute permissions.

**Solution:**
```bash
chmod +x build/cyberbasic
```

### Program runs but window doesn't appear

**Problem:** The program runs but no window is displayed.

**Solutions:**
1. Check for error messages in the console
2. Make sure `INITWINDOW()` is called before the game loop
3. Verify graphics drivers are up to date
4. Check if the program is waiting for input

### "DLL not found" errors

**Problem:** Missing DLL files when running the executable.

**Solutions:**
1. Use the distribution build (`build-dist/cyberbasic.exe`) which is statically linked
2. Or copy required DLLs from MinGW installation
3. Or install Visual C++ Redistributables (if using non-static build)

**Best solution:** Always use `build-dist/cyberbasic.exe` for distribution.

---

## Best Practices

### 1. Use Distribution Build for Sharing

Always use `build-dist/cyberbasic.exe` when:
- Distributing games to others
- Creating game packages
- Testing on clean systems

### 2. Organize Your Projects

Keep your projects organized:

```
MyGame/
├── cyberbasic.exe      (from build-dist/)
├── game.bas
├── assets/
│   ├── images/
│   ├── sounds/
│   └── models/
└── README.txt
```

### 3. Use Relative Paths

In your `.bas` files, use relative paths for assets:

```basic
REM Good - relative path
VAR texture = LOADTEXTURE("assets/images/player.png")

REM Avoid - absolute path (won't work on other systems)
VAR texture = LOADTEXTURE("C:\Users\YourName\MyGame\assets\images\player.png")
```

### 4. Test Before Distributing

Always test your game with the distribution build:

```bash
# Test your game
.\build-dist\cyberbasic.exe my_game.bas

# Test on a clean system (if possible)
# Copy to a different computer and test
```

---

## Quick Reference

### Common Commands

```bash
# Run a program
cyberbasic.exe program.bas

# Run an example
.\build-dist\cyberbasic.exe examples\hello_graphics.bas

# Run Pong game
.\build-dist\cyberbasic.exe examples\simple_pong.bas

# Run Space Invaders
.\build-dist\cyberbasic.exe examples\space_invaders.bas

# Run 3D Model Viewer
.\build-dist\cyberbasic.exe examples\model_viewer.bas
```

### Build Commands

```bash
# Create distribution build (Windows)
.\build-dist.bat

# Create development build (Windows)
.\build.bat

# Create distribution build (Linux/macOS)
cmake -S . -B build-dist -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON
cmake --build build-dist
```

---

## Next Steps

- **[Getting Started Guide](GETTING_STARTED.md)** - Installation and first steps
- **[Game Development Guide](GAME_DEVELOPMENT_GUIDE.md)** - Learn to make games
- **[Distribution Guide](DISTRIBUTION_GUIDE.md)** - How to distribute your games
- **[Examples](../examples/)** - Browse 60+ example programs

---

**Happy coding!** 🎮




