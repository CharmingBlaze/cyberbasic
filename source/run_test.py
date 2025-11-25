import subprocess
import os

def run_test(test_file, out_file, err_file, exit_code_file):
    executable = os.path.join("build", "cyberbasic.exe")
    command = [executable, test_file]
    try:
        with open(out_file, 'w') as f_out, open(err_file, 'w') as f_err:
            result = subprocess.run(command, stdout=f_out, stderr=f_err, text=True, check=False)
        with open(exit_code_file, 'w') as f_exit:
            f_exit.write(str(result.returncode))
    except Exception as e:
        with open(err_file, 'a') as f_err:
            f_err.write(f"\nPython script error: {e}")
        with open(exit_code_file, 'w') as f_exit:
            f_exit.write('1')

if __name__ == "__main__":
    run_test("tests/for_in_basic.bas", "out_forin.txt", "err_forin.txt", "exit_forin.txt")
