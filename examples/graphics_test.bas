REM Graphics Test
REM Simple animated rectangle demo using Raylib

INITWINDOW(800, 600, "Graphics Test")
SETTARGETFPS(60)

VAR x = 100
VAR y = 100
VAR dir = 1

WHILE NOT WINDOWSHOULDCLOSE()
    x = x + dir
    IF x > 700 THEN dir = -1
    IF x < 100 THEN dir = 1
    
    BEGINDRAWING()
    CLEARBACKGROUND(0, 121, 241)
    DRAWRECTANGLE(x, y, 100, 50, 255, 161, 0, 255)
    DRAWTEXT("Hello Raylib!", 10, 10, 20, 255, 255, 255)
    ENDDRAWING()
WEND

PRINT "Graphics test completed!"
