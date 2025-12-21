# detect-compiler.ps1
# Detects available compilers and returns appropriate CMake generator string
# Returns: "MinGW Makefiles" or "Visual Studio 16 2019" or empty string if none found

param(
    [switch]$Verbose = $false
)

function Write-Verbose-Output($msg) {
    if ($Verbose) {
        Write-Host "[detect-compiler] $msg" -ForegroundColor Gray
    }
}

# Check for MinGW-w64 (preferred)
$mingwFound = $false
$mingwPath = $null

# Check common MSYS2 installation path
if (Test-Path "C:\msys64\mingw64\bin\gcc.exe") {
    $mingwFound = $true
    $mingwPath = "C:\msys64\mingw64\bin"
    Write-Verbose-Output "MinGW-w64 found at: $mingwPath"
} elseif (Test-Path "C:\msys2\mingw64\bin\gcc.exe") {
    $mingwFound = $true
    $mingwPath = "C:\msys2\mingw64\bin"
    Write-Verbose-Output "MinGW-w64 found at: $mingwPath"
} elseif (Get-Command gcc -ErrorAction SilentlyContinue) {
    $gccPath = (Get-Command gcc).Source
    if ($gccPath -like "*mingw*" -or $gccPath -like "*msys*") {
        $mingwFound = $true
        $mingwPath = Split-Path $gccPath
        Write-Verbose-Output "MinGW-w64 found in PATH: $gccPath"
    }
}

if ($mingwFound) {
    Write-Verbose-Output "Selected generator: MinGW Makefiles"
    return "MinGW Makefiles"
}

# Check for MSVC (optional fallback, but project prefers MinGW)
$msvcFound = $false
if (Get-Command cl -ErrorAction SilentlyContinue) {
    $clPath = (Get-Command cl).Source
    if ($clPath -like "*Visual Studio*" -or $clPath -like "*MSBuild*") {
        $msvcFound = $true
        Write-Verbose-Output "MSVC found at: $clPath"
    }
}

# Try to detect Visual Studio installation
if (-not $msvcFound) {
    $vsVersions = @(
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
    )
    
    foreach ($vsPath in $vsVersions) {
        if (Test-Path $vsPath) {
            $msvcFound = $true
            Write-Verbose-Output "Visual Studio found at: $vsPath"
            break
        }
    }
}

if ($msvcFound) {
    Write-Verbose-Output "MSVC found, but MinGW is preferred for this project"
    Write-Verbose-Output "To use MSVC, set -DALLOW_MSVC=ON in CMake"
    # Don't return MSVC generator - project prefers MinGW
}

# No suitable compiler found
Write-Verbose-Output "No suitable compiler found"
Write-Verbose-Output "Please install MinGW-w64 from https://www.msys2.org/"
return ""

