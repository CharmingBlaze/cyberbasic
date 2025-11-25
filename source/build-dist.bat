@echo off
setlocal EnableDelayedExpansion
REM Build CyberBasic for distribution with static linking
echo Building CyberBasic for Distribution with Static Linking

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this from the project root.
    pause
    exit /b 1
)

if exist "C:\msys64\mingw64\bin" (
    REM Setup MinGW environment
    set "PATH=C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%"

    REM Clean previous build directory
    if exist "build-dist" (
        echo Cleaning previous build-dist directory
        rmdir /s /q build-dist
    )

    REM Configure with static linking
    echo Configuring CMake for Distribution
    cmake -S . -B build-dist -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DBASIC_STATIC_LINK=ON
    if %errorlevel% neq 0 (
        echo CMake configuration failed
        pause
        exit /b %errorlevel%
    )

    REM Build
    echo.
    echo Building Project - this may take a while
    cmake --build build-dist --config Release
    if %errorlevel% neq 0 (
        echo Build failed
        pause
        exit /b %errorlevel%
    )

    echo.
    echo Build Successful
    echo Executable: build-dist\cyberbasic.exe
    echo.
    echo This is a statically linked build ready for distribution
    echo It does not require any DLL files
    echo.
) else (
    echo Error: MinGW-w64 not found at C:\msys64\mingw64
    echo Please install MSYS2 and the MinGW-w64 toolchain
    pause
    exit /b 1
)

pause
