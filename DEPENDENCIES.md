# CyberBasic Dependencies

## Required Dependencies

### Build System
- **CMake**: 3.25 or higher
- **C++ Compiler**: GCC 13+ or Clang 15+ with C++20 support
- **Python**: 3.10+ with PyYAML

### Runtime Dependencies
- **Raylib**: 5.0 (automatically downloaded via FetchContent)

## Optional Dependencies

### Development Tools
- **Ninja**: For faster builds (optional, CMake will use Make if not available)
- **clang-format**: For code formatting (optional)
- **clang-tidy**: For static analysis (optional)

## Dependency Management

### Raylib
Raylib is automatically downloaded and built during CMake configuration using FetchContent. No manual installation required.

**Version**: 5.0 (latest stable)
**Source**: https://github.com/raysan5/raylib.git

### Python Packages
Required Python packages:
- `PyYAML` - For parsing YAML specification files

Install with:
```bash
pip install PyYAML
```

## Platform-Specific

### Windows
- **MSYS2** with MinGW-w64 toolchain (recommended)
- Or Visual Studio 2022+ with C++20 support

### Linux
- Standard build tools (gcc, make, cmake)
- Development headers for OpenGL, ALSA, etc.

### macOS
- Xcode Command Line Tools
- Homebrew for CMake (if not using Xcode)

## Version Information

| Dependency | Minimum Version | Recommended | Notes |
|------------|----------------|-------------|-------|
| CMake | 3.25 | Latest | Required for C++20 support |
| GCC | 13.0 | Latest | C++20 support required |
| Clang | 15.0 | Latest | C++20 support required |
| Python | 3.10 | 3.11+ | For code generation |
| Raylib | 5.0 | 5.0 | Auto-downloaded |

## Reproducible Builds

All dependencies are either:
1. System-level (CMake, compiler)
2. Auto-downloaded (Raylib via FetchContent)
3. Standard library (Python stdlib + PyYAML)

This ensures reproducible builds across different environments.

## Updating Dependencies

### Raylib
Update the `GIT_TAG` in `CMakeLists.txt`:
```cmake
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0  # Update this to newer version
)
```

### CMake
Update system CMake installation:
- Windows: Download from cmake.org
- Linux: `sudo apt update && sudo apt upgrade cmake`
- macOS: `brew upgrade cmake`

---

**Last Updated**: 2025-01-XX

