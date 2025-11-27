@echo off
REM CyberBasic Windows Build Script
REM This script builds CyberBasic on Windows with MinGW or MSVC

setlocal enabledelayedexpansion

echo CyberBasic Universal Build Script for Windows
echo ============================================

REM Check if Python is available for the universal script
python --version >nul 2>&1
if %errorlevel% == 0 (
    echo Python found. Using universal build script...
    python build.py %*
    goto :end
)

echo Python not found. Using fallback batch build...
echo.

REM Check for CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: CMake not found. Please install CMake from https://cmake.org/
    echo Or use the Python build script: python build.py
    pause
    exit /b 1
)

REM Check for MinGW GCC
gcc --version >nul 2>&1
if %errorlevel% == 0 (
    set COMPILER=MinGW
    set GENERATOR=MinGW Makefiles
    goto :build
)

REM Check for MSVC
cl >nul 2>&1
if %errorlevel% == 0 (
    set COMPILER=MSVC
    set GENERATOR=Visual Studio 16 2019
    set ALLOW_MSVC=-DALLOW_MSVC=ON
    echo WARNING: Using MSVC (experimental). MinGW-w64 is recommended.
    goto :build
)

echo ERROR: No suitable compiler found.
echo Please install one of the following:
echo   - MinGW-w64 GCC (recommended): https://www.mingw-w64.org/
echo   - MSYS2 with GCC: https://www.msys2.org/
echo   - Visual Studio Build Tools (experimental)
pause
exit /b 1

:build
echo Building with %COMPILER%...
echo.

REM Create build directory
if not exist "build-windows" mkdir build-windows
cd build-windows

REM Configure
echo Configuring...
cmake -G "%GENERATOR%" -DCMAKE_BUILD_TYPE=Release %ALLOW_MSVC% ../source
if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed.
    pause
    exit /b 1
)

REM Build
echo Building...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERROR: Build failed.
    pause
    exit /b 1
)

REM Find executable
if exist "cyberbasic.exe" (
    set EXE_PATH=cyberbasic.exe
) else if exist "Release\cyberbasic.exe" (
    set EXE_PATH=Release\cyberbasic.exe
) else (
    echo ERROR: Executable not found after build.
    pause
    exit /b 1
)

echo.
echo Build successful!
echo Executable: %CD%\!EXE_PATH!

REM Copy to bin directory
cd ..
if not exist "bin" mkdir bin
copy "build-windows\!EXE_PATH!" "bin\cyberbasic.exe" >nul
echo Copied to: %CD%\bin\cyberbasic.exe

echo.
echo To test your build:
echo   bin\cyberbasic.exe examples\hello_text.bas

:end
pause
