# CyberBasic Build Guide

## Quick Start

### Windows
```bash
build.bat
```

### Linux/macOS
```bash
chmod +x build.sh
./build.sh
```

### Python (Cross-platform)
```bash
python build.py
```

## Manual Build

### Prerequisites
- CMake 3.25+
- C++20 compiler (GCC 13+ or Clang 15+)
- Python 3.10+ with PyYAML

### Build Steps

1. **Create build directory:**
   ```bash
   mkdir build
   cd build
   ```

2. **Configure:**
   ```bash
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

3. **Build:**
   ```bash
   cmake --build . --config Release
   ```

4. **Run:**
   ```bash
   ./cyberbasic ../examples/hello_text.bas
   ```

## Build Options

### Module Selection
Disable optional modules to reduce build size:

```bash
cmake .. \
  -DBUILD_GRAPHICS_MODULE=OFF \
  -DBUILD_AUDIO_MODULE=OFF \
  -DBUILD_NETWORKING_MODULE=OFF
```

Available options:
- `BUILD_GRAPHICS_MODULE` (default: ON)
- `BUILD_AUDIO_MODULE` (default: ON)
- `BUILD_INPUT_MODULE` (default: ON)
- `BUILD_NETWORKING_MODULE` (default: ON)
- `BUILD_PHYSICS_MODULE` (default: ON)
- `BUILD_GUI_MODULE` (default: ON)
- `BUILD_3D_MODULE` (default: ON)
- `BUILD_AI_MODULE` (default: ON)
- `BUILD_GAME_MODULE` (default: ON)
- `BUILD_RAYMATH_MODULE` (default: ON)

### Static Linking (Windows/MinGW)
```bash
cmake .. -DBASIC_STATIC_LINK=ON
```

### Build Types
- `Release` - Optimized build (default)
- `Debug` - Debug symbols, no optimization
- `RelWithDebInfo` - Optimized with debug info

## Build Directory Structure

After building:
```
build/
├── cyberbasic.exe (or cyberbasic on Unix)
├── src/
│   └── bindings/
│       └── rt_raylib.gen.cpp (generated)
└── _deps/ (raylib source, auto-downloaded)
```

## Troubleshooting

### CMake not found
- Windows: Install from cmake.org or use MSYS2
- Linux: `sudo apt install cmake`
- macOS: `brew install cmake`

### Compiler not found
- Windows: Install MSYS2 MinGW-w64
- Linux: `sudo apt install build-essential`
- macOS: Install Xcode Command Line Tools

### Python/PyYAML not found
```bash
pip install PyYAML
```

### Build fails with "duplicate function"
- This is expected if FunctionRegistry detects duplicates
- Check CLEANUP_AUDIT.md for resolution

## Clean Build

Remove build directory and rebuild:
```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

## Cross-Platform Notes

### Windows
- Uses MinGW-w64 (recommended) or MSVC
- Static linking available via `BASIC_STATIC_LINK`

### Linux
- Requires OpenGL development headers
- Audio: ALSA or PulseAudio

### macOS
- Requires Xcode or Command Line Tools
- Frameworks: Cocoa, IOKit, OpenGL

---

**For more details, see DEPENDENCIES.md**

