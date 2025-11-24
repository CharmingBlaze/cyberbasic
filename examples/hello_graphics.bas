REM Hello Graphics - Simple animated rectangle demo
INITWINDOW(800, 450, "BASIC + Raylib")
SETTARGETFPS(60)

VAR x = 10
VAR dir = 2

WHILE NOT WINDOWSHOULDCLOSE()
    x = x + dir
    IF x > 700 THEN dir = -2
    IF x < 10 THEN dir = 2
    
    BEGINDRAWING()
    CLEARBACKGROUND(0, 121, 241)
    DRAWRECTANGLE(x, 200, 100, 50, 255, 161, 0)
    DRAWTEXT("Hello", 10, 10, 20, 255, 255, 255, 255)
    ENDDRAWING()
WEND

CLOSEWINDOW()
