# build-robust.ps1
# Robust build script that handles file locks, generator mismatches, and ensures clean compilation
# Designed for easy use by anyone wanting to build CyberBasic

param(
    [switch]$Clean = $false,
    [switch]$Debug = $false,
    [switch]$Static = $true,
    [switch]$Verbose = $false
)

# Color output functions
function Write-Status($msg) { Write-Host "`e[34m[BUILD]`e[0m $msg" }
function Write-Success($msg) { Write-Host "`e[32m[SUCCESS]`e[0m $msg" }
function Write-Warning($msg) { Write-Host "`e[33m[WARNING]`e[0m $msg" }
function Write-Error($msg) { Write-Host "`e[31m[ERROR]`e[0m $msg" }

# Stop build processes that might lock files
function Stop-BuildProcesses {
    Write-Status "Stopping build processes that might lock files..."
    $processes = @("cl", "link", "mspdbsrv", "gcc", "g++", "cmake", "make", "mingw32-make")
    $stopped = 0
    foreach ($proc in $processes) {
        try {
            $procs = Get-Process -Name $proc -ErrorAction SilentlyContinue | Where-Object { $_.Id -ne $PID }
            foreach ($p in $procs) {
                Write-Verbose "Stopping process: $($p.ProcessName) (PID: $($p.Id))"
                Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue
                $stopped++
            }
        } catch {
            # Ignore errors - process might not exist
        }
    }
    if ($stopped -gt 0) {
        Write-Status "Stopped $stopped process(es)"
        Start-Sleep -Milliseconds 1000  # Wait for cleanup
    } else {
        Write-Success "No build processes found"
    }
}

# Remove locked directory with retry logic
function Remove-LockedDirectory {
    param([string]$Path)
    
    if (-not (Test-Path $Path)) {
        return $true
    }
    
    Stop-BuildProcesses
    
    for ($i = 0; $i -lt 5; $i++) {
        try {
            Remove-Item -Recurse -Force $Path -ErrorAction Stop
            Start-Sleep -Milliseconds 200  # Brief pause to ensure filesystem updates
            if (-not (Test-Path $Path)) {
                return $true
            }
        } catch {
            if ($i -lt 4) {
                Write-Warning "Attempt $($i + 1) failed to remove $Path, retrying..."
                Start-Sleep -Milliseconds 500
            } else {
                Write-Error "Failed to remove $Path after 5 attempts: $($_.Exception.Message)"
                return $false
            }
        }
    }
    return $false
}

# Detect and validate generator from existing build directory
function Get-CachedGenerator {
    param([string]$BuildDir)
    
    $cacheFile = Join-Path $BuildDir "CMakeCache.txt"
    if (-not (Test-Path $cacheFile)) {
        return $null
    }
    
    try {
        $content = Get-Content $cacheFile -Raw
        if ($content -match "CMAKE_GENERATOR:INTERNAL=([^\r\n]+)") {
            return $matches[1].Trim()
        }
    } catch {
        Write-Warning "Could not read CMakeCache.txt: $($_.Exception.Message)"
    }
    
    return $null
}

# Detect available compiler and return generator
function Get-RecommendedGenerator {
    $detectScript = Join-Path $PSScriptRoot "tools\detect-compiler.ps1"
    if (Test-Path $detectScript) {
        $generator = & $detectScript -Verbose:$Verbose
        if ($generator) {
            return $generator
        }
    }
    
    # Fallback detection
    if (Test-Path "C:\msys64\mingw64\bin\gcc.exe") {
        return "MinGW Makefiles"
    } elseif (Get-Command gcc -ErrorAction SilentlyContinue) {
        return "MinGW Makefiles"
    }
    
    return "MinGW Makefiles"  # Default assumption
}

Write-Status "CyberBasic Robust Build Script"
Write-Status "=============================="

# Phase 1: Check prerequisites
Write-Status "Phase 1: Checking prerequisites"

# Check Python
try {
    $pythonVersion = python --version 2>&1
    Write-Success "Python found: $pythonVersion"
} catch {
    Write-Error "Python not found. Please install Python 3.6+"
    exit 1
}

# Check CMake
try {
    $cmakeVersion = cmake --version 2>&1 | Select-Object -First 1
    Write-Success "CMake found: $cmakeVersion"
} catch {
    Write-Error "CMake not found. Please install CMake 3.10+"
    exit 1
}

# Detect recommended generator
$recommendedGenerator = Get-RecommendedGenerator
Write-Success "Recommended generator: $recommendedGenerator"

# Phase 2: Detect and validate generator consistency
Write-Status "Phase 2: Validating build directory generator"

$buildType = if ($Debug) { "Debug" } else { "Release" }
$buildDir = "build"

