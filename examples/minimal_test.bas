REM Minimal Test - Basic window and drawing demo
INITWINDOW(800, 600, "Minimal Test")
SETTARGETFPS(60)

VAR x = 100
VAR y = 100
VAR running = TRUE

WHILE running AND NOT WINDOWSHOULDCLOSE()
    BEGINDRAWING()
    CLEARBACKGROUND(50, 50, 100)
    DRAWTEXT("Hello World", x, y, 30, 255, 255, 255)
    DRAWRECTANGLE(x, y + 50, 100, 50, 255, 0, 0)
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN running = FALSE
WEND

CLOSEWINDOW()
