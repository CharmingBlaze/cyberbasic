# CyberBasic Build Guide - Complete Integration & Compilation

This guide ensures CyberBasic compiles successfully with all features working together for 2D/3D game development.

## Quick Start

**Windows (Recommended):**
```powershell
cd source
.\build-robust.ps1 -Clean -Static
```

This uses the robust build script that automatically handles generator mismatches and file locks.

**Linux/macOS:**
```bash
cd source
./build-static-linux.sh    # Linux
./build-static-macos.sh    # macOS
```

**Universal (Python):**
```bash
cd source
python build.py --clean --static
```

## System Architecture Overview

CyberBasic integrates multiple systems that must work together:

```
┌─────────────────────────────────────────────────────────┐
│                    YAML Specifications                 │
│  (65+ files in specs/) defining Raylib functions        │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│          Code Generator (gen_raylib_bindings.py)       │
│  Processes YAML → Generates rt_raylib.gen.cpp         │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│         Generated Bindings (rt_raylib.gen.cpp)         │
│  C++ functions that bridge BASIC ↔ Raylib             │
└────────────────────┬────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────┐
│              Runtime Integration                        │
│  • FunctionRegistry: Registers all functions          │
│  • NativeFunctionRegistry: For YAML module references  │
│  • YamlModuleLoader: Runtime YAML module loading      │
│  • Type conversions: Value ↔ Raylib types              │
└─────────────────────────────────────────────────────────┘
```

## Build Process Steps

### 1. Prerequisites Check

**Required:**
- **CMake** 3.10+ (`cmake --version`)
- **Python** 3.6+ (`python --version`)
- **C++ Compiler**: MinGW-w64 (Windows), GCC/Clang (Linux/macOS)
- **Git** (for cloning)

**Windows Setup:**
```powershell
# Install MSYS2 (includes MinGW-w64)
# Download from: https://www.msys2.org/
# Then install tools:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-python
```

**Linux Setup:**
```bash
sudo apt update
sudo apt install build-essential cmake python3 python3-pip
```

**macOS Setup:**
```bash
xcode-select --install
brew install cmake python3
```

### 2. Clean Build (Recommended First Time)

**Why clean?** Removes stale object files and ensures fresh generation:

```powershell
# Windows PowerShell
cd source
Remove-Item -Recurse -Force build* -ErrorAction SilentlyContinue
.\build-complete-static-enhanced.ps1
```

```bash
# Linux/macOS
cd source
rm -rf build*
./build-static-linux.sh    # or build-static-macos.sh
```

### 3. Build Process Breakdown

The build system automatically:

1. **Configures CMake** - Sets up build environment
2. **Generates Bindings** - Runs `gen_raylib_bindings.py` to create `rt_raylib.gen.cpp`
3. **Compiles Core** - Builds interpreter, parser, runtime
4. **Compiles Modules** - Graphics, audio, 3D, networking, etc.
5. **Links Everything** - Creates final `cyberbasic.exe`

### 4. Troubleshooting Build Issues

#### Generator Mismatch Errors

**Problem:** `Generator mismatch detected! Cached: Visual Studio 16 2019, Current: MinGW Makefiles`

**Cause:** Build directory was configured with one generator (e.g., MSVC) but script tries to use another (e.g., MinGW).

**Solution:** The robust build script (`build-robust.ps1`) automatically detects and fixes this:
```powershell
cd source
.\build-robust.ps1 -Clean
```

**Manual fix:**
```powershell
# Delete build directory completely
Remove-Item -Recurse -Force build
# Then rebuild
.\build-robust.ps1
```

**Prevention:** Always use the same generator. The build script uses MinGW Makefiles by default.

#### File Lock Errors

**Problem:** `fatal error LNK1104: cannot open file 'rt_raylib.gen.obj': Permission denied`

**Cause:** File is locked by:
- IDE/Editor (VS Code, Visual Studio)
- Previous build process still running
- Antivirus scanner
- File system not released yet

**Solutions:**

1. **Use robust build script (automatic):**
   ```powershell
   .\build-robust.ps1 -Clean
   ```
   This automatically kills locking processes and cleans directories.

2. **Manual process cleanup:**
   ```powershell
   # Kill all build-related processes
   Get-Process | Where-Object {
       $_.ProcessName -in @("cl", "link", "mspdbsrv", "gcc", "g++", "cmake", "make")
   } | Stop-Process -Force
   ```

3. **Close IDEs:** Close VS Code, Visual Studio, or any editor with the project open

4. **Clean build directory:**
   ```powershell
   Remove-Item -Recurse -Force build
   Start-Sleep -Seconds 2  # Wait for filesystem
   .\build-robust.ps1
   ```

#### CMake Configuration Errors

**Problem:** `CMake Error: The source directory ... does not appear to contain CMakeLists.txt`

**Cause:** Running CMake from wrong directory or build directory corruption.

**Solution:**
```powershell
# Ensure you're in the source directory
cd source
# Clean and rebuild
.\build-robust.ps1 -Clean
```

## Integration Points Verification

### ✅ YAML → Code Generator

**Location:** `source/tools/gen_raylib_bindings.py`

**What it does:**
- Reads all `.yaml` files from `specs/`
- Generates C++ bindings in `rt_raylib.gen.cpp`
- Handles type conversions (Vector2, Color, Camera3D, etc.)
- Creates `populate_raylib_natives()` function

**Verify:**
```bash
cd source/build
python ../tools/gen_raylib_bindings.py
# Should generate: src/bindings/rt_raylib.gen.cpp
```

### ✅ Generated Bindings → Runtime

