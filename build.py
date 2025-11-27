#!/usr/bin/env python3
"""
CyberBasic Universal Build Script

This script automatically detects your platform and builds CyberBasic with the best
configuration for your system. It handles all the complexity so you don't have to.

Usage:
    python build.py                    # Auto-detect and build
    python build.py --help            # Show all options
    python build.py --msvc            # Force MSVC on Windows (experimental)
    python build.py --debug           # Debug build
    python build.py --clean           # Clean before building
"""

import os
import sys
import subprocess
import platform
import argparse
import shutil
from pathlib import Path

def run_command(cmd, cwd=None, check=True):
    """Run a command and return the result."""
    print(f"Running: {' '.join(cmd) if isinstance(cmd, list) else cmd}")
    if isinstance(cmd, str):
        cmd = cmd.split()
    
    try:
        result = subprocess.run(cmd, cwd=cwd, check=check, 
                              capture_output=True, text=True)
        if result.stdout:
            print(result.stdout)
        return result
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {e}")
        if e.stderr:
            print(f"Error output: {e.stderr}")
        if check:
            sys.exit(1)
        return e

def detect_platform():
    """Detect the current platform."""
    system = platform.system().lower()
    if system == "windows":
        return "windows"
    elif system == "darwin":
        return "macos"
    elif system == "linux":
        return "linux"
    else:
        print(f"Unsupported platform: {system}")
        sys.exit(1)

def check_dependencies():
    """Check if required dependencies are available."""
    dependencies = {
        "cmake": "CMake is required. Install from https://cmake.org/",
        "python": "Python 3.6+ is required for binding generation.",
    }
    
    missing = []
    for dep, msg in dependencies.items():
        try:
            result = run_command([dep, "--version"], check=False)
            if result.returncode != 0:
                missing.append((dep, msg))
        except FileNotFoundError:
            missing.append((dep, msg))
    
    return missing

def check_compiler(force_msvc=False):
    """Check available compilers and return the best one."""
    system = detect_platform()
    
    if system == "windows":
        if force_msvc:
            # Check for MSVC
            try:
                result = run_command(["cl"], check=False)
                if result.returncode == 0 or "Microsoft" in result.stderr:
                    return "msvc"
            except FileNotFoundError:
                pass
            print("MSVC not found. Install Visual Studio or Build Tools.")
            sys.exit(1)
        else:
            # Prefer MinGW-w64
            for compiler in ["gcc", "clang"]:
                try:
                    result = run_command([compiler, "--version"], check=False)
                    if result.returncode == 0:
                        return compiler
                except FileNotFoundError:
                    continue
            
            print("No suitable compiler found.")
            print("Install MinGW-w64 GCC (recommended) or Clang.")
            print("For MSVC support (experimental), use --msvc flag.")
            sys.exit(1)
    else:
        # Linux/macOS
        for compiler in ["gcc", "clang"]:
            try:
                result = run_command([compiler, "--version"], check=False)
                if result.returncode == 0:
                    return compiler
            except FileNotFoundError:
                continue
        
        print("No suitable compiler found.")
        if system == "linux":
            print("Install build-essential: sudo apt install build-essential")
        elif system == "macos":
            print("Install Xcode Command Line Tools: xcode-select --install")
        sys.exit(1)

def get_cmake_generator(compiler, system):
    """Get the appropriate CMake generator."""
    if system == "windows":
        if compiler == "msvc":
            return "Visual Studio 16 2019"  # Or detect version
        else:
            return "MinGW Makefiles"
    else:
        return "Unix Makefiles"

