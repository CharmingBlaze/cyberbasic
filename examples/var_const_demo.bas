REM VAR and CONST demo
CONST PI = 3.14159
VAR radius = 5
VAR area = PI * radius * radius
PRINT "Radius=" + radius
PRINT "Area=" + area

REM Attempting to reassign a CONST should fail
REM Uncomment to test error: Assignment to constant
REM PI = 3

REM Indexed assign with VAR sugar
VAR arr[0] = 10
VAR arr[1] = 20
PRINT "arr[0]=" + arr[0] + ", arr[1]=" + arr[1]
