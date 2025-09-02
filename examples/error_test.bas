REM This program has intentional errors to test the enhanced error system

LET x = 10
LET y = 20

IF x > y THEN
    PRINT "x is greater"
    REM Missing ENDIF

WHILE x > 0
    PRINT x
    LET x = x - 1
    REM Missing WEND

LET array = ARRAY(5)
LET array[0] = 10
LET array[1] = 20

REM Test function call
CALL MySub(x, y)

REM Test undefined variable
PRINT undefined_variable

REM Test malformed DIM
DIM bad_array

REM Test malformed IF
IF x > 5
    PRINT "x is greater than 5"
ENDIF
