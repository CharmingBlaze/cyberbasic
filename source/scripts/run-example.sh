#!/usr/bin/env bash
set -euo pipefail
: "${1:=examples/hello_graphics.bas}"
./build/cyberbasic "$1"
