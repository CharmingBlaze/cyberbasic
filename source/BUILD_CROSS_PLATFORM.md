# CyberBasic Cross-Platform Build Guide

CyberBasic supports building on **Windows**, **Linux**, and **macOS** with full static linking for minimal dependencies.

## Quick Start

### Universal Build (Recommended)
```bash
# Automatically detects your platform and builds accordingly
./build-static-universal.sh
```

### Platform-Specific Builds

#### Windows
```powershell
# PowerShell (Recommended)
.\build-complete-static-enhanced.ps1

# Or batch file
.\build.bat
```

#### Linux
```bash
# Static build with minimal dependencies
./build-static-linux.sh

# Or standard build
./build.sh
```

#### macOS
```bash
# Static build with minimal dependencies  
./build-static-macos.sh

# Or standard build
./build.sh
```

## Prerequisites

### All Platforms
- **CMake** 3.10 or higher
- **Python** 3.6 or higher (for binding generation)
- **Git** (for raylib submodule)

### Windows
- **MinGW-w64** or **MSYS2** with GCC
- **Make** (usually included with MinGW/MSYS2)

### Linux
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake python3 git

# Fedora/RHEL
sudo dnf install gcc-c++ cmake python3 git make

# Arch Linux
sudo pacman -S base-devel cmake python git
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake python git
```

## Build Options

### Static Linking (Recommended)
Static builds create self-contained executables with minimal external dependencies:

- **Windows**: Only depends on system DLLs (KERNEL32, USER32, etc.)
- **Linux**: Only depends on glibc and basic system libraries
- **macOS**: Only depends on system frameworks

### Standard Build
Standard builds may require additional runtime libraries but are faster to compile.

## Build Outputs

All builds create:
- **Executable**: `bin/cyberbasic` (or `cyberbasic.exe` on Windows)
- **Build artifacts**: `build-static-{platform}/` directory

## Testing Your Build

### Run without arguments (Welcome Screen)
```bash
cd bin
./cyberbasic
```

### Run with a BASIC file
```bash
cd bin
./cyberbasic ../examples/hello_text.bas
```

### Verify dependencies (Linux)
```bash
ldd bin/cyberbasic
```

### Verify dependencies (macOS)
```bash
otool -L bin/cyberbasic
```

### Verify dependencies (Windows)
```powershell
objdump -p bin/cyberbasic.exe | Select-String "DLL Name"
```

## Troubleshooting

### Common Issues

#### "Python not found"
- **Windows**: Install Python from python.org or use MSYS2: `pacman -S python`
- **Linux**: `sudo apt install python3` or equivalent for your distro
- **macOS**: `brew install python`

#### "CMake not found"
- **Windows**: Install from cmake.org or use MSYS2: `pacman -S cmake`
- **Linux**: `sudo apt install cmake` or equivalent
- **macOS**: `brew install cmake`

#### "Make not found" (Windows)
- Install MSYS2 or MinGW-w64 which includes make
- Or use Visual Studio with CMake support

#### Build fails with linking errors
- Try the standard build script first: `./build.sh`
- Check that all dependencies are installed
- On Linux, you may need additional development packages

### Platform-Specific Notes

#### Windows
- Use MSYS2 terminal for best compatibility
- PowerShell scripts require execution policy: `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`
- MinGW-w64 is recommended over older MinGW

#### Linux
- Some distributions may require additional development packages
- Static linking may not work on all distributions (use standard build as fallback)

#### macOS
- Xcode Command Line Tools are required
- On Apple Silicon Macs, builds will be native ARM64
- Static linking is more limited on macOS due to system restrictions

## Advanced Configuration

### Custom CMake Options
```bash
mkdir build-custom
cd build-custom
cmake .. -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON -DCUSTOM_OPTION=ON
make -j$(nproc)
```

### Available CMake Options
- `BASIC_STATIC_LINK`: Enable static linking (ON/OFF)
- `CMAKE_BUILD_TYPE`: Release, Debug, RelWithDebInfo, MinSizeRel
- `BUILD_SHARED_LIBS`: Build shared libraries (ON/OFF)

## Contributing

When adding platform-specific code:
1. Use `#ifdef` guards for platform-specific includes
2. Test on all three platforms if possible
3. Update this guide if new dependencies are added
4. Consider using CMake's platform detection features

## Support

For build issues:
1. Check this guide first
2. Verify all prerequisites are installed
3. Try the standard build before static build
4. Report issues with full error output and platform details


