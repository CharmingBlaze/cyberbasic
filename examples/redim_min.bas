REM Redim Min - CyberBasic Example
REM This example demonstrates redim min functionality
REM Use WASD or arrow keys to interact (where applicable)

DIM a(3)
a[0] = 1
a[1] = 2
a[2] = 3
REDIM PRESERVE a(5)
PRINT STR(a[0]) + "," + STR(a[1]) + "," + STR(a[2]) + "," + STR(a[3]) + "," + STR(a[4])
