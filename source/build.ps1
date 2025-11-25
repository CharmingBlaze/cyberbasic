# Set the correct path for the MinGW compiler
$env:PATH="C:\msys64\mingw64\bin;C:\msys64\usr\bin;" + $env:PATH

Write-Host "Running build and logging to full_build_log.txt..."

# Run both commands and redirect all output to a file.
# The `*>&1` part redirects all output streams to the success stream, which is then piped to the file.
{
    Write-Output "--- 1. Generating build files... ---"
    cmake -S . -B build -G "MinGW Makefiles"
    
    Write-Output "--- 2. Compiling the project... ---"
    cmake --build build

} *>&1 | Tee-Object -FilePath full_build_log.txt

Write-Host "Build process finished. Please check full_build_log.txt for details."