# Check if build directory exists and validate generator
$needsClean = $false
if (Test-Path $buildDir) {
    $cachedGenerator = Get-CachedGenerator $buildDir
    if ($cachedGenerator) {
        Write-Status "Found existing build directory with generator: $cachedGenerator"
        
        if ($cachedGenerator -ne $recommendedGenerator) {
            Write-Warning "Generator mismatch detected!"
            Write-Warning "  Cached:  $cachedGenerator"
            Write-Warning "  Desired: $recommendedGenerator"
            Write-Warning "Build directory will be cleaned to prevent conflicts"
            $needsClean = $true
        } else {
            Write-Success "Generator matches: $cachedGenerator"
        }
    } else {
        Write-Warning "Build directory exists but CMakeCache.txt not found or invalid"
        Write-Warning "Will clean and reconfigure"
        $needsClean = $true
    }
} else {
    Write-Success "No existing build directory found"
}

# Phase 3: Clean build directory if needed
if ($Clean -or $needsClean) {
    Write-Status "Phase 3: Cleaning build directories"
    
    $buildDirs = @("build", "build-release", "build-debug", "build-static-complete", "build-robust-Release", "build-robust-Debug")
    foreach ($dir in $buildDirs) {
        if (Test-Path $dir) {
            Write-Status "Removing: $dir"
            if (-not (Remove-LockedDirectory $dir)) {
                Write-Error "Failed to remove $dir. Please close any programs using files in this directory."
                Write-Error "You may need to:"
                Write-Error "  1. Close Visual Studio / VS Code / any IDE"
                Write-Error "  2. Close file explorer windows showing the build directory"
                Write-Error "  3. Run: Get-Process | Where-Object {`$_.Path -like '*build*'} | Stop-Process -Force"
                exit 1
            }
        }
    }
    Write-Success "Cleanup completed"
}

# Phase 4: Stop locking processes
Write-Status "Phase 4: Checking for file-locking processes"
Stop-BuildProcesses

# Phase 5: Create build directory
Write-Status "Phase 5: Creating build directory: $buildDir"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
    Write-Success "Build directory created"
} else {
    Write-Success "Build directory exists"
}

Push-Location $buildDir

