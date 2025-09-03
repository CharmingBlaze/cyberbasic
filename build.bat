@echo off
setlocal EnableDelayedExpansion
REM Set NO_PAUSE=1 to skip pauses for non-interactive CI/runs
if "%NO_PAUSE%"=="1" set SKIP_PAUSE=1
echo Building BASIC + Raylib for Windows...

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this from the project root.
    if not defined SKIP_PAUSE pause
    exit /b 1
)

set BUILD_COMMAND=

if exist "C:\msys64\mingw64\bin" (
    @echo off
    setlocal

    REM Setup MinGW environment
    set "PATH=C:\msys64\mingw64\bin;C:\msys64\usr\bin;%PATH%"

    REM Clean previous build directory

    REM Configure
    echo --- Configuring CMake ---
    cmake -S . -B build_cascade -G "MinGW Makefiles" -DBASIC_STATIC_LINK=ON
    if %errorlevel% neq 0 (
        echo CMake configuration failed.
        exit /b %errorlevel%
    )

    REM Build
    echo.
    echo --- Building Project ---
    cmake --build build_cascade --verbose
    if %errorlevel% neq 0 (
        echo Build failed.
        exit /b %errorlevel%
    )

    echo.
    echo --- Build Successful ---
    endlocal
) else (
    echo Error: MinGW-w64 not found at C:\msys64\mingw64. Please install MSYS2 and the MinGW-w64 toolchain.
    if not defined SKIP_PAUSE pause
    exit /b 1
)

set BUILD_RET=%ERRORLEVEL%

if %BUILD_RET% neq 0 (
    echo Build failed.
    if not defined SKIP_PAUSE pause
    exit /b %BUILD_RET%
)

if %ERRORLEVEL% neq 0 (
    echo Build failed
    if not defined SKIP_PAUSE pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable: build\cyberbasic.exe
echo.
echo To run examples:
echo   cyberbasic.exe ..\examples\hello_text.bas
echo   cyberbasic.exe ..\examples\hello_graphics.bas
echo.
if not defined SKIP_PAUSE pause
