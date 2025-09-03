#!/usr/bin/env bash
set -euo pipefail

if ! command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1 && ! command -v g++ >/dev/null 2>&1; then
  echo "MinGW-w64 g++ not found. Install MinGW-w64 GCC 13+." >&2
  exit 1
fi

if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: This script must be run from the project root." >&2
    exit 1
fi

mkdir -p build

cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi
