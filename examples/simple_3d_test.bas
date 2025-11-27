REM Simple 3D Test - Diagnose the 3D camera issue
INITWINDOW(800, 600, "Simple 3D Test")
SETTARGETFPS(60)

PRINT "Diagnosing 3D camera issue..."

WHILE NOT WINDOWSHOULDCLOSE()
    BEGINDRAWING()
    CLEARBACKGROUND(10, 10, 30)
    
    REM The problem: BEGINMODE3D(0) uses ::BeginMode3D(::Camera3D{})
    REM This creates a camera at (0,0,0) looking at (0,0,0) with FOV=0
    REM This is an invalid camera setup!
    
    BEGINMODE3D(0)
    
    REM Try objects at many different positions to see if ANY are visible
    REM Objects right at camera position (0,0,0)
    DRAWCUBE(0, 0, 0, 1, 1, 1, 255, 0, 0)
    
    REM Objects slightly offset from camera
    DRAWCUBE(0.1, 0, 0, 1, 1, 1, 0, 255, 0)
    DRAWCUBE(0, 0.1, 0, 1, 1, 1, 0, 0, 255)
    DRAWCUBE(0, 0, 0.1, 1, 1, 1, 255, 255, 0)
    
    REM Objects at various distances
    DRAWCUBE(1, 0, 0, 2, 2, 2, 255, 100, 100)
    DRAWCUBE(0, 1, 0, 2, 2, 2, 100, 255, 100)
    DRAWCUBE(0, 0, 1, 2, 2, 2, 100, 100, 255)
    
    REM Objects further away
    DRAWCUBE(5, 0, 0, 3, 3, 3, 200, 0, 0)
    DRAWCUBE(0, 5, 0, 3, 3, 3, 0, 200, 0)
    DRAWCUBE(0, 0, 5, 3, 3, 3, 0, 0, 200)
    
    REM Objects very far away
    DRAWCUBE(20, 0, 0, 5, 5, 5, 150, 0, 0)
    DRAWCUBE(0, 20, 0, 5, 5, 5, 0, 150, 0)
    DRAWCUBE(0, 0, 20, 5, 5, 5, 0, 0, 150)
    
    REM Try negative positions too
    DRAWCUBE(-1, 0, 0, 2, 2, 2, 255, 150, 150)
    DRAWCUBE(0, -1, 0, 2, 2, 2, 150, 255, 150)
    DRAWCUBE(0, 0, -1, 2, 2, 2, 150, 150, 255)
    
    ENDMODE3D()
    
    REM Draw diagnostic info
    DRAWTEXT("3D Camera Diagnosis", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("PROBLEM: BEGINMODE3D uses invalid camera", 10, 40, 16, 255, 100, 100)
    DRAWTEXT("Camera position: (0,0,0)", 10, 70, 16, 200, 200, 200)
    DRAWTEXT("Camera target: (0,0,0)", 10, 90, 16, 200, 200, 200)
    DRAWTEXT("Field of view: 0 degrees", 10, 110, 16, 200, 200, 200)
    DRAWTEXT("This makes 3D objects invisible!", 10, 130, 16, 255, 100, 100)
    DRAWTEXT("", 10, 160, 16, 200, 200, 200)
    DRAWTEXT("SOLUTION: Fix BEGINMODE3D to use proper camera", 10, 180, 16, 100, 255, 100)
    DRAWTEXT("Press ESC to exit", 10, 210, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "3D diagnosis completed!"
PRINT "The BEGINMODE3D function needs to be fixed to use a proper camera setup."
