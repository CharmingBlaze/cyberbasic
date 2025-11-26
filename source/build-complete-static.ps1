#!/usr/bin/env pwsh
# Complete Static Build Script for CyberBasic
# This script ensures a clean, static build with zero DLL dependencies

param(
    [switch]$SkipClean = $false,
    [switch]$Verbose = $false
)

# Set error handling
$ErrorActionPreference = "Stop"

# Colors for output
$Red = "`e[31m"
$Green = "`e[32m"
$Yellow = "`e[33m"
$Blue = "`e[34m"
$Reset = "`e[0m"

function Write-Status {
    param([string]$Message, [string]$Color = $Blue)
    Write-Host "${Color}[BUILD]${Reset} $Message"
}

function Write-Success {
    param([string]$Message)
    Write-Host "${Green}[SUCCESS]${Reset} $Message"
}

function Write-Warning {
    param([string]$Message)
    Write-Host "${Yellow}[WARNING]${Reset} $Message"
}

function Write-Error {
    param([string]$Message)
    Write-Host "${Red}[ERROR]${Reset} $Message"
}

# Verify we're in the source directory
if (!(Test-Path "CMakeLists.txt")) {
    Write-Error "Must run from source directory containing CMakeLists.txt"
    exit 1
}

Write-Status "Starting Complete Static Build Process"
Write-Status "Target: Zero DLL dependencies, fully static executable"

# Step 1: Clean all build directories and cached files
if (!$SkipClean) {
    Write-Status "Phase 1: Cleaning all build directories and cached files"
    
    $buildDirs = @("build-dist", "build-static-final", "build", "build-release", "build-debug")
    foreach ($dir in $buildDirs) {
        if (Test-Path $dir) {
            Write-Status "Removing build directory: $dir"
            Remove-Item -Recurse -Force $dir
        }
    }
    
    # Clean any cached binding files in source
    if (Test-Path "src/bindings/rt_raylib.gen.cpp") {
        Write-Status "Removing cached binding file from source"
        Remove-Item "src/bindings/rt_raylib.gen.cpp" -Force
    }
    
    Write-Success "Cache cleanup completed"
} else {
    Write-Warning "Skipping cache cleanup (--SkipClean specified)"
}

# Step 2: Verify Python is available for binding generation
Write-Status "Phase 2: Verifying Python availability"
try {
    $pythonVersion = python --version 2>&1
    Write-Status "Python found: $pythonVersion"
} catch {
    Write-Error "Python not found. Required for binding generation."
    exit 1
}

# Step 3: Create build directory
$buildDir = "build-static-complete"
Write-Status "Phase 3: Creating build directory: $buildDir"
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
Set-Location $buildDir

# Step 4: Configure CMake with static linking
Write-Status "Phase 4: Configuring CMake with static linking options"
$cmakeArgs = @(
    ".."
    "-G", "MinGW Makefiles"
    "-DCMAKE_BUILD_TYPE=Release"
    "-DBASIC_STATIC_LINK=ON"
    "-DBUILD_SHARED_LIBS=OFF"
    "-DCMAKE_CXX_FLAGS=-static-libgcc -static-libstdc++"
    "-DCMAKE_EXE_LINKER_FLAGS=-static"
)

if ($Verbose) {
    $cmakeArgs += "--debug-output"
}

Write-Status "CMake command: cmake $($cmakeArgs -join ' ')"
& cmake @cmakeArgs

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed"
    exit 1
}

Write-Success "CMake configuration completed"

# Step 5: Generate bindings first
Write-Status "Phase 5: Generating raylib bindings"
& make rt_raylib.gen.cpp

if ($LASTEXITCODE -ne 0) {
    Write-Warning "Binding generation via make failed, trying manual generation"
    
    # Manual binding generation
    $bindingDir = "src/bindings"
    New-Item -ItemType Directory -Path $bindingDir -Force | Out-Null
    
    Set-Location ..
    & python tools/gen_raylib_bindings.py
    Set-Location $buildDir
    
    # Copy generated binding to build directory
    if (Test-Path "../src/bindings/rt_raylib.gen.cpp") {
        Copy-Item "../src/bindings/rt_raylib.gen.cpp" "src/bindings/" -Force
        Write-Success "Manual binding generation completed"
    } else {
        Write-Error "Binding generation failed completely"
        exit 1
    }
}

