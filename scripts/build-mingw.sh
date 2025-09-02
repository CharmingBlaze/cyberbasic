#!/usr/bin/env bash
set -euo pipefail
if ! command -v x86_64-w64-mingw32-g++ >/dev/null 2>&1 && ! command -v g++ >/dev/null 2>&1; then
  echo "MinGW-w64 g++ not found. Install MinGW-w64 GCC 13+." >&2
  exit 1
fi
rm -rf build && mkdir -p build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release -j
