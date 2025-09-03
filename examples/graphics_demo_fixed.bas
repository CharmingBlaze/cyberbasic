REM Graphics System Demo
REM Demonstrates basic graphics functionality

PRINT "Initializing Graphics System..."
INITGRAPHICS()

PRINT "Creating basic graphics demo..."

REM Initialize window
INITWINDOW(800, 600, "Graphics Demo")

PRINT "Window created successfully!"

REM Simple graphics demo
WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw some basic shapes
    DRAWCIRCLE(400, 300, 50, 255, 0, 0)
    DRAWRECTANGLE(100, 100, 100, 100, 0, 255, 0)
    DRAWTEXT("Graphics Demo", 10, 10, 20, 255, 255, 255)
    
    ENDDRAW()
    
    REM Exit on ESC
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Graphics demo completed!"
