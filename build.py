import subprocess
import os

def main():
    msys_bash = 'C:\\msys64\\usr\\bin\\bash.exe'

    project_root = os.getcwd()
    # Convert Windows path C:\... to MSYS2 path /c/...
    msys_path = project_root.replace('C:', '/c').replace('\\', '/')

    # The -l flag can cause bash to start in the user's home directory, so we
    # must explicitly cd to the project root before running the script.
    build_command = f'"{msys_bash}" -l -c "cd \'{msys_path}\' && ./scripts/build-mingw.sh"'

    print(f'--- Running build command: {build_command} ---')
    try:
        # Using a list of arguments is more robust than shell=True on Windows.
        # We also remove output capturing to see real-time output.
        command_list = [msys_bash, '-l', '-c', f"cd '{msys_path}' && ./scripts/build-mingw.sh"]
        process = subprocess.run(
            command_list,
            cwd=os.getcwd()
        )

        print(f'--- Return Code: {process.returncode} ---')

        if process.returncode != 0:
            print('Build failed.')
        else:
            print('Build successful.')

    except Exception as e:
        print(f'An exception occurred: {e}')

if __name__ == '__main__':
    main()
