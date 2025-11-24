REM Raymath Functionality Test
REM Demonstrates basic raymath utility functions

PRINT "Testing raymath functions..."

VAR result = LERP(0, 10, 0.5)
PRINT "LERP(0, 10, 0.5) = " + STR(result)

VAR min_val = MIN(5, 10)
PRINT "MIN(5, 10) = " + STR(min_val)

VAR max_val = MAX(5, 10)
PRINT "MAX(5, 10) = " + STR(max_val)

VAR clamped = CLAMP(15, 0, 10)
PRINT "CLAMP(15, 0, 10) = " + STR(clamped)

PRINT "Raymath test completed!"
