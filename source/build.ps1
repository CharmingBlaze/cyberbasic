# build.ps1
# Simple wrapper for build-robust.ps1 with sensible defaults
# This is the main entry point for building CyberBasic

param(
    [switch]$Clean = $false,
    [switch]$Debug = $false,
    [switch]$Static = $true,
    [switch]$Verbose = $false,
    [switch]$Help = $false
)

if ($Help) {
    Write-Host @"
CyberBasic Build Script
=======================

Usage:
  .\build.ps1 [options]

Options:
  -Clean      Clean build directories before building
  -Debug      Build debug version (default: Release)
  -Static     Enable static linking (default: ON)
  -Verbose    Show verbose output
  -Help       Show this help message

Examples:
  .\build.ps1                    # Standard release build
  .\build.ps1 -Clean             # Clean rebuild
  .\build.ps1 -Debug             # Debug build
  .\build.ps1 -Clean -Static     # Clean static build

The script automatically:
  - Detects available compilers
  - Validates generator consistency
  - Handles file locks
  - Generates bindings
  - Builds the executable

For more details, see BUILD_GUIDE.md
"@
    exit 0
}

# Get the directory where this script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$robustScript = Join-Path $scriptDir "build-robust.ps1"

if (-not (Test-Path $robustScript)) {
    Write-Error "build-robust.ps1 not found at: $robustScript"
    Write-Error "Please ensure you're running this from the source directory"
    exit 1
}

# Build argument array for build-robust.ps1
$args = @()
if ($Clean) { $args += "-Clean" }
if ($Debug) { $args += "-Debug" }
if ($Static) { $args += "-Static" }
if ($Verbose) { $args += "-Verbose" }

# Call the robust build script
Write-Host "Calling build-robust.ps1 with arguments: $($args -join ' ')" -ForegroundColor Cyan
& $robustScript @args

exit $LASTEXITCODE
