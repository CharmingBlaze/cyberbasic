REM Basic 3D Test - Minimal 3D rendering test
INITWINDOW(800, 600, "Basic 3D Test")
SETTARGETFPS(60)

PRINT "Testing basic 3D rendering..."

WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Test if 3D mode works at all
    BEGINMODE3D(0)
    
    REM Draw a simple cube right in front of the default camera
    REM Default camera is at (0,0,10) looking at (0,0,0)
    REM So put cube at origin where camera is looking
    DRAWCUBE(0, 0, 0, 4, 4, 4, 255, 0, 0)
    
    REM Try a cube closer to camera
    DRAWCUBE(0, 0, 5, 2, 2, 2, 0, 255, 0)
    
    REM Try a cube further away
    DRAWCUBE(0, 0, -5, 2, 2, 2, 0, 0, 255)
    
    ENDMODE3D()
    
    REM Draw UI to confirm the program is running
    DRAWTEXT("Basic 3D Test", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("If 3D works, you should see colored cubes", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Red cube at origin (0,0,0)", 10, 70, 16, 255, 100, 100)
    DRAWTEXT("Green cube closer (0,0,5)", 10, 90, 16, 100, 255, 100)
    DRAWTEXT("Blue cube further (0,0,-5)", 10, 110, 16, 100, 100, 255)
    DRAWTEXT("Press ESC to exit", 10, 140, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Basic 3D test completed!"
