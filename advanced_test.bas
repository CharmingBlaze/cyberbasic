PRINT "=== Advanced BASIC Features Test ==="

' Test control flow
LET x = 5
IF x > 3 THEN
    PRINT "IF statement works"
ENDIF

' Test loops
LET i = 1
WHILE i <= 3
    PRINT "Loop iteration"
    LET i = i + 1
WEND

' Test functions
LET result = RANDOM(100)
PRINT "Random number generated"

' Test string functions
LET text = "Hello World"
LET length = LEN(text)
PRINT "String length calculated"

' Test file operations
LET exists = FILEEXISTS("test.bas")
PRINT "File existence checked"

' Test Raylib graphics functions
PRINT "Testing Raylib graphics..."
INITWINDOW(800, 600, "Advanced Test")
LET width = 800
LET height = 600
PRINT "Window dimensions set"
CLOSEWINDOW()

PRINT "=== Advanced test completed! ==="
