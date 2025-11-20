import subprocess
import os
import sys

def run_command(command, env):
    """Runs a command and prints its output in real-time."""
    print(f"\n--- Running: {' '.join(command)} ---")
    try:
        process = subprocess.Popen(
            command,
            env=env,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
            universal_newlines=True
        )

        for line in process.stdout:
            print(line, end='')

        process.wait()
        return_code = process.returncode
    except Exception as e:
        print(f"An exception occurred: {e}")
        return_code = -1

    print(f"\n--- Return Code: {return_code} ---")
    return return_code

def main():
    # Set up the environment for MinGW
    env = os.environ.copy()
    mingw_path = "C:\\msys64\\mingw64\\bin;C:\\msys64\\usr\\bin"
    env["PATH"] = mingw_path + os.pathsep + env["PATH"]

    # Define CMake commands
    cmake_configure_cmd = [
        "cmake",
        "-S", ".",
        "-B", "build",
        "-G", "Ninja",
        "-DCMAKE_BUILD_TYPE=Release"
    ]
    cmake_build_cmd = ["cmake", "--build", "build", "--config", "Release"]

    # Run configure step
    if run_command(cmake_configure_cmd, env) != 0:
        print("\nCMake configuration failed!")
        sys.exit(1)

    print("\nCMake configuration successful.")

    # Run build step
    if run_command(cmake_build_cmd, env) != 0:
        print("\nBuild failed!")
        sys.exit(1)

    print("\nBuild completed successfully!")

if __name__ == '__main__':
    main()
