REM Massive 3D Demo - Many objects to test 3D visibility
REM Working around the broken BEGINMODE3D camera setup

INITWINDOW(800, 600, "Massive 3D Demo")
SETTARGETFPS(60)

PRINT "Testing 3D with many objects at different positions..."

VAR time = 0.0

WHILE NOT WINDOWSHOULDCLOSE()
    time = time + 0.02
    
    BEGINDRAWING()
    CLEARBACKGROUND(10, 10, 30)
    
    REM Try 3D mode - even though camera is broken, maybe some objects will be visible
    BEGINMODE3D(0)
    
    REM Create a massive field of objects at many different positions
    REM Try objects very close to origin (where broken camera might be)
    
    REM Tiny objects right at origin
    DRAWCUBE(0, 0, 0, 0.1, 0.1, 0.1, 255, 255, 255)
    DRAWCUBE(0.1, 0, 0, 0.1, 0.1, 0.1, 255, 0, 0)
    DRAWCUBE(0, 0.1, 0, 0.1, 0.1, 0.1, 0, 255, 0)
    DRAWCUBE(0, 0, 0.1, 0.1, 0.1, 0.1, 0, 0, 255)
    
    REM Small objects near origin
    FOR i = -2 TO 2
        FOR j = -2 TO 2
            FOR k = -2 TO 2
                IF i <> 0 OR j <> 0 OR k <> 0 THEN
                    VAR r = 100 + i * 50
                    VAR g = 100 + j * 50  
                    VAR b = 100 + k * 50
                    IF r > 255 THEN r = 255 ENDIF
                    IF g > 255 THEN g = 255 ENDIF
                    IF b > 255 THEN b = 255 ENDIF
                    IF r < 0 THEN r = 0 ENDIF
                    IF g < 0 THEN g = 0 ENDIF
                    IF b < 0 THEN b = 0 ENDIF
                    DRAWCUBE(i * 0.5, j * 0.5, k * 0.5, 0.3, 0.3, 0.3, r, g, b)
                ENDIF
            NEXT
        NEXT
    NEXT
    
    REM Medium objects further out
    FOR i = -5 TO 5 STEP 2
        FOR j = -5 TO 5 STEP 2
            VAR x = i + SIN(time + i) * 0.5
            VAR y = j + COS(time + j) * 0.5
            VAR z = SIN(time * 2 + i + j) * 2
            DRAWCUBE(x, y, z, 1, 1, 1, 150, 100, 200)
        NEXT
    NEXT
    
    REM Large objects at various distances
    DRAWCUBE(10, 0, 0, 3, 3, 3, 255, 100, 100)
    DRAWCUBE(-10, 0, 0, 3, 3, 3, 100, 255, 100)
    DRAWCUBE(0, 10, 0, 3, 3, 3, 100, 100, 255)
    DRAWCUBE(0, -10, 0, 3, 3, 3, 255, 255, 100)
    DRAWCUBE(0, 0, 10, 3, 3, 3, 255, 100, 255)
    DRAWCUBE(0, 0, -10, 3, 3, 3, 100, 255, 255)
    
    REM Objects at extreme distances
    DRAWCUBE(50, 0, 0, 10, 10, 10, 255, 0, 0)
    DRAWCUBE(-50, 0, 0, 10, 10, 10, 0, 255, 0)
    DRAWCUBE(0, 0, 50, 10, 10, 10, 0, 0, 255)
    DRAWCUBE(0, 0, -50, 10, 10, 10, 255, 255, 0)
    
    REM Draw coordinate system at multiple scales
    REM Tiny axes
    DRAWLINE3D(0, 0, 0, 0.1, 0, 0, 255, 0, 0)
    DRAWLINE3D(0, 0, 0, 0, 0.1, 0, 0, 255, 0)
    DRAWLINE3D(0, 0, 0, 0, 0, 0.1, 0, 0, 255)
    
    REM Small axes  
    DRAWLINE3D(0, 0, 0, 1, 0, 0, 255, 100, 100)
    DRAWLINE3D(0, 0, 0, 0, 1, 0, 100, 255, 100)
    DRAWLINE3D(0, 0, 0, 0, 0, 1, 100, 100, 255)
    
    REM Large axes
    DRAWLINE3D(0, 0, 0, 10, 0, 0, 200, 0, 0)
    DRAWLINE3D(0, 0, 0, 0, 10, 0, 0, 200, 0)
    DRAWLINE3D(0, 0, 0, 0, 0, 10, 0, 0, 200)
    
    ENDMODE3D()
    
    REM Draw UI
    DRAWTEXT("Massive 3D Demo - Testing Visibility", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Objects at many positions and scales", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("If ANY 3D objects are visible, 3D works!", 10, 70, 16, 200, 200, 200)
    DRAWTEXT("Time: " + STR(INT(time * 10)), 10, 100, 16, 150, 150, 150)
    DRAWTEXT("PROBLEM: BEGINMODE3D uses broken camera", 10, 130, 16, 255, 100, 100)
    DRAWTEXT("Camera at (0,0,0) looking at (0,0,0) = invalid!", 10, 150, 16, 255, 100, 100)
    DRAWTEXT("Press ESC to exit", 10, 180, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Massive 3D demo completed!"
PRINT "If you saw no 3D objects, the BEGINMODE3D function needs to be fixed!"
