#!/bin/bash
# CyberBasic Universal Build Script for Linux/macOS
# This script builds CyberBasic with the best configuration for your system

set -e  # Exit on any error

echo "CyberBasic Universal Build Script"
echo "================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if Python is available for the universal script
if command -v python3 >/dev/null 2>&1; then
    echo "Python found. Using universal build script..."
    python3 build.py "$@"
    exit $?
elif command -v python >/dev/null 2>&1; then
    echo "Python found. Using universal build script..."
    python build.py "$@"
    exit $?
fi

echo "Python not found. Using fallback shell build..."
echo

# Detect platform
PLATFORM=$(uname -s)
case $PLATFORM in
    Linux*)
        PLATFORM_NAME="Linux"
        CORES=$(nproc)
        ;;
    Darwin*)
        PLATFORM_NAME="macOS"
        CORES=$(sysctl -n hw.ncpu)
        ;;
    *)
        echo -e "${RED}ERROR: Unsupported platform: $PLATFORM${NC}"
        exit 1
        ;;
esac

echo "Building on $PLATFORM_NAME..."

# Check dependencies
echo "Checking dependencies..."

if ! command -v cmake >/dev/null 2>&1; then
    echo -e "${RED}ERROR: CMake not found.${NC}"
    if [[ "$PLATFORM_NAME" == "Linux" ]]; then
        echo "Install with: sudo apt install cmake"
    elif [[ "$PLATFORM_NAME" == "macOS" ]]; then
        echo "Install with: brew install cmake"
    fi
    exit 1
fi

# Check for compiler
COMPILER=""
if command -v gcc >/dev/null 2>&1; then
    COMPILER="GCC"
elif command -v clang >/dev/null 2>&1; then
    COMPILER="Clang"
else
    echo -e "${RED}ERROR: No suitable compiler found.${NC}"
    if [[ "$PLATFORM_NAME" == "Linux" ]]; then
        echo "Install with: sudo apt install build-essential"
    elif [[ "$PLATFORM_NAME" == "macOS" ]]; then
        echo "Install Xcode Command Line Tools: xcode-select --install"
    fi
    exit 1
fi

echo "Using $COMPILER compiler"

# Parse arguments
BUILD_TYPE="Release"
CLEAN=false
STATIC=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --static)
            STATIC=true
            shift
            ;;
        --help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --debug    Build in debug mode"
            echo "  --clean    Clean before building"
            echo "  --static   Enable static linking"
            echo "  --help     Show this help"
            exit 0
            ;;
        *)
            echo -e "${YELLOW}WARNING: Unknown option $1${NC}"
            shift
            ;;
    esac
done

# Create build directory
BUILD_DIR="build-$(echo $PLATFORM_NAME | tr '[:upper:]' '[:lower:]')-$(echo $BUILD_TYPE | tr '[:upper:]' '[:lower:]')"

if [[ "$CLEAN" == true ]] && [[ -d "$BUILD_DIR" ]]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo "Configuring..."
CMAKE_ARGS=(
    -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
    ../source
)

if [[ "$STATIC" == true ]]; then
    CMAKE_ARGS+=(-DBASIC_STATIC_LINK=ON)
fi

cmake "${CMAKE_ARGS[@]}"

# Build
echo "Building with $CORES cores..."
make -j"$CORES"

# Find executable
EXE_NAME="cyberbasic"
if [[ -f "$EXE_NAME" ]]; then
    EXE_PATH="$PWD/$EXE_NAME"
else
    echo -e "${RED}ERROR: Executable not found after build.${NC}"
    exit 1
fi

echo
echo -e "${GREEN}Build successful!${NC}"
echo "Executable: $EXE_PATH"

# Copy to bin directory
cd ..
mkdir -p bin
cp "$BUILD_DIR/$EXE_NAME" "bin/$EXE_NAME"
echo "Copied to: $PWD/bin/$EXE_NAME"

echo
echo "To test your build:"
echo "  ./bin/cyberbasic examples/hello_text.bas"
