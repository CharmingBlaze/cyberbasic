REM Comprehensive VAR test - should work everywhere
VAR globalVar = 100

IF TRUE THEN
    VAR ifVar = 10
    PRINT ifVar
ENDIF

WHILE globalVar > 0
    VAR whileVar = globalVar
    PRINT whileVar
    globalVar = globalVar - 1
WEND

FOR i = 1 TO 5
    VAR forVar = i * 2
    PRINT forVar
NEXT

PRINT "All VAR tests passed!"

