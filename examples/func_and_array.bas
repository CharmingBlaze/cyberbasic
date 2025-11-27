REM Func And Array - CyberBasic Example
REM This example demonstrates func and array functionality
REM Use WASD or arrow keys to interact (where applicable)

# Test FUNCTION return and arrays
FUNCTION Add(a, b)
  RETURN a + b
ENDFUNCTION

LET nums = [1, 2, 3]
LET x = Add(10, 20)
PRINT x
PRINT nums[1]
LET nums[5] = 42
PRINT nums[5]
