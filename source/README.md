# CyberBasic Source & Build Guide

This `source/` directory contains everything required to compile the CyberBasic interpreter from scratch:

- `CMakeLists.txt` – main build definition  
- `cmake/` – helper modules  
- `src/`, `include/`, `tests/`, `specs/` – interpreter source, headers, tests, and API specs  
- `raylib/`, `raygui/` – vendored dependencies (raylib is tracked as a submodule)  
- `scripts/`, `tools/`, `build-*.bat|.ps1|.sh` – helper utilities for local builds

If you only want to *use* CyberBasic, grab the prebuilt Windows executable at `bin/cyberbasic.exe` from the repository root. Everything in this folder is optional and can be deleted without affecting the ready-to-run interpreter.

---

## Prerequisites

- CMake 3.25+
- A C++20 compiler
  - Windows: MinGW-w64 GCC 13+ (via MSYS2 UCRT64 recommended)
  - Linux/macOS: GCC or Clang with C++20 support
- Python 3.10+ with `pyyaml` (needed for the Raylib binding generator)
- Git with submodule support (raylib lives inside this folder)

Clone with submodules so raylib is available:

```bash
git clone --recursive https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic/source
# or, if you already cloned without submodules
git submodule update --init --recursive
```

---

## Standard Builds

All commands below assume your working directory is `cyberbasic/source`.

### Windows (MinGW / MSYS2)

```bash
cmake -S . -B build-mingw -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-mingw
```

### Linux

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(nproc)
```

### macOS

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -- -j$(sysctl -n hw.ncpu)
```

macOS uses the same `CMakeLists.txt`; CMake automatically links Cocoa, IOKit, and OpenGL. I don’t have a Mac, so community fixes are appreciated if something breaks.

The resulting binaries live under the build directory you chose (`build-mingw/cyberbasic.exe`, `build/cyberbasic`, etc.).

---

## Distribution Build (Static Windows EXE)

To reproduce the statically linked executable that ships at `bin/cyberbasic.exe`, run:

```powershell
cd cyberbasic\source
.\build-dist.bat
```

The script wipes `build-dist/`, configures CMake with `BASIC_STATIC_LINK=ON`, and produces `build-dist\cyberbasic.exe` with no DLL dependencies.

---

## Troubleshooting

1. **Missing raylib files** – ensure you ran `git submodule update --init --recursive`.
2. **Old build cache** – delete your `build*/` directory and re-run CMake.
3. **Compiler errors about C++ features** – confirm your compiler advertises full C++20 support.
4. **Python errors** – install PyYAML: `pip install pyyaml`.

If you need to regenerate Raylib bindings, run `python tools/gen_raylib_bindings.py` from this directory before rebuilding.

---

## Folder Layout

```
source/
├── CMakeLists.txt          # Build definition
├── build-*.bat/.ps1/.sh    # Convenience scripts
├── cmake/                  # Custom CMake helpers
├── include/                # Public headers
├── raylib/, raygui/        # Dependencies
├── scripts/, tools/        # Build + tooling helpers
├── specs/                  # Raylib binding specifications
├── src/                    # Interpreter implementation
├── tests/                  # BASIC smoke tests
└── docs/implementation/    # Developer notes & history
```

Everything outside `source/` is safe for end users; everything inside `source/` is for developers who want to hack on CyberBasic itself.

