REM Fixed 3D Demo - CyberBasic Example
REM This works with the current 3D implementation

INITWINDOW(800, 600, "Fixed 3D Demo")
SETTARGETFPS(60)

PRINT "Starting fixed 3D demo..."

VAR rotation = 0.0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 0.02
    
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Start 3D mode with default camera
    REM The default camera is at (0,0,10) looking at (0,0,0)
    BEGINMODE3D(0)
    
    REM Draw 3D objects at positions visible from default camera
    REM Main cube at origin
    DRAWCUBE(0, 0, 0, 2, 2, 2, 255, 100, 100)
    
    REM Smaller cubes around it
    DRAWCUBE(SIN(rotation) * 4, 0, COS(rotation) * 4, 1, 1, 1, 100, 255, 100)
    DRAWCUBE(-SIN(rotation) * 4, 0, -COS(rotation) * 4, 1, 1, 1, 100, 100, 255)
    
    REM Vertical cubes
    DRAWCUBE(0, 3, 0, 1, 1, 1, 255, 255, 100)
    DRAWCUBE(0, -3, 0, 1, 1, 1, 255, 100, 255)
    
    REM Draw coordinate axes for reference
    DRAWLINE3D(0, 0, 0, 3, 0, 0, 255, 0, 0)
    DRAWLINE3D(0, 0, 0, 0, 3, 0, 0, 255, 0)
    DRAWLINE3D(0, 0, 0, 0, 0, 3, 0, 0, 255)
    
    REM Draw a simple grid
    FOR i = -5 TO 5
        DRAWLINE3D(i, -2, -5, i, -2, 5, 60, 60, 60)
        DRAWLINE3D(-5, -2, i, 5, -2, i, 60, 60, 60)
    NEXT
    
    ENDMODE3D()
    
    REM Draw UI
    DRAWTEXT("Fixed 3D Demo", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("You should see 3D cubes and rotating objects!", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Default camera at (0,0,10) looking at origin", 10, 70, 16, 150, 150, 150)
    DRAWTEXT("Rotation: " + STR(INT(rotation * 57.3)), 10, 100, 16, 150, 150, 150)
    DRAWTEXT("Mouse X: " + STR(MOUSEX()), 10, 130, 16, 100, 100, 100)
    DRAWTEXT("Mouse Y: " + STR(MOUSEY()), 10, 150, 16, 100, 100, 100)
    
    IF ISMOUSEBUTTONDOWN(1) THEN
        DRAWTEXT("Left mouse pressed!", 10, 180, 16, 255, 255, 0)
    ENDIF
    
    DRAWTEXT("Press ESC to exit", 10, 210, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Fixed 3D demo completed!"
