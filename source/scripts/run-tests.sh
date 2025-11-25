#!/bin/bash

# Test runner script for BASIC + Raylib
# Usage: ./scripts/run-tests.sh

echo "Running BASIC + Raylib tests..."
echo ""

# Check if cyberbasic executable exists
if [ -f "./build/cyberbasic" ]; then
    EXECUTABLE="./build/cyberbasic"
elif [ -f "./build/cyberbasic.exe" ]; then
    EXECUTABLE="./build/cyberbasic.exe"
else
    echo "Error: cyberbasic executable not found. Please build the project first:"
    echo "  ./scripts/build-unix.sh    # For Unix/Linux/macOS"
    echo "  ./scripts/build-mingw.sh   # For Windows MinGW"
    echo "  ./build.bat                # For Windows (batch)"
    exit 1
fi

echo "Using executable: $EXECUTABLE"
echo ""

# Test counter
PASSED=0
FAILED=0

# Function to run a test
run_test() {
    local test_file="$1"
    local test_name="$2"
    
    echo "Running $test_name..."
    echo "  File: $test_file"
    
    if $EXECUTABLE "$test_file" > /dev/null 2>&1; then
        echo "  ✓ PASSED"
        ((PASSED++))
    else
        echo "  ✗ FAILED"
        ((FAILED++))
    fi
    echo ""
}

# Run all tests
run_test "tests/smoke_lexer.bas" "Lexer Smoke Test"
run_test "tests/smoke_parser.bas" "Parser Smoke Test"
run_test "tests/smoke_interpreter.bas" "Interpreter Smoke Test"

# Summary
echo "Test Results:"
echo "  Passed: $PASSED"
echo "  Failed: $FAILED"
echo "  Total:  $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "All tests passed! 🎉"
    exit 0
else
    echo "Some tests failed. Please check the output above."
    exit 1
fi