try {
    # Phase 6: Configure CMake
    Write-Status "Phase 6: Configuring CMake with generator: $recommendedGenerator"
    $cmakeArgs = @(
        ".."
        "-G", $recommendedGenerator
        "-DCMAKE_BUILD_TYPE=$buildType"
    )
    
    if ($Static) {
        $cmakeArgs += "-DBASIC_STATIC_LINK=ON"
        $cmakeArgs += "-DBUILD_SHARED_LIBS=OFF"
        if ($recommendedGenerator -eq "MinGW Makefiles") {
            $cmakeArgs += "-DCMAKE_CXX_FLAGS=-static-libgcc -static-libstdc++"
            $cmakeArgs += "-DCMAKE_EXE_LINKER_FLAGS=-static"
        }
    }
    
    if ($Verbose) {
        $cmakeArgs += "--debug-output"
    }
    
    Write-Status "CMake command: cmake $($cmakeArgs -join ' ')"
    & cmake @cmakeArgs 2>&1 | Out-Host
    
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed with exit code $LASTEXITCODE"
    }
    Write-Success "CMake configuration completed"
    
    # Verify CMakeCache.txt was created with correct generator
    $cacheFile = "CMakeCache.txt"
    if (Test-Path $cacheFile) {
        $cachedGen = Get-CachedGenerator "."
        if ($cachedGen -eq $recommendedGenerator) {
            Write-Success "Verified generator in CMakeCache.txt: $cachedGen"
        } else {
            Write-Warning "Generator mismatch in cache file (this shouldn't happen)"
        }
    } else {
        throw "CMakeCache.txt was not created"
    }
    
    # Phase 7: Generate bindings first (critical step)
    Write-Status "Phase 7: Generating raylib bindings"
    Write-Status "This may take a minute..."
    
    $bindingsGenerated = $false
    
    # Try to generate bindings via CMake target
    try {
        & cmake --build . --target generate_bindings 2>&1 | Out-Host
        if ($LASTEXITCODE -eq 0) {
            $bindingsGenerated = $true
            Write-Success "Bindings generated via CMake target"
        }
    } catch {
        Write-Warning "CMake binding generation failed: $($_.Exception.Message)"
    }
    
    # Fallback: Manual generation
    if (-not $bindingsGenerated) {
        Write-Status "Generating bindings manually..."
        Push-Location ..
        try {
            # Ensure we're in the source directory
            if (-not (Test-Path "tools\gen_raylib_bindings.py")) {
                throw "Could not find tools\gen_raylib_bindings.py"
            }
            if (-not (Test-Path "specs")) {
                throw "Could not find specs directory"
            }
            
            # Run generator (it will detect build directory from cwd)
            python tools/gen_raylib_bindings.py 2>&1 | Out-Host
            if ($LASTEXITCODE -eq 0) {
                $bindingsGenerated = $true
                Write-Success "Bindings generated manually"
            } else {
                throw "Manual binding generation failed with exit code $LASTEXITCODE"
            }
        } finally {
            Pop-Location
        }
    }
    
    if (-not $bindingsGenerated) {
        throw "Failed to generate bindings"
    }
    
    # Verify bindings file exists and is non-empty
    $bindingsFile = "src\bindings\rt_raylib.gen.cpp"
    if (-not (Test-Path $bindingsFile)) {
        throw "Generated bindings file not found: $bindingsFile"
    }
    
    $bindingsInfo = Get-Item $bindingsFile
    if ($bindingsInfo.Length -eq 0) {
        throw "Generated bindings file is empty: $bindingsFile"
    }
    
    Write-Success "Bindings file verified: $bindingsFile ($([math]::Round($bindingsInfo.Length / 1KB, 2)) KB)"
    
    # Phase 8: Build executable
    Write-Status "Phase 8: Building cyberbasic executable"
    Write-Status "This may take several minutes..."
    
    $cpuCount = (Get-CimInstance Win32_ComputerSystem).NumberOfLogicalProcessors
    Write-Status "Using $cpuCount parallel jobs"
    
    & cmake --build . --config $buildType -j $cpuCount 2>&1 | Out-Host
    
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed with exit code $LASTEXITCODE"
    }
    
    Write-Success "Build completed successfully!"
    
    # Phase 9: Verify output
    Write-Status "Phase 9: Verifying build output"
    $exePath = if ($IsWindows -or $env:OS -like "*Windows*") { 
        "cyberbasic.exe" 
    } else { 
        "cyberbasic" 
    }
    
    if (Test-Path $exePath) {
        $fileInfo = Get-Item $exePath
        Write-Success "Executable found: $exePath"
        Write-Success "Size: $([math]::Round($fileInfo.Length / 1MB, 2)) MB"
        Write-Success "Location: $($fileInfo.FullName)"
        
        # Verify executable is actually executable (not just a file)
        try {
            $versionOutput = & ".\$exePath" --version 2>&1 | Out-String
            if ($LASTEXITCODE -eq 0 -or $versionOutput -match "cyberbasic|version|help") {
                Write-Success "Executable is functional"
            }
        } catch {
            Write-Warning "Could not verify executable functionality: $($_.Exception.Message)"
        }
    } else {
        throw "Executable not found: $exePath"
    }
    
    # Phase 10: Quick test
    Write-Status "Phase 10: Running quick test"
    $testScript = "..\examples\hello_text.bas"
    if (Test-Path $testScript) {
        Write-Status "Testing with: $testScript"
        $testOutput = & ".\$exePath" $testScript 2>&1 | Select-Object -First 10
        $testOutput | Out-Host
        if ($LASTEXITCODE -eq 0) {
            Write-Success "Test passed!"
        } else {
            Write-Warning "Test had issues (exit code: $LASTEXITCODE) - but executable was built successfully"
        }
    } else {
        Write-Warning "Test script not found: $testScript (skipping test)"
    }
    
    Write-Status "=============================="
    Write-Success "BUILD COMPLETE!"
    Write-Status "=============================="
    Write-Host ""
    Write-Host "Executable: $($fileInfo.FullName)" -ForegroundColor Green
    Write-Host ""
    Write-Host "To run examples:" -ForegroundColor Cyan
    Write-Host "  .\$exePath ..\examples\hello_text.bas"
    Write-Host "  .\$exePath ..\examples\pong_game.bas"
    Write-Host "  .\$exePath ..\examples\2d_game_template.bas"
    Write-Host ""
    
} catch {
    Write-Error "Build failed: $($_.Exception.Message)"
    if ($Verbose) {
        Write-Error "Stack trace: $($_.ScriptStackTrace)"
    }
    Write-Host ""
    Write-Host "Troubleshooting tips:" -ForegroundColor Yellow
    Write-Host "  1. Try running with -Clean flag: .\build-robust.ps1 -Clean"
    Write-Host "  2. Close any IDEs (VS Code, Visual Studio) that might lock files"
    Write-Host "  3. Check that MinGW-w64 is installed: C:\msys64\mingw64\bin\gcc.exe"
    Write-Host "  4. Verify Python and CMake are in PATH"
    Write-Host ""
    Pop-Location
    exit 1
} finally {
    Pop-Location
}
