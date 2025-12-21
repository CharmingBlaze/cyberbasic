REM Spinning 3D Cube Demo
REM A simple demonstration of 3D graphics in CyberBasic

INITWINDOW(800, 600, "Spinning 3D Cube")
SETTARGETFPS(60)

REM Rotation variables
VAR rotationX = 0.0
VAR rotationY = 0.0
VAR rotationSpeed = 1.5

WHILE NOT WINDOWSHOULDCLOSE()
    REM Update rotation
    rotationX = rotationX + rotationSpeed
    rotationY = rotationY + rotationSpeed * 0.7
    
    REM Draw
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 30)
    
    REM Begin 3D mode (uses default camera at 0,10,10 looking at 0,0,0)
    BEGINMODE3D(0)
    
        REM Draw grid on the ground
        DRAWCUBE(0, -0.1, 0, 20, 0.1, 20, 50, 50, 50)
        
        REM Draw rotating cube (red)
        REM Note: Rotation is done by calculating positions, as ROTATE may not be available
        VAR cubeX = 0
        VAR cubeY = 0
        VAR cubeZ = 0
        DRAWCUBE(cubeX, cubeY, cubeZ, 2, 2, 2, 200, 50, 50)
        
        REM Draw some reference cubes around it
        DRAWCUBE(4, 0, 0, 0.5, 0.5, 0.5, 100, 200, 100)   REM Green
        DRAWCUBE(-4, 0, 0, 0.5, 0.5, 0.5, 100, 100, 200)  REM Blue
        DRAWCUBE(0, 0, 4, 0.5, 0.5, 0.5, 200, 200, 100)   REM Yellow
        DRAWCUBE(0, 0, -4, 0.5, 0.5, 0.5, 200, 100, 200) REM Magenta
        
        REM Draw orbiting cubes to show rotation effect
        VAR orbitRadius = 3.0
        VAR orbitX1 = SIN(rotationX * 0.0174533) * orbitRadius
        VAR orbitZ1 = COS(rotationX * 0.0174533) * orbitRadius
        VAR orbitX2 = SIN(rotationY * 0.0174533) * orbitRadius
        VAR orbitZ2 = COS(rotationY * 0.0174533) * orbitRadius
        
        DRAWCUBE(orbitX1, 2, orbitZ1, 1, 1, 1, 255, 200, 100)
        DRAWCUBE(orbitX2, -2, orbitZ2, 1, 1, 1, 100, 200, 255)
        
    ENDMODE3D()
    
    REM Draw UI overlay
    DRAWTEXT("Spinning 3D Cube", 10, 10, 30, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 45, 20, 200, 200, 200)
    DRAWTEXT("Rotation X: " + STR(INT(rotationX)) + "°", 10, 70, 18, 200, 200, 200)
    DRAWTEXT("Rotation Y: " + STR(INT(rotationY)) + "°", 10, 95, 18, 200, 200, 200)
    DRAWTEXT("Press ESC or close window to exit", 10, 570, 18, 150, 150, 150)
    
    ENDDRAWING()
WEND

CLOSEWINDOW()