**Location:** `source/src/bindings/rt_raylib.gen.cpp`

**What it provides:**
- `register_raylib_bindings(FunctionRegistry&)` - Registers all functions
- `populate_raylib_natives(NativeFunctionRegistry&, FunctionRegistry&)` - For YAML modules

**Verify:** Check that file exists and contains both functions.

### ✅ Runtime Integration

**Location:** `source/src/main.cpp` (lines 680-804)

**Integration flow:**
```cpp
// 1. Register raylib bindings
bas::register_raylib_bindings(R);

// 2. Populate native registry (for YAML modules)
bas::NativeFunctionRegistry native_registry;
bas::populate_raylib_natives(native_registry, R);

// 3. Load YAML modules (if enabled)
bas::YamlModuleLoader yaml_loader(R, &namespace_registry, &type_registry, &native_registry, loader_options);
yaml_loader.load_modules_from_directory(modules_path);
```

### ✅ Type Conversions

**Location:** 
- `source/include/bas/raymath_helpers.hpp` - Vector2, Vector3, Camera2D, Camera3D
- `source/include/bas/raygui_helpers.hpp` - Rectangle, Color

**Verify:** All conversion functions exist:
- `Value_to_Vector2`, `Vector2_to_Map`
- `Value_to_Camera3D`, `Camera3D_to_Map`
- `Value_to_Color`, `Color_to_Map`
- etc.

## Build Configuration Options

### Static Linking (Default on Windows)

Creates standalone executable with no DLL dependencies:

```bash
cmake .. -DBASIC_STATIC_LINK=ON -DCMAKE_BUILD_TYPE=Release
```

### Debug Build

Includes debug symbols and assertions:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### Module Selection

Disable specific modules for minimal builds:

```bash
cmake .. -DBUILD_AUDIO_MODULE=OFF -DBUILD_NETWORKING_MODULE=OFF
```

## Verification Checklist

After building, verify:

- [ ] `cyberbasic.exe` exists in `build/` directory
- [ ] Can run: `.\build\cyberbasic.exe --help`
- [ ] Can run example: `.\build\cyberbasic.exe ..\examples\hello_text.bas`
- [ ] All raylib functions accessible (test with simple program)
- [ ] YAML modules can be loaded (if using runtime modules)

## Common Issues & Solutions

### Issue: "Python not found"
**Solution:** Install Python 3.6+ and ensure it's in PATH

### Issue: "CMake not found"
**Solution:** Install CMake and add to PATH, or use package manager

### Issue: "MinGW not found" (Windows)
**Solution:** Install MSYS2 and MinGW-w64 toolchain

### Issue: Binding generation fails
**Solution:** 
1. Check Python version: `python --version` (need 3.6+)
2. Check YAML files exist: `ls source/specs/*.yaml`
3. Run generator manually: `cd source/build && python ../tools/gen_raylib_bindings.py`

### Issue: Linking errors for networking functions
**Solution:** Already fixed - networking functions are exported (not static)

### Issue: Missing Camera2D/Camera3D conversions
**Solution:** Already fixed - added to `raymath_helpers.hpp`

### Issue: Generator Mismatch
**Symptoms:** 
- `Generator mismatch detected!` error from CMake
- Build fails with confusing errors after switching generators

**Solution:** 
1. Use `build-robust.ps1` which auto-detects and fixes mismatches
2. Or manually: `Remove-Item -Recurse -Force build` then rebuild
3. Always use the same generator (MinGW Makefiles recommended)

### Issue: File Lock / Permission Denied
**Symptoms:**
- `fatal error LNK1104: cannot open file`
- `Permission denied` errors
- Build fails randomly

**Solution:**
1. Use `build-robust.ps1 -Clean` (handles this automatically)
2. Close all IDEs and editors
3. Kill build processes: `Get-Process cl,link,gcc,g++,cmake | Stop-Process -Force`
4. Delete build directory and rebuild

### Issue: Build Directory Corruption
**Symptoms:**
- CMake configuration succeeds but build fails
- Strange linker errors
- Missing object files

**Solution:**
```powershell
# Complete clean rebuild
cd source
Remove-Item -Recurse -Force build*
.\build-robust.ps1 -Clean -Static
```

## Advanced: Manual Build Steps

If automated scripts fail, build manually:

```bash
# 1. Create build directory
cd source
mkdir build && cd build

# 2. Configure CMake
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON

# 3. Generate bindings first
cmake --build . --target generate_bindings

# 4. Build everything
cmake --build . --config Release -j$(nproc)

# 5. Test
./cyberbasic.exe ../examples/hello_text.bas
```

## Distribution Build

For distribution (zero dependencies):

```powershell
# Windows
cd source
.\build-complete-static-enhanced.ps1
# Output: build-static-complete/cyberbasic.exe
```

This creates a fully static executable that runs on any Windows 10+ system without installing anything.

## Next Steps

After successful build:

1. **Test Examples:** Run all examples in `examples/` directory
2. **Read Documentation:** See `docs/` for game development guides
3. **Create Games:** Start with `examples/2d_game_template.bas`
4. **Explore 3D:** Try `examples/3d_game_template.bas`

## Support

If build fails:
1. Check prerequisites are installed
2. Try clean build (remove `build*` directories)
3. Check error messages for specific issues
4. Review `COMPILATION_FIXES.md` for known issues
5. Open issue on GitHub with build log

---

**Remember:** The build system is designed to "just work". If it doesn't, the issue is usually:
- Missing prerequisites
- File locks (close IDE)
- Stale build cache (clean build)

Most issues resolve with a clean build!

