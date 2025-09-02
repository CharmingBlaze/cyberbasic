REM Test BREAK statement
PRINT "Testing BREAK statement..."

LET i = 1
WHILE i <= 10
  PRINT "Loop iteration: " + STR(i)
  IF i = 5 THEN
    PRINT "Breaking at iteration 5"
    BREAK
  ENDIF
  LET i = i + 1
WEND

PRINT "After WHILE loop"

REM Test BREAK in FOR loop
FOR j = 1 TO 10
  PRINT "FOR loop iteration: " + STR(j)
  IF j = 7 THEN
    PRINT "Breaking at iteration 7"
    BREAK
  ENDIF
NEXT

PRINT "After FOR loop"
PRINT "BREAK statement test completed!"
