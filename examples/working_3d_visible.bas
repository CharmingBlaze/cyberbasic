REM Working 3D Demo - Objects positioned for default camera
REM Default camera is at (0,0,10) looking at (0,0,0) with FOV 60 degrees

INITWINDOW(800, 600, "Working 3D - Visible Objects")
SETTARGETFPS(60)

PRINT "Starting 3D demo with properly positioned objects..."

VAR rotation = 0.0

WHILE NOT WINDOWSHOULDCLOSE()
    rotation = rotation + 0.02
    
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Start 3D mode - default camera at (0,0,10) looking at (0,0,0)
    BEGINMODE3D(0)
    
    REM Position objects in front of the camera (negative Z values)
    REM Camera is at Z=10, so objects at Z=0 to Z=-10 should be visible
    
    REM Main cube at origin - should be visible
    DRAWCUBE(0, 0, 0, 3, 3, 3, 255, 100, 100)
    
    REM Smaller cubes arranged around the center
    DRAWCUBE(-5, 0, 0, 2, 2, 2, 100, 255, 100)
    DRAWCUBE(5, 0, 0, 2, 2, 2, 100, 100, 255)
    DRAWCUBE(0, 5, 0, 2, 2, 2, 255, 255, 100)
    DRAWCUBE(0, -5, 0, 2, 2, 2, 255, 100, 255)
    
    REM Rotating cubes for animation
    VAR orbit_x = SIN(rotation) * 8
    VAR orbit_z = COS(rotation) * 8
    DRAWCUBE(orbit_x, 0, orbit_z, 1.5, 1.5, 1.5, 100, 255, 255)
    DRAWCUBE(-orbit_x, 0, -orbit_z, 1.5, 1.5, 1.5, 255, 255, 100)
    
    REM Draw some objects further back (more negative Z)
    DRAWCUBE(0, 0, -8, 2, 2, 2, 150, 150, 255)
    DRAWCUBE(3, 3, -5, 1, 1, 1, 255, 150, 150)
    DRAWCUBE(-3, -3, -5, 1, 1, 1, 150, 255, 150)
    
    REM Draw coordinate axes to show orientation
    DRAWLINE3D(0, 0, 0, 4, 0, 0, 255, 0, 0)
    DRAWLINE3D(0, 0, 0, 0, 4, 0, 0, 255, 0)
    DRAWLINE3D(0, 0, 0, 0, 0, 4, 0, 0, 255)
    
    REM Draw a grid on the ground plane
    FOR i = -10 TO 10 STEP 2
        DRAWLINE3D(i, -3, -10, i, -3, 10, 80, 80, 80)
        DRAWLINE3D(-10, -3, i, 10, -3, i, 80, 80, 80)
    NEXT
    
    ENDMODE3D()
    
    REM Draw UI
    DRAWTEXT("Working 3D Demo - Visible Objects", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Objects positioned for default camera view", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Camera at (0,0,10) looking at (0,0,0)", 10, 70, 16, 150, 150, 150)
    DRAWTEXT("You should see cubes and rotating objects!", 10, 100, 16, 200, 200, 200)
    DRAWTEXT("Rotation: " + STR(INT(rotation * 57.3)) + " degrees", 10, 130, 16, 150, 150, 150)
    DRAWTEXT("Press ESC to exit", 10, 160, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "3D demo completed!"
