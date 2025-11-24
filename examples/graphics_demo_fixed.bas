REM Graphics Demo
REM Demonstrates basic graphics functionality with shapes and text

INITWINDOW(800, 600, "Graphics Demo")
SETTARGETFPS(60)

PRINT "Window created successfully!"

REM Simple graphics demo
WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw some basic shapes
    DRAWCIRCLE(400, 300, 50, 255, 0, 0, 255)
    DRAWRECTANGLE(100, 100, 100, 100, 0, 255, 0, 255)
    DRAWTEXT("Graphics Demo", 10, 10, 20, 255, 255, 255, 255)
    
    ENDDRAWING()
    
    REM Exit on ESC
    IF ISKEYPRESSED(KEY_ESCAPE) THEN
        EXIT
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Graphics demo completed!"
