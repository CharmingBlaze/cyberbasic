#!/bin/bash

# CyberBasic Universal Static Build Script
# Automatically detects platform and builds accordingly

set -e  # Exit on error

# Detect platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="Linux"
    BUILD_SCRIPT="./build-static-linux.sh"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macOS"
    BUILD_SCRIPT="./build-static-macos.sh"
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    PLATFORM="Windows"
    BUILD_SCRIPT="./build-complete-static-enhanced.ps1"
else
    echo "Unknown platform: $OSTYPE"
    echo "Supported platforms: Linux, macOS, Windows"
    exit 1
fi

echo "Detected platform: $PLATFORM"
echo "Using build script: $BUILD_SCRIPT"
echo

# Make script executable (Linux/macOS only)
if [[ "$PLATFORM" != "Windows" ]]; then
    chmod +x "$BUILD_SCRIPT"
fi

# Run platform-specific build script
if [[ "$PLATFORM" == "Windows" ]]; then
    powershell.exe -ExecutionPolicy Bypass -File "$BUILD_SCRIPT"
else
    "$BUILD_SCRIPT"
fi


