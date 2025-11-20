#!/bin/bash

set -e  # Exit on error

echo "Building CyberBasic..."
echo

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
echo "Building..."
make -j$(nproc)

echo
echo "Build completed successfully!"
echo "Executable: build/cyberbasic"
echo
echo "To run examples:"
echo "  ./cyberbasic ../examples/hello_text.bas"
echo "  ./cyberbasic ../examples/hello_graphics.bas"
