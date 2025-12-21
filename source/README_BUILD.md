# CyberBasic - Build & Integration README

**Complete integration guide for building CyberBasic with all features working.**

## Quick Start

**Just want to build?** Run:
```powershell
cd source
.\build-robust.ps1 -Clean -Static
```

**Want to understand everything?** Read on!

## What's Integrated

CyberBasic integrates multiple systems that work together:

1. **YAML Specifications** (65+ files) → Define Raylib functions
2. **Code Generator** (Python script) → Generates C++ bindings
3. **Generated Bindings** (C++ file) → Bridges BASIC ↔ Raylib
4. **Runtime System** → Registers functions, handles types
5. **YAML Module Loader** → Runtime module loading
6. **Type Conversions** → Value ↔ Raylib types

## Build Process

### Step 1: Prerequisites

- **Python 3.6+** - For code generation
- **CMake 3.10+** - Build system
- **C++ Compiler** - MinGW-w64 (Windows), GCC/Clang (Linux/macOS)

### Step 2: Build

**Windows:**
```powershell
cd source
.\build-robust.ps1 -Clean -Static
```

**Linux:**
```bash
cd source
./build-static-linux.sh
```

**macOS:**
```bash
cd source
./build-static-macos.sh
```

### Step 3: Verify

```bash
# Test executable
.\build-robust-Release\cyberbasic.exe --help

# Run example
.\build-robust-Release\cyberbasic.exe ..\examples\hello_text.bas
```

## 📁 File Structure

```
source/
├── specs/                    # 65+ YAML specification files
│   ├── raylib_api.yaml      # Main API file
│   ├── 01_core_window.yaml  # Window management
│   ├── 30_models_3d_functions.yaml  # 3D models
│   └── ...                  # More YAML files
│
├── tools/
│   └── gen_raylib_bindings.py  # Code generator
│
├── src/
│   ├── bindings/
│   │   └── rt_raylib.gen.cpp  # Generated C++ bindings
│   │
│   ├── core/
│   │   ├── yaml_module_loader.cpp  # YAML module loader
│   │   ├── parser.cpp              # BASIC parser
│   │   └── runtime.cpp              # Runtime system
│   │
│   └── main.cpp  # Entry point, registers everything
│
├── include/bas/
│   ├── raymath_helpers.hpp  # Vector2, Vector3, Camera conversions
│   ├── raygui_helpers.hpp   # Rectangle, Color conversions
│   └── yaml_module_loader.hpp  # YAML loader interface
│
└── CMakeLists.txt  # Build configuration
```

## Integration Flow

```
┌─────────────────┐
│  YAML Specs     │  (65+ files in specs/)
│  Define API     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Code Generator  │  (gen_raylib_bindings.py)
│ Processes YAML  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Generated C++   │  (rt_raylib.gen.cpp)
│ • register_raylib_bindings()
│ • populate_raylib_natives()
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Runtime System  │  (main.cpp)
│ • Registers functions
│ • Populates native registry
│ • Loads YAML modules
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ BASIC Code      │  (Your game!)
│ Uses functions  │
└─────────────────┘
```

## Verification Checklist

After building, verify:

- [ ] `cyberbasic.exe` exists
- [ ] Can run `--help` command
- [ ] Can run example programs
- [ ] All Raylib functions accessible
- [ ] Type conversions work (Vector2, Color, etc.)
- [ ] YAML modules can be loaded (if using)

## Troubleshooting

### File Lock Errors

**Problem:** `fatal error LNK1104: cannot open file`

**Solution:**
1. Close IDE/editor (VS Code, Visual Studio, etc.)
2. Kill build processes: `Get-Process | Where-Object {$_.Path -like "*mingw*"} | Stop-Process`
3. Clean build: `.\build-robust.ps1 -Clean`

### Binding Generation Fails

**Problem:** `Error generating bindings`

**Solution:**
1. Check Python: `python --version` (need 3.6+)
2. Check YAML files exist: `ls source/specs/*.yaml`
3. Run manually: `cd source/build && python ../tools/gen_raylib_bindings.py`

### Missing Functions

**Problem:** Function not found at runtime

**Solution:**
1. Check function is in YAML spec
2. Regenerate bindings: `cmake --build . --target generate_bindings`
3. Rebuild: `cmake --build .`

## Documentation

- **BUILD_GUIDE.md** - Complete build instructions
- **EASY_START_GUIDE.md** - Quick start for game developers
- **INTEGRATION_COMPLETE.md** - Integration status
- **COMPILATION_FIXES.md** - Known issues and fixes

## Next Steps

1. **Build:** Use `build-robust.ps1` or platform script
2. **Test:** Run examples in `examples/` directory
3. **Create:** Start with `examples/2d_game_template.bas`
4. **Learn:** Read `docs/GAME_DEVELOPMENT_GUIDE.md`

## Key Points

- **Everything is integrated** - YAML → Generator → Bindings → Runtime
- **Type conversions work** - Vector2, Vector3, Color, Camera2D, Camera3D
- **Build system is robust** - Handles file locks, clean builds
- **Easy to use** - Simple build scripts, clear documentation

**Ready to build?** Run `.\build-robust.ps1 -Clean -Static` and start creating games!

