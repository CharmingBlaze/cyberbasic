# Project Organization

This document describes the organization and structure of the CyberBasic project.

## Root Directory

### Configuration Files
- `.clang-format` - Code formatting configuration
- `.clang-tidy` - Static analysis configuration
- `.gitignore` - Git ignore rules
- `CMakeLists.txt` - Main CMake build configuration

### Build Scripts (Entry Points)
- `build.bat` - Windows batch build script
- `build.ps1` - PowerShell build script
- `build.sh` - Unix/Linux build script

### Utility Scripts
- `install_deps.py` - Dependency installation script
- `run_test.py` - Test runner script

## Directory Structure

### Core Source Code
- **`src/`** - Main source code
  - `core/` - Core interpreter (lexer, parser, interpreter, runtime)
  - `modules/` - Feature modules (3d, ai, audio, game, graphics, gui, input, networking, physics, raylib)
  - `builtins_*.cpp` - Built-in function implementations
  - `main.cpp` - Entry point
  - `custom_cyberbasic.cpp` - Custom CyberBasic extensions

- **`include/bas/`** - Header files
  - All public headers organized by feature/module

### Documentation
- **`docs/`** - All documentation
  - User guides (GETTING_STARTED.md, QUICK_REFERENCE.md, etc.)
  - Feature guides (2D_GRAPHICS_GUIDE.md, 3D_GRAPHICS_GUIDE.md, etc.)
  - `implementation/` - Internal implementation documentation

### Examples
- **`examples/`** - All `.bas` example files
  - Game examples (space_invaders.bas, simple_pong.bas, ai_pong.bas)
  - Feature demos (hello_graphics.bas, collision_demo.bas, etc.)
  - Test examples (minimal_test.bas, raymath_test.bas, etc.)

### Specifications
- **`specs/`** - API specification files (YAML)
  - Organized by feature/category
  - Used for code generation and documentation

### Tests
- **`tests/`** - Test files
  - `.bas` test files
  - CMake test configuration

### Build System
- **`cmake/`** - CMake modules and utilities
- **`build/`**, **`build-msys/`**, **`build-msys2/`** - Build output directories (gitignored)

### Scripts and Tools
- **`scripts/`** - Build and utility scripts
  - `build.py` - Python build script
  - `clean.py` - Clean build artifacts
  - `build-*.sh` - Platform-specific build scripts
  - `run-*.sh`, `run-*.ps1` - Test and example runners

- **`tools/`** - Development tools
  - `gen_raylib_bindings.py` - Code generator for Raylib bindings
  - `clean_generator.py` - Cleanup utility

### Resources
- **`images/`** - Project images and assets
  - `logo.png` - Project logo
  - `pong.png` - Pong game screenshot
  - `SpaceInvaders.png` - Space Invaders game screenshot

- **`3Dmodels/`** - 3D model files for examples
  - `object.obj` - Example 3D model

### External Dependencies
- **`raygui/`** - Raygui library (submodule/clone)
- **`other/`** - Miscellaneous files
  - `raylibAPI.docx` - Raylib API reference document

## File Naming Conventions

### Source Files
- C++ source: `*.cpp`
- C++ headers: `*.hpp`
- BASIC examples: `*.bas`

### Documentation
- Markdown: `*.md`
- Organized in `docs/` with subdirectories for implementation notes

### Build Artifacts
- All build outputs go to `build/` or `build-*/` directories
- These are gitignored and can be safely deleted

## Organization Principles

1. **Separation of Concerns**: Source code, documentation, examples, and tests are clearly separated
2. **Modular Structure**: Code is organized into logical modules
3. **Clear Entry Points**: Build scripts in root for easy access
4. **Comprehensive Documentation**: All docs in `docs/` with clear organization
5. **Example-Driven**: All examples in `examples/` folder
6. **Clean Root**: Root directory contains only essential files

## Maintenance

- Keep root directory clean - only essential entry points
- Move utility scripts to `scripts/` or `tools/`
- All `.bas` files should be in `examples/` or `tests/`
- Build logs and temporary files should be gitignored
- Delete duplicate or unused files regularly

