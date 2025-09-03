import subprocess
import os
import sys

def main():
    print('--- Starting dependency installation ---')
    msys_bash = 'C:\\msys64\\usr\\bin\\bash.exe'
    packages = [
        'mingw-w64-x86_64-toolchain',
        'mingw-w64-x86_64-cmake',
        'mingw-w64-x86_64-ninja',
        'mingw-w64-x86_64-openal'
    ]
    install_command = f'pacman -S --noconfirm --needed {' '.join(packages)}'
    full_command = f'\"{msys_bash}\" -l -c \"{install_command}\"' 

    print(f'--- Running installation command: {full_command} ---')
    try:
        # Using shell=True to correctly interpret the command string with bash
        process = subprocess.run(
            full_command, 
            capture_output=True, 
            text=True, 
            shell=True, 
            cwd=os.getcwd()
        )

        print('--- STDOUT ---')
        print(process.stdout)
        print('--- STDERR ---')
        print(process.stderr)
        print(f'--- Return Code: {process.returncode} ---')

        if process.returncode != 0:
            print('Installation failed.')
        else:
            print('Installation successful.')

    except Exception as e:
        print(f'An exception occurred: {e}')

if __name__ == '__main__':
    main()
