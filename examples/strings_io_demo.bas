REM Demo: INSTR/TRIM and READALL/WRITEALL
PRINT "-- Strings & IO Demo --"
LET text = "  hello world  "
PRINT "orig='" + text + "'"
PRINT "LTRIM='" + LTRIM(text) + "'"
PRINT "RTRIM='" + RTRIM(text) + "'"
PRINT "TRIM='" + TRIM(text) + "'"
PRINT "INSTR(TRIM(text), 'world') = " + STR(INSTR(TRIM(text), "world"))

LET ok = WRITEALL("demo.txt", TRIM(text))
PRINT "WRITEALL ok? " + STR(ok)
LET readBack = READALL("demo.txt")
PRINT "READALL='" + readBack + "'"
PRINT "INSTR(readBack, 'hello') = " + STR(INSTR(readBack, "hello"))
# End
