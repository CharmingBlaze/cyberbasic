REM Test raylib graphics functionality
PRINT "Testing raylib graphics..."

INITWINDOW(800, 600, "Graphics Test")
SETTARGETFPS(60)

LET x = 100
LET y = 100
LET dir = 1

WHILE NOT WINDOWSHOULDCLOSE()
LET x = x + dir
IF x > 700 THEN LET dir = -1
IF x < 100 THEN LET dir = 1

BEGINDRAW()
CLEARBACKGROUND(0, 121, 241)
DRAWRECTANGLE(x, y, 100, 50, 255, 161, 0)
DRAWTEXT("Hello Raylib!", 10, 10, 20, 255, 255, 255, 255)
ENDDRAW()
WEND

PRINT "Graphics test completed!"