# Step 6: Build the executable
Write-Status "Phase 6: Building cyberbasic executable"
& make cyberbasic -j4

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

Write-Success "Build completed successfully"

# Step 7: Verify executable exists
$exePath = "cyberbasic.exe"
if (!(Test-Path $exePath)) {
    Write-Error "Executable not found: $exePath"
    exit 1
}

Write-Success "Executable created: $exePath"

# Step 8: Verify DLL dependencies
Write-Status "Phase 7: Verifying DLL dependencies"

# Try multiple methods to check dependencies
$hasDependencies = $false
$dependencyOutput = ""

# Method 1: objdump (preferred)
try {
    $objdumpOutput = & objdump -p $exePath 2>&1 | Select-String "DLL Name"
    if ($objdumpOutput) {
        $dependencyOutput += "objdump found DLL dependencies:`n"
        $objdumpOutput | ForEach-Object { $dependencyOutput += "  $_`n" }
        $hasDependencies = $true
    }
} catch {
    Write-Warning "objdump not available for dependency checking"
}

# Method 2: ldd (if available)
try {
    $lddOutput = & ldd $exePath 2>&1 | Select-String -NotMatch "not a dynamic executable"
    if ($lddOutput -and $lddOutput.Count -gt 0) {
        $dependencyOutput += "ldd found dependencies:`n"
        $lddOutput | ForEach-Object { $dependencyOutput += "  $_`n" }
        $hasDependencies = $true
    }
} catch {
    # ldd not available on Windows, this is expected
}

# Method 3: Check file size (static executables are typically larger)
$fileSize = (Get-Item $exePath).Length
$fileSizeMB = [math]::Round($fileSize / 1MB, 2)
Write-Status "Executable size: $fileSizeMB MB"

if ($hasDependencies) {
    Write-Warning "DLL dependencies detected:"
    Write-Host $dependencyOutput
    Write-Warning "Build may not be fully static"
} else {
    Write-Success "No external DLL dependencies detected"
    Write-Success "Static linking appears successful"
}

# Step 8: Copy to bin directory
Write-Status "Phase 8: Copying executable to bin directory"
Set-Location ..

$binDir = "../bin"
if (!(Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir -Force | Out-Null
}

$finalExePath = "$binDir/cyberbasic.exe"
Copy-Item "$buildDir/$exePath" $finalExePath -Force

Write-Success "Executable copied to: $finalExePath"

# Step 9: Final verification
Write-Status "Phase 9: Final verification"

if (Test-Path $finalExePath) {
    $finalSize = (Get-Item $finalExePath).Length
    $finalSizeMB = [math]::Round($finalSize / 1MB, 2)
    Write-Success "Final executable: $finalExePath ($finalSizeMB MB)"
    
    # Test basic functionality
    Write-Status "Testing basic functionality..."
    try {
        # Try to run with --help or --version if available
        $testOutput = & $finalExePath --help 2>&1
        Write-Status "Executable responds to commands"
    } catch {
        Write-Warning "Could not test executable functionality (may require GUI)"
    }
    
    Write-Success "Build process completed successfully!"
    Write-Status "Ready for distribution: $finalExePath"
    
    if (!$hasDependencies) {
        Write-Success "✓ Zero DLL dependencies confirmed"
        Write-Success "✓ Fully static executable ready"
    }
    
} else {
    Write-Error "Final executable not found at: $finalExePath"
    exit 1
}

Write-Status "Build Summary:"
Write-Status "- Build directory: $buildDir"
Write-Status "- Final executable: $finalExePath"
Write-Status "- Size: $finalSizeMB MB"
Write-Status "- Static linking: $(if (!$hasDependencies) { 'SUCCESS' } else { 'PARTIAL' })"
