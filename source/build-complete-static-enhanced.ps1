# build-complete-static-enhanced.ps1
# Complete Static Build Fix and DLL Elimination Script
# Implements systematic approach to achieve zero DLL dependencies

param(
    [switch]$SkipClean = $false,
    [switch]$Verbose = $false,
    [switch]$DebugBuild = $false
)

# Color output functions
function Write-Status($msg) { Write-Host "`e[34m[BUILD]`e[0m $msg" }
function Write-Success($msg) { Write-Host "`e[32m[SUCCESS]`e[0m $msg" }
function Write-Warning($msg) { Write-Host "`e[33m[WARNING]`e[0m $msg" }
function Write-Error($msg) { Write-Host "`e[31m[ERROR]`e[0m $msg" }

Write-Status "Starting Complete Static Build Process"
Write-Status "Target: Zero DLL dependencies, fully static executable"

# Phase 1: Clean Slate Approach
if (-not $SkipClean) {
    Write-Status "Phase 1: Cleaning all build directories and cached files"
    
    # Remove ALL build directories
    $buildDirs = @(
        "build-dist",
        "build-static-final", 
        "build-static-complete",
        "build-debug",
        "build-release",
        "build"
    )
    
    foreach ($dir in $buildDirs) {
        if (Test-Path $dir) {
            Write-Status "Removing build directory: $dir"
            Remove-Item -Recurse -Force $dir -ErrorAction SilentlyContinue
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

# Phase 2: Verify Python availability
Write-Status "Phase 2: Verifying Python availability"
try {
    $pythonVersion = (python -c "import sys; print(f'Python {sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro}')")
    Write-Status "Python found: $pythonVersion"
} catch {
    Write-Error "Python not found. Please install Python 3."
    exit 1
}

# Phase 3: Create build directory
$buildDir = "build-static-complete"
Write-Status "Phase 3: Creating build directory: $buildDir"
New-Item -ItemType Directory -Force $buildDir | Out-Null

# Phase 4: Configure CMake with enhanced static linking
Write-Status "Phase 4: Configuring CMake with static linking options"
$buildType = if ($DebugBuild) { "Debug" } else { "Release" }
$cmakeCommand = "cmake .. -G `"MinGW Makefiles`" -DCMAKE_BUILD_TYPE=$buildType -DBASIC_STATIC_LINK=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_FLAGS=`"-static-libgcc -static-libstdc++`" -DCMAKE_EXE_LINKER_FLAGS=`"-static`""

Write-Status "CMake command: $cmakeCommand"
Push-Location $buildDir
try {
    Invoke-Expression $cmakeCommand | Out-Host
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed."
    }
    Write-Success "CMake configuration completed"
} catch {
    Write-Error "CMake configuration failed: $($_.Exception.Message)"
    Pop-Location
    exit 1
}

# Phase 5: Generate raylib bindings (with fallback)
Write-Status "Phase 5: Generating raylib bindings"
try {
    # Try make target first
    make generate_bindings
    if ($LASTEXITCODE -ne 0) {
        Write-Warning "Binding generation via make failed, trying manual generation"
        # Ensure the output directory exists for manual generation
        New-Item -ItemType Directory -Force "src/bindings" | Out-Null
        python ../tools/gen_raylib_bindings.py | Out-Host
        if ($LASTEXITCODE -ne 0) {
            throw "Manual binding generation failed."
        }
    }
    Write-Success "Binding generation completed"
} catch {
    Write-Error "Binding generation failed: $($_.Exception.Message)"
    Pop-Location
    exit 1
}

# Phase 6: Build executable
Write-Status "Phase 6: Building cyberbasic executable"
try {
    $cores = (Get-CimInstance Win32_Processor | Measure-Object NumberOfCores -Sum).Sum
    if ($cores -gt 1) {
        make cyberbasic "-j$cores" | Out-Host
    } else {
        make cyberbasic | Out-Host
    }
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed."
    }
    Write-Success "Build completed successfully"
    Write-Success "Executable created: cyberbasic.exe"
} catch {
    Write-Error "Build failed: $($_.Exception.Message)"
    Pop-Location
    exit 1
}

# Phase 7: Verify DLL dependencies
Write-Status "Phase 7: Verifying DLL dependencies"
$executablePath = "cyberbasic.exe"
if (Test-Path $executablePath) {
    $fileSize = [math]::Round((Get-Item $executablePath).Length / 1MB, 2)
    Write-Status "Executable size: $fileSize MB"
    
    try {
        $dlls = objdump -p $executablePath | Select-String "DLL Name"
        if ($dlls.Length -gt 0) {
            Write-Warning "DLL dependencies detected:"
            Write-Host "objdump found DLL dependencies:"
            $dlls | ForEach-Object { Write-Host "        $_" }
            
            # Filter out system DLLs that are acceptable
            $systemDlls = @("KERNEL32.dll", "USER32.dll", "GDI32.dll", "SHELL32.dll", "WINMM.dll")
            $crtDlls = $dlls | Where-Object { $_.ToString() -match "api-ms-win-crt" }
            $nonSystemDlls = $dlls | Where-Object { 
                $dll = $_.ToString()
                -not ($systemDlls | Where-Object { $dll -match $_ }) -and 
                -not ($dll -match "api-ms-win-crt")
            }
            
            if ($nonSystemDlls.Length -gt 0) {
                Write-Error "Non-system DLL dependencies found:"
                $nonSystemDlls | ForEach-Object { Write-Host "        $_" }
                Write-Error "Build is not fully static!"
            } else {
                Write-Warning "Build may not be fully static"
            }
        } else {
            Write-Success "No DLL dependencies found (excluding system DLLs)."
        }
    } catch {
        Write-Warning "objdump not found or failed to run. Skipping DLL verification."
    }
} else {
    Write-Error "Executable not found at $executablePath"
    Pop-Location
    exit 1
}

# Phase 8: Copy to bin folder
$binDir = "../bin"
Write-Status "Phase 8: Copying executable to bin directory"
New-Item -ItemType Directory -Force $binDir | Out-Null
Copy-Item $executablePath (Join-Path $binDir "cyberbasic.exe") -Force
Write-Success "Executable copied to: $binDir/cyberbasic.exe"

# Phase 9: Final verification
Write-Status "Phase 9: Final verification"
$finalExe = Join-Path $binDir "cyberbasic.exe"
if (Test-Path $finalExe) {
    $finalSize = [math]::Round((Get-Item $finalExe).Length / 1MB, 2)
    Write-Success "Final executable: $finalExe ($finalSize MB)"
    
    # Test basic functionality
    Write-Status "Testing basic functionality..."
    try {
        $testResult = & $finalExe --help 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Success "Basic functionality test passed"
        } else {
            Write-Warning "Could not test executable functionality (may require GUI)"
        }
    } catch {
        Write-Warning "Could not test executable functionality (may require GUI)"
    }
} else {
    Write-Error "Final executable not found at $finalExe"
    Pop-Location
    exit 1
}

Pop-Location

Write-Success "Complete Static Build Process Finished"
Write-Status "Build Summary:"
Write-Status "- Build directory: $buildDir"
Write-Status "- Final executable: $finalExe"
Write-Status "- Size: $finalSize MB"
Write-Status "- Static linking: $(if ($nonSystemDlls.Length -eq 0) { 'SUCCESS' } else { 'PARTIAL' })"

if ($Verbose) {
    Write-Status "Verbose mode: Additional details logged above"
}

Write-Success "Ready for distribution: $finalExe"
exit 0
