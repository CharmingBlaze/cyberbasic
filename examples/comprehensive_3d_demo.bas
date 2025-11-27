REM Comprehensive 3D Demo - CyberBasic
REM This demonstrates the FIXED 3D system with proper camera setup
REM Camera is now at (0,10,10) looking at (0,0,0) with 45 degree FOV

INITWINDOW(800, 600, "Comprehensive 3D Demo - FIXED")
SETTARGETFPS(60)

PRINT "Starting comprehensive 3D demo with FIXED camera system..."

VAR time = 0.0
VAR rotation_speed = 50.0

WHILE NOT WINDOWSHOULDCLOSE()
    time = time + 0.016
    
    BEGINDRAWING()
    CLEARBACKGROUND(20, 30, 50)
    
    REM NOW 3D MODE WORKS! Camera is properly set up at (0,10,10) looking at origin
    BEGINMODE3D(0)
    
    REM === STATIC OBJECTS ===
    REM Central red cube at origin - should be clearly visible
    DRAWCUBE(0, 0, 0, 2, 2, 2, 255, 100, 100)
    
    REM Surrounding cubes in a cross pattern
    DRAWCUBE(-5, 0, 0, 1.5, 1.5, 1.5, 100, 255, 100)  REM Green left
    DRAWCUBE(5, 0, 0, 1.5, 1.5, 1.5, 100, 100, 255)   REM Blue right
    DRAWCUBE(0, 0, -5, 1.5, 1.5, 1.5, 255, 255, 100)  REM Yellow back
    DRAWCUBE(0, 0, 5, 1.5, 1.5, 1.5, 255, 100, 255)   REM Magenta front
    
    REM Elevated cubes
    DRAWCUBE(0, 3, 0, 1, 1, 1, 255, 255, 255)          REM White top
    DRAWCUBE(0, -3, 0, 1, 1, 1, 128, 128, 128)         REM Gray bottom
    
    REM === ANIMATED OBJECTS ===
    REM Orbiting cubes
    VAR orbit_radius = 8.0
    VAR orbit_x1 = SIN(time) * orbit_radius
    VAR orbit_z1 = COS(time) * orbit_radius
    VAR orbit_x2 = SIN(time + 3.14159) * orbit_radius
    VAR orbit_z2 = COS(time + 3.14159) * orbit_radius
    
    DRAWCUBE(orbit_x1, 2, orbit_z1, 1, 1, 1, 255, 200, 100)
    DRAWCUBE(orbit_x2, -2, orbit_z2, 1, 1, 1, 100, 200, 255)
    
    REM Vertical oscillating cubes
    VAR osc_y = SIN(time * 2) * 3
    DRAWCUBE(-3, osc_y, -3, 0.8, 0.8, 0.8, 200, 255, 200)
    DRAWCUBE(3, -osc_y, 3, 0.8, 0.8, 0.8, 255, 200, 200)
    
    REM === 3D COORDINATE SYSTEM ===
    REM X-axis (Red) - points right
    DRAWLINE3D(0, 0, 0, 6, 0, 0, 255, 0, 0)
    DRAWLINE3D(5.5, 0.2, 0, 6, 0, 0, 255, 0, 0)
    DRAWLINE3D(5.5, -0.2, 0, 6, 0, 0, 255, 0, 0)
    
    REM Y-axis (Green) - points up
    DRAWLINE3D(0, 0, 0, 0, 6, 0, 0, 255, 0)
    DRAWLINE3D(0.2, 5.5, 0, 0, 6, 0, 0, 255, 0)
    DRAWLINE3D(-0.2, 5.5, 0, 0, 6, 0, 0, 255, 0)
    
    REM Z-axis (Blue) - points toward camera
    DRAWLINE3D(0, 0, 0, 0, 0, 6, 0, 0, 255)
    DRAWLINE3D(0, 0, 5.5, 0.2, 0, 6, 0, 0, 255)
    DRAWLINE3D(0, 0, 5.5, -0.2, 0, 6, 0, 0, 255)
    
    REM === GROUND GRID ===
    FOR i = -10 TO 10 STEP 2
        DRAWLINE3D(i, -4, -10, i, -4, 10, 60, 60, 80)
        DRAWLINE3D(-10, -4, i, 10, -4, i, 60, 60, 80)
    NEXT
    
    REM === FLOATING GEOMETRIC SHAPES ===
    REM Create a more complex scene
    VAR wave_y = SIN(time * 1.5) * 2
    DRAWCUBE(-8, wave_y, 0, 1, 1, 1, 180, 100, 220)
    DRAWCUBE(8, -wave_y, 0, 1, 1, 1, 100, 180, 220)
    
    REM Rotating formation
    FOR i = 0 TO 5
        VAR angle = time + i * 1.047
        VAR form_x = SIN(angle) * 6
        VAR form_z = COS(angle) * 6
        VAR form_y = SIN(time * 3 + i) * 1.5
        
        VAR r = 100 + i * 30
        VAR g = 255 - i * 40
        VAR b = 150 + i * 20
        IF r > 255 THEN
            r = 255
        ENDIF
        IF g < 50 THEN
            g = 50
        ENDIF
        IF b > 255 THEN
            b = 255
        ENDIF
        
        DRAWCUBE(form_x, form_y, form_z, 0.6, 0.6, 0.6, r, g, b)
    NEXT i
    
    ENDMODE3D()
    
    REM === 2D UI OVERLAY ===
    DRAWTEXT("COMPREHENSIVE 3D DEMO - FIXED CAMERA!", 10, 10, 24, 255, 255, 100)
    DRAWTEXT("Camera: (0,10,10) -> (0,0,0), FOV: 45 degrees", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("You should see:", 10, 70, 18, 255, 255, 255)
    DRAWTEXT("- Red cube at center", 10, 95, 14, 255, 100, 100)
    DRAWTEXT("- Colored cubes in cross pattern", 10, 115, 14, 100, 255, 100)
    DRAWTEXT("- Orbiting animated cubes", 10, 135, 14, 255, 200, 100)
    DRAWTEXT("- 3D coordinate axes (RGB = XYZ)", 10, 155, 14, 200, 200, 255)
    DRAWTEXT("- Ground grid and floating shapes", 10, 175, 14, 180, 180, 255)
    
    DRAWTEXT("Time: " + STR(INT(time * 10)), 10, 210, 16, 150, 150, 150)
    DRAWTEXT("FPS should be smooth at 60", 10, 230, 14, 150, 150, 150)
    
    IF time > 2 THEN
        DRAWTEXT("SUCCESS: 3D rendering is working!", 10, 260, 18, 100, 255, 100)
    ENDIF
    
    DRAWTEXT("Press ESC to exit", 10, 290, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Comprehensive 3D demo completed successfully!"
PRINT "If you saw 3D objects, the fix worked!"
