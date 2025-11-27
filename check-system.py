#!/usr/bin/env python3
"""
CyberBasic System Check

This script checks your system for CyberBasic build requirements and provides
helpful suggestions for missing dependencies.

Usage:
    python check-system.py
"""

import os
import sys
import subprocess
import platform
import shutil
from pathlib import Path

def run_command(cmd, check=False):
    """Run a command and return the result."""
    try:
        if isinstance(cmd, str):
            cmd = cmd.split()
        result = subprocess.run(cmd, capture_output=True, text=True, check=check)
        return result
    except (subprocess.CalledProcessError, FileNotFoundError):
        return None

def check_command(name, cmd, version_flag="--version"):
    """Check if a command is available and get its version."""
    print(f"Checking {name}...", end=" ")
    
    result = run_command([cmd, version_flag])
    if result and result.returncode == 0:
        # Extract version from output (first line usually)
        version = result.stdout.split('\n')[0].strip()
        print(f"Found: {version}")
        return True
    else:
        print(f"Not found")
        return False

def check_python():
    """Check Python version."""
    print(f"Checking Python...", end=" ")
    version = sys.version.split()[0]
    major, minor = map(int, version.split('.')[:2])
    
    if major >= 3 and minor >= 6:
        print(f"Found: Python {version}")
        return True
    else:
        print(f"Found Python {version}, but need 3.6+")
        return False

def detect_platform():
    """Detect and display platform information."""
    system = platform.system()
    release = platform.release()
    machine = platform.machine()
    
    print(f"Platform: {system} {release} ({machine})")
    return system.lower()

def check_compilers():
    """Check available compilers."""
    print("\nCompilers:")
    
    compilers = []
    
    # Check GCC
    if check_command("GCC", "gcc"):
        compilers.append("gcc")
    
    # Check Clang
    if check_command("Clang", "clang"):
        compilers.append("clang")
    
    # Check MSVC (Windows only)
    if platform.system().lower() == "windows":
        print("Checking MSVC...", end=" ")
        result = run_command(["cl"])
        if result and ("Microsoft" in result.stderr or result.returncode == 0):
            print("Found: Microsoft Visual C++")
            compilers.append("msvc")
        else:
            print("Not found")
    
    return compilers

def check_build_tools():
    """Check build tools."""
    print("\nBuild Tools:")
    
    tools = {}
    tools['cmake'] = check_command("CMake", "cmake")
    tools['make'] = check_command("Make", "make")
    tools['ninja'] = check_command("Ninja", "ninja")
    
    return tools

def check_git():
    """Check Git."""
    print("\nVersion Control:")
    return check_command("Git", "git")

def provide_installation_help(system, missing_compilers, missing_tools):
    """Provide installation suggestions."""
    print("\nInstallation Suggestions:")
    
    if system == "windows":
        if not missing_compilers:
            print("You have compilers installed!")
        else:
            print("No compilers found. Install one of:")
            print("   • MinGW-w64 via MSYS2: https://www.msys2.org/")
            print("     Then: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake")
            print("   • Visual Studio: https://visualstudio.microsoft.com/")
            print("     (Use --msvc flag with build script)")
        
        if not missing_tools.get('cmake', True):
            print("❌ CMake not found:")
            print("   • Download from: https://cmake.org/")
            print("   • Or via MSYS2: pacman -S mingw-w64-x86_64-cmake")
    
    elif system == "linux":
        print("Install missing packages:")
        
        # Detect Linux distribution
        if Path("/etc/debian_version").exists():
            # Debian/Ubuntu
            packages = []
            if not missing_compilers:
                packages.append("build-essential")
            if not missing_tools.get('cmake', True):
                packages.append("cmake")
            
            if packages:
                print(f"   sudo apt install {' '.join(packages)}")
        
        elif Path("/etc/redhat-release").exists():
            # RHEL/Fedora
            packages = []
            if not missing_compilers:
                packages.append("gcc-c++ make")
            if not missing_tools.get('cmake', True):
                packages.append("cmake")
            
            if packages:
                print(f"   sudo dnf install {' '.join(packages)}")
        
        else:
            print("   • Install build-essential or equivalent for your distribution")
            print("   • Install cmake package")
    
    elif system == "darwin":  # macOS
        if not missing_compilers:
            print("No compilers found:")
            print("   xcode-select --install")
        
        if not missing_tools.get('cmake', True):
            print("❌ CMake not found:")
            print("   brew install cmake")

def check_cyberbasic_source():
    """Check if CyberBasic source is available."""
    print("\nCyberBasic Source:")
    
    source_dir = Path("source")
    cmake_file = source_dir / "CMakeLists.txt"
    
    if source_dir.exists() and cmake_file.exists():
        print("Source directory found")
        return True
    else:
        print("Source directory not found")
        print("   Make sure you're in the CyberBasic root directory")
        print("   Or clone the repository:")
        print("   git clone https://github.com/CharmingBlaze/cyberbasic.git")
        return False

def main():
    print("CyberBasic System Check")
    print("======================")
    
    # Platform info
    system = detect_platform()
    print()
    
    # Python check
    print("Python:")
    python_ok = check_python()
    
    # Compiler check
    compilers = check_compilers()
    
    # Build tools check
    tools = check_build_tools()
    
    # Git check
    git_ok = check_git()
    
    # Source check
    source_ok = check_cyberbasic_source()
    
    # Summary
    print("\nSummary:")
    
    ready = True
    
    if not python_ok:
        print("Python 3.6+ required")
        ready = False
    
    if not compilers:
        print("No compilers found")
        ready = False
    
    if not tools.get('cmake', False):
        print("CMake required")
        ready = False
    
    if not source_ok:
        print("CyberBasic source not found")
        ready = False
    
    if ready:
        print("Your system is ready to build CyberBasic!")
        print("\nNext steps:")
        print("   python build.py")
        print("   # or")
        if system == "windows":
            print("   build.bat")
        else:
            print("   ./build.sh")
    else:
        print("Some requirements are missing")
        provide_installation_help(system, not bool(compilers), tools)
    
    print(f"\nFor detailed instructions, see: INSTALL.md")
    print(f"For help, visit: https://github.com/CharmingBlaze/cyberbasic/issues")

if __name__ == "__main__":
    main()
