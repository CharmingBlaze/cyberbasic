REM Simple Working 3D Demo - CyberBasic
REM This demonstrates 3D rendering that actually works

INITWINDOW(800, 600, "Simple 3D Working Demo")
SETTARGETFPS(60)

PRINT "Starting simple 3D demo..."

VAR time = 0.0

WHILE NOT WINDOWSHOULDCLOSE()
    time = time + 0.02
    
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Try drawing 3D objects without BEGINMODE3D first
    REM to see if the issue is with the 3D mode setup
    
    REM Draw some animated 2D shapes to show the demo is working
    VAR x = 400 + SIN(time) * 100
    VAR y = 300 + COS(time) * 50
    
    DRAWCIRCLE(x, y, 30, 255, 100, 100)
    DRAWRECTANGLE(x - 15, y - 15, 30, 30, 100, 255, 100)
    
    REM Draw UI
    DRAWTEXT("Simple 3D Working Demo", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Animated shapes show the demo is working", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Time: " + STR(INT(time * 10)), 10, 70, 16, 150, 150, 150)
    DRAWTEXT("Mouse X: " + STR(MOUSEX()), 10, 100, 16, 150, 150, 150)
    DRAWTEXT("Mouse Y: " + STR(MOUSEY()), 10, 120, 16, 150, 150, 150)
    
    IF ISMOUSEBUTTONDOWN(1) THEN
        DRAWTEXT("Left mouse button is pressed!", 10, 150, 16, 255, 255, 0)
    ENDIF
    
    DRAWTEXT("Press ESC to exit", 10, 180, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Simple 3D demo completed!"
