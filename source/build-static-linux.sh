#!/bin/bash

# CyberBasic Static Build Script for Linux
# Creates a fully static executable with minimal dependencies

set -e  # Exit on error

echo -e "\e[34m[BUILD]\e[0m Starting Complete Static Build Process for Linux"
echo -e "\e[34m[BUILD]\e[0m Target: Minimal dependencies, optimized executable"

SOURCE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SOURCE_DIR/build-static-linux"
BIN_DIR="$SOURCE_DIR/../bin"
PYTHON_SCRIPT="$SOURCE_DIR/tools/gen_raylib_bindings.py"
OUTPUT_BINDING_FILE="$BUILD_DIR/src/bindings/rt_raylib.gen.cpp"
SOURCE_BINDING_FILE="$SOURCE_DIR/src/bindings/rt_raylib.gen.cpp"

# Phase 1: Clean slate approach
echo -e "\e[34m[BUILD]\e[0m Phase 1: Cleaning all build directories and cached files"
rm -rf "$SOURCE_DIR/build-dist" 2>/dev/null || true
rm -rf "$SOURCE_DIR/build-static-final" 2>/dev/null || true
rm -rf "$BUILD_DIR" 2>/dev/null || true
rm -f "$SOURCE_BINDING_FILE" 2>/dev/null || true
echo -e "\e[32m[SUCCESS]\e[0m Cache cleanup completed"

# Phase 2: Verify Python availability
echo -e "\e[34m[BUILD]\e[0m Phase 2: Verifying Python availability"
if command -v python3 &> /dev/null; then
    PYTHON_VERSION=$(python3 -c "import sys; print(f'Python {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}')")
    echo -e "\e[34m[BUILD]\e[0m Python found: $PYTHON_VERSION"
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_VERSION=$(python -c "import sys; print(f'Python {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}')")
    echo -e "\e[34m[BUILD]\e[0m Python found: $PYTHON_VERSION"
    PYTHON_CMD="python"
else
    echo -e "\e[31m[ERROR]\e[0m Python not found. Please install Python 3."
    exit 1
fi

# Phase 3: Create build directory
echo -e "\e[34m[BUILD]\e[0m Phase 3: Creating build directory: $BUILD_DIR"
mkdir -p "$BUILD_DIR"

# Phase 4: Configure CMake
echo -e "\e[34m[BUILD]\e[0m Phase 4: Configuring CMake with static linking options"
cd "$BUILD_DIR"
CMAKE_COMMAND="cmake .. -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON -DBUILD_SHARED_LIBS=OFF"
echo -e "\e[34m[BUILD]\e[0m CMake command: $CMAKE_COMMAND"
if ! $CMAKE_COMMAND; then
    echo -e "\e[31m[ERROR]\e[0m CMake configuration failed"
    exit 1
fi
echo -e "\e[32m[SUCCESS]\e[0m CMake configuration completed"

# Phase 5: Generate raylib bindings
echo -e "\e[34m[BUILD]\e[0m Phase 5: Generating raylib bindings"
if ! make generate_bindings; then
    echo -e "\e[33m[WARNING]\e[0m Binding generation via make failed, trying manual generation"
    mkdir -p "$(dirname "$OUTPUT_BINDING_FILE")"
    if ! $PYTHON_CMD "$PYTHON_SCRIPT"; then
        echo -e "\e[31m[ERROR]\e[0m Manual binding generation failed"
        exit 1
    fi
fi
echo -e "\e[32m[SUCCESS]\e[0m Binding generation completed"

# Phase 6: Build executable
echo -e "\e[34m[BUILD]\e[0m Phase 6: Building cyberbasic executable"
CPU_CORES=$(nproc)
if ! make cyberbasic -j$CPU_CORES; then
    echo -e "\e[31m[ERROR]\e[0m Build failed"
    exit 1
fi
echo -e "\e[32m[SUCCESS]\e[0m Build completed"

# Phase 7: Verify dependencies
echo -e "\e[34m[BUILD]\e[0m Phase 7: Verifying library dependencies"
EXECUTABLE_PATH="$BUILD_DIR/cyberbasic"
if [[ -f "$EXECUTABLE_PATH" ]]; then
    if command -v ldd &> /dev/null; then
        echo -e "\e[34m[BUILD]\e[0m Library dependencies:"
        ldd "$EXECUTABLE_PATH" | grep -v "linux-vdso\|ld-linux" || echo -e "\e[32m[SUCCESS]\e[0m Minimal system dependencies only"
    else
        echo -e "\e[33m[WARNING]\e[0m ldd not found, skipping dependency verification"
    fi
else
    echo -e "\e[31m[ERROR]\e[0m Executable not found at $EXECUTABLE_PATH"
    exit 1
fi

# Phase 8: Copy to bin folder
echo -e "\e[34m[BUILD]\e[0m Phase 8: Copying executable to bin folder: $BIN_DIR"
mkdir -p "$BIN_DIR"
cp "$EXECUTABLE_PATH" "$BIN_DIR/cyberbasic"
echo -e "\e[32m[SUCCESS]\e[0m Executable copied to $BIN_DIR"

cd "$SOURCE_DIR"
echo -e "\e[32m[SUCCESS]\e[0m Complete Static Build Process Finished"
echo -e "\e[34m[INFO]\e[0m To test: cd ../bin && ./cyberbasic"
exit 0


