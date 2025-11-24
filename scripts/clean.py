import os
import shutil
import stat

def on_rm_error(func, path, exc_info):
    """
    Error handler for shutil.rmtree.

    If the error is due to an access error (read only file)
    it attempts to add write permission and then retries.

    If the error is for another reason it re-raises the error.
    """
    # path contains the path of the file that couldn't be removed
    # let's just assume that it's read-only and unlink it.
    os.chmod(path, stat.S_IWRITE)
    func(path)

def main():
    build_dir = 'build'
    if os.path.exists(build_dir):
        print(f"Forcefully removing directory: {build_dir}")
        try:
            shutil.rmtree(build_dir, onerror=on_rm_error)
            print("Directory removed successfully.")
        except Exception as e:
            print(f"Error: Failed to remove directory {build_dir}. Reason: {e}")
    else:
        print(f"Directory '{build_dir}' does not exist.")

if __name__ == '__main__':
    main()
