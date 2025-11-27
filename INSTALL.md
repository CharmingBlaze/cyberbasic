# CyberBasic Installation Guide

## Quick Start (5 Minutes)

### Option 1: Download Pre-built Windows Executable (Easiest)

1. **Download**: Get the latest `cyberbasic.exe` from [bin/cyberbasic.exe](https://github.com/CharmingBlaze/cyberbasic/raw/master/bin/cyberbasic.exe)
2. **Save**: Put it in a folder of your choice
3. **Run**: `cyberbasic.exe your_game.bas`

**That's it!** No installation, no dependencies, just download and run.

### Option 2: Build from Source (All Platforms)

```bash
# 1. Clone the repository
git clone https://github.com/CharmingBlaze/cyberbasic.git
cd cyberbasic

# 2. Build (choose one)
python build.py        # Universal (recommended)
build.bat             # Windows batch file  
./build.sh            # Linux/macOS shell script

# 3. Test
./bin/cyberbasic examples/hello_text.bas
```

## Prerequisites for Building

### Windows
- **Python 3.6+** (optional, but recommended)
- **MinGW-w64 GCC** or **MSVC** (Visual Studio)
- **CMake 3.10+**

**Easy Installation:**
1. Install [Python](https://python.org/) (check "Add to PATH")
2. Install [MSYS2](https://www.msys2.org/) for MinGW-w64:
   ```bash
   # In MSYS2 terminal
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake python
   ```
3. Or install [Visual Studio](https://visualstudio.microsoft.com/) with C++ tools

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake python3 git
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install gcc-c++ cmake python3 git make
```

### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew (optional but recommended)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake python git
```

## Build Methods

### Method 1: Universal Python Script (Recommended)

**Advantages:** Automatic platform detection, dependency checking, best configuration

```bash
python build.py                    # Standard build
python build.py --debug           # Debug build
python build.py --clean           # Clean build
python build.py --msvc            # Use MSVC (Windows only)
python build.py --help            # Show all options
```

### Method 2: Platform Scripts

**Windows:**
```batch
build.bat                         # Uses MinGW or MSVC automatically
```

**Linux/macOS:**
```bash
./build.sh                        # Uses GCC or Clang automatically
./build.sh --debug                # Debug build
./build.sh --static               # Static linking
```

### Method 3: Manual CMake

**For advanced users who want full control:**

```bash
mkdir build && cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Release ../source

# Build
cmake --build .                   # All platforms
# OR
make -j$(nproc)                   # Linux/macOS
```

## What You Get

After building:
- **Executable**: `bin/cyberbasic` (or `cyberbasic.exe`)
- **Examples**: 69+ sample programs in `examples/`
- **Documentation**: Complete guides in `docs/`

## Test Your Installation

```bash
# Test with a simple program
./bin/cyberbasic examples/hello_text.bas

# Test with a game
./bin/cyberbasic examples/simple_pong.bas

# Test graphics
./bin/cyberbasic examples/hello_graphics.bas
```

## Troubleshooting

### "Command not found" or "File not found"

**Windows:**
- Make sure you're in the correct directory
- Use `.\bin\cyberbasic.exe` instead of `./bin/cyberbasic`
- Check that the build completed successfully

**Linux/macOS:**
- Make sure you're in the project root directory
- Use `./bin/cyberbasic` (with the `./`)
- Check file permissions: `chmod +x bin/cyberbasic`

### "No suitable compiler found"

**Windows:**
- Install MinGW-w64 via [MSYS2](https://www.msys2.org/) (recommended)
- Or install [Visual Studio](https://visualstudio.microsoft.com/) with C++ tools
- Make sure the compiler is in your PATH

**Linux:**
```bash
# Ubuntu/Debian
sudo apt install build-essential

# Fedora/RHEL  
sudo dnf install gcc-c++ make

# Arch Linux
sudo pacman -S base-devel
```

**macOS:**
```bash
xcode-select --install
```

### "CMake not found"

**Windows:**
- Download from [cmake.org](https://cmake.org/) (check "Add to PATH")
- Or via MSYS2: `pacman -S mingw-w64-x86_64-cmake`

**Linux:**
```bash
# Ubuntu/Debian
sudo apt install cmake

# Fedora/RHEL
sudo dnf install cmake
```

**macOS:**
```bash
brew install cmake
```

### "Python not found"

**Windows:**
- Download from [python.org](https://python.org/) (check "Add to PATH")
- Or via MSYS2: `pacman -S python`

**Linux:**
```bash
sudo apt install python3
```

**macOS:**
```bash
brew install python
# Or use system Python (usually pre-installed)
```

### Build Fails with MSVC

MSVC support is experimental. For best results:
1. Use MinGW-w64 GCC instead: Install [MSYS2](https://www.msys2.org/)
2. Or force MSVC: `python build.py --msvc`
3. Or download the pre-built Windows executable

### "Permission denied" (Linux/macOS)

```bash
chmod +x build.sh
chmod +x bin/cyberbasic
```

## Distribution

To distribute your games:
1. Copy `bin/cyberbasic(.exe)` to your game folder
2. Include your `.bas` files and any assets
3. Create a launcher script or batch file
4. Zip everything together

See [Distribution Guide](docs/DISTRIBUTION_GUIDE.md) for details.

## Getting Help

If you're still having issues:

1. **Check the logs**: Build scripts show detailed error messages
2. **Try debug build**: `python build.py --debug` for more information
3. **Clean build**: `python build.py --clean` to start fresh
4. **Check issues**: [GitHub Issues](https://github.com/CharmingBlaze/cyberbasic/issues)
5. **Ask for help**: Open a new issue with your error message and platform details

## Next Steps

Once installed:
1. **Learn the basics**: Read [Getting Started Guide](docs/GETTING_STARTED.md)
2. **Try examples**: Run programs from the `examples/` directory
3. **Make your first game**: Follow [Game Development Guide](docs/GAME_DEVELOPMENT_GUIDE.md)
4. **Explore features**: Check out [Advanced Features Guide](docs/ADVANCED_FEATURES_GUIDE.md)

Happy coding!
