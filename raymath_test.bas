REM Test raymath functionality
PRINT "Testing raymath functions..."

LET result = LERP(0, 10, 0.5)
PRINT "LERP(0, 10, 0.5) ="
PRINT result

LET min_val = MIN(5, 10)
PRINT "MIN(5, 10) ="
PRINT min_val

LET max_val = MAX(5, 10)
PRINT "MAX(5, 10) ="
PRINT max_val

LET clamped = CLAMP(15, 0, 10)
PRINT "CLAMP(15, 0, 10) ="
PRINT clamped

PRINT "Raymath test completed!"