def build_cyberbasic(args):
    """Build CyberBasic with the specified configuration."""
    
    # Get project root (where this script is located)
    script_dir = Path(__file__).parent.absolute()
    source_dir = script_dir / "source"
    
    if not source_dir.exists():
        print(f"Source directory not found: {source_dir}")
        print("Make sure you're running this script from the CyberBasic root directory.")
        sys.exit(1)
    
    # Detect platform and compiler
    system = detect_platform()
    compiler = check_compiler(args.msvc)
    
    print(f"Building CyberBasic on {system} with {compiler}")
    
    # Check dependencies
    missing_deps = check_dependencies()
    if missing_deps:
        print("Missing dependencies:")
        for dep, msg in missing_deps:
            print(f"  - {dep}: {msg}")
        sys.exit(1)
    
    # Create build directory
    build_type = "Debug" if args.debug else "Release"
    build_dir = script_dir / f"build-{system}-{build_type.lower()}"
    
    if args.clean and build_dir.exists():
        print(f"Cleaning build directory: {build_dir}")
        shutil.rmtree(build_dir)
    
    build_dir.mkdir(exist_ok=True)
    
    # Prepare CMake arguments
    cmake_args = [
        "cmake",
        str(source_dir),
        f"-DCMAKE_BUILD_TYPE={build_type}",
    ]
    
    # Add generator
    generator = get_cmake_generator(compiler, system)
    cmake_args.extend(["-G", generator])
    
    # Add platform-specific options
    if args.msvc:
        cmake_args.append("-DALLOW_MSVC=ON")
    
    if args.static:
        cmake_args.append("-DBASIC_STATIC_LINK=ON")
    
    # Configure
    print("Configuring build...")
    run_command(cmake_args, cwd=build_dir)
    
    # Build
    print("Building...")
    build_cmd = ["cmake", "--build", ".", "--config", build_type]
    
    if not args.msvc and system != "windows":
        # Add parallel build for make-based generators
        import multiprocessing
        build_cmd.extend(["-j", str(multiprocessing.cpu_count())])
    
    run_command(build_cmd, cwd=build_dir)
    
    # Find the executable
    exe_name = "cyberbasic.exe" if system == "windows" else "cyberbasic"
    exe_path = build_dir / exe_name
    
    if not exe_path.exists():
        # Try in subdirectories for MSVC builds
        for subdir in [build_type, "Debug", "Release"]:
            alt_path = build_dir / subdir / exe_name
            if alt_path.exists():
                exe_path = alt_path
                break
    
    if exe_path.exists():
        print(f"\nBuild successful!")
        print(f"Executable: {exe_path}")
        print(f"\nTo test your build:")
        print(f"  {exe_path} examples/hello_text.bas")
        
        # Copy to bin directory for convenience
        bin_dir = script_dir / "bin"
        bin_dir.mkdir(exist_ok=True)
        bin_exe = bin_dir / exe_name
        shutil.copy2(exe_path, bin_exe)
        print(f"\nCopied to: {bin_exe}")
        
    else:
        print("Build completed but executable not found.")
        print(f"Check the build directory: {build_dir}")

def main():
    parser = argparse.ArgumentParser(
        description="Universal build script for CyberBasic",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python build.py                    # Standard release build
  python build.py --debug           # Debug build with symbols
  python build.py --clean           # Clean and rebuild
  python build.py --msvc            # Use MSVC on Windows (experimental)
  python build.py --static          # Static linking (default on Windows)

Platform-specific notes:
  Windows: Prefers MinGW-w64 GCC, falls back to Clang, MSVC with --msvc
  Linux:   Uses GCC or Clang, requires build-essential
  macOS:   Uses Clang from Xcode Command Line Tools
        """
    )
    
    parser.add_argument("--debug", action="store_true",
                       help="Build in debug mode with symbols")
    parser.add_argument("--clean", action="store_true",
                       help="Clean build directory before building")
    parser.add_argument("--msvc", action="store_true",
                       help="Use MSVC on Windows (experimental, may have issues)")
    parser.add_argument("--static", action="store_true",
                       help="Enable static linking (default on Windows)")
    
    args = parser.parse_args()
    
    try:
        build_cyberbasic(args)
    except KeyboardInterrupt:
        print("\nBuild cancelled by user")
        sys.exit(1)
    except Exception as e:
        print(f"Build failed: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
