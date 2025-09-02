#!/bin/bash

echo "Building CyberBasic Modular System..."
echo

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

echo
echo "Build completed!"
echo "Run: ./cyberbasic ../examples/hello_world.bas"
