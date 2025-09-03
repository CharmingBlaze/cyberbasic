@echo off
echo Building BASIC + Raylib for Windows...

REM Check if we're in the right directory
if not exist "CMakeLists.txt" (
    echo Error: CMakeLists.txt not found. Please run this from the project root.
    pause
    exit /b 1
)

REM Create build directory
if not exist "build" mkdir build
cd build

REM Configure with CMake (reuse existing generator if cache present)
echo Configuring with CMake...
if exist CMakeCache.txt (
    rem Reconfigure in-place using the existing generator
    cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release
    if %ERRORLEVEL% neq 0 (
        echo CMake reconfiguration failed
        pause
        exit /b 1
    )
) else (
    rem Fresh configure with Ninja (matches prior builds and is fast)
    cmake -G "Ninja" -S .. -B . -DCMAKE_BUILD_TYPE=Release
    if %ERRORLEVEL% neq 0 (
        echo CMake configuration failed
        pause
        exit /b 1
    )
)

REM Build
echo Building...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo Build failed
    pause
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
pause
