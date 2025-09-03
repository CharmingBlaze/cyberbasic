REM Demonstration of optional LOCAL/GLOBAL and OPTION EXPLICIT
OPTION EXPLICIT

REM Global variables
GLOBAL score, lives

REM Declare globals at top-level when strict
LET score = 0
LET lives = 3

SUB AddPoints(p)
  REM Local temp shadows any outer with same name
  LOCAL temp
  LET temp = p
  REM Mark score as global so we can modify the root binding
  GLOBAL score
  LET score = score + temp
ENDSUB

FUNCTION UseLife()
  GLOBAL lives
  IF lives > 0 THEN
    LET lives = lives - 1
    RETURN TRUE
  ELSE
    RETURN FALSE
  ENDIF
ENDFUNCTION

CALL AddPoints(10)
PRINT score

IF UseLife() THEN PRINT "Life used" ELSE PRINT "No lives left"
PRINT lives
