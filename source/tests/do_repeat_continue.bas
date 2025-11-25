LET i = 0
' Trigger agklite auto-detect without producing output
IF 0 PRINT "noop"

' Simple DO...LOOP with BREAK
DO
  PRINT "D"
  BREAK
LOOP

' CONTINUE inside DO...LOOP
LET i = 0
DO
  LET i = i + 1
  IF i = 2 THEN CONTINUE
  PRINT i
  IF i >= 3 THEN BREAK
LOOP

' REPEAT...UNTIL loop
LET i = 0
REPEAT
  LET i = i + 1
UNTIL i = 3
PRINT i
