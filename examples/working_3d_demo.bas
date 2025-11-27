REM Interactive 3D Demo - CyberBasic Example
REM Fixed 3D camera with proper mouse orbital controls

INITWINDOW(800, 600, "Interactive 3D Demo")
SETTARGETFPS(60)

PRINT "Starting interactive 3D demo..."

REM Camera parameters for orbital control
VAR camera_distance = 15.0
VAR camera_angle_x = 0.0
VAR camera_angle_y = 0.0
VAR camera_target_x = 0.0
VAR camera_target_y = 0.0
VAR camera_target_z = 0.0

REM Mouse control variables
VAR last_mouse_x = MOUSEX()
VAR last_mouse_y = MOUSEY()
VAR mouse_sensitivity = 0.005

WHILE NOT WINDOWSHOULDCLOSE()
    REM Get current mouse position
    VAR mouse_x = MOUSEX()
    VAR mouse_y = MOUSEY()
    
    REM Handle mouse controls
    IF ISMOUSEBUTTONDOWN(1) THEN
        REM Left mouse button - orbit camera
        VAR delta_x = mouse_x - last_mouse_x
        VAR delta_y = mouse_y - last_mouse_y
        camera_angle_x = camera_angle_x + delta_x * mouse_sensitivity
        camera_angle_y = camera_angle_y + delta_y * mouse_sensitivity
        
        REM Clamp vertical angle to prevent flipping
        IF camera_angle_y > 1.4 THEN camera_angle_y = 1.4 ENDIF
        IF camera_angle_y < -1.4 THEN camera_angle_y = -1.4 ENDIF
    ENDIF
    
    IF ISMOUSEBUTTONDOWN(2) THEN
        REM Right mouse button - pan camera target
        VAR delta_x = mouse_x - last_mouse_x
        VAR delta_y = mouse_y - last_mouse_y
        camera_target_x = camera_target_x - delta_x * 0.02
        camera_target_y = camera_target_y + delta_y * 0.02
    ENDIF
    
    REM Mouse wheel zoom
    VAR wheel = GETMOUSEWHEELMOVE()
    camera_distance = camera_distance - wheel * 2.0
    IF camera_distance < 3.0 THEN camera_distance = 3.0 ENDIF
    IF camera_distance > 50.0 THEN camera_distance = 50.0 ENDIF
    
    REM Update last mouse position
    last_mouse_x = mouse_x
    last_mouse_y = mouse_y
    
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Calculate camera position using spherical coordinates
    VAR cam_x = camera_target_x + SIN(camera_angle_x) * COS(camera_angle_y) * camera_distance
    VAR cam_y = camera_target_y + SIN(camera_angle_y) * camera_distance
    VAR cam_z = camera_target_z + COS(camera_angle_x) * COS(camera_angle_y) * camera_distance
    
    REM Create and configure camera
    VAR camera = Camera3D()
    
    REM Set camera position (where the camera is)
    LET camera.position.x = cam_x
    LET camera.position.y = cam_y
    LET camera.position.z = cam_z
    
    REM Set camera target (what it's looking at) - fixed at center
    LET camera.target.x = 0
    LET camera.target.y = 0
    LET camera.target.z = 0
    
    REM Set camera up vector
    LET camera.up.x = 0
    LET camera.up.y = 1
    LET camera.up.z = 0
    
    REM Set field of view
    LET camera.fovy = 45
    
    REM Start 3D mode - objects stay fixed at center
    BEGINMODE3D(0)
    
    REM Draw 3D objects at fixed positions (not moving!)
    DRAWCUBE(0, 0, 0, 2, 2, 2, 255, 100, 100)
    DRAWCUBE(-4, 0, 0, 1.5, 1.5, 1.5, 100, 255, 100)
    DRAWCUBE(4, 0, 0, 1.5, 1.5, 1.5, 100, 100, 255)
    DRAWCUBE(0, 3, 0, 1, 1, 1, 255, 255, 100)
    DRAWCUBE(0, -3, 0, 1, 1, 1, 255, 100, 255)
    DRAWCUBE(0, 0, 4, 1, 1, 1, 100, 255, 255)
    
    REM Draw coordinate axes for reference (fixed)
    DRAWLINES(0, 0, 0, 5, 0, 0, 255, 0, 0)
    DRAWLINES(0, 0, 0, 0, 5, 0, 0, 255, 0)
    DRAWLINES(0, 0, 0, 0, 0, 5, 0, 0, 255)
    
    REM Draw a grid on the ground (fixed)
    FOR i = -10 TO 10
        DRAWLINES(i, -1, -10, i, -1, 10, 80, 80, 80)
        DRAWLINES(-10, -1, i, 10, -1, i, 80, 80, 80)
    NEXT
    
    ENDMODE3D()
    
    REM Draw UI
    DRAWTEXT("Interactive 3D Demo - Fixed Objects", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Left click + drag: Orbit camera around center", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Right click + drag: Pan camera target", 10, 60, 16, 200, 200, 200)
    DRAWTEXT("Mouse wheel: Zoom in/out", 10, 80, 16, 200, 200, 200)
    DRAWTEXT("Distance: " + STR(INT(camera_distance)), 10, 110, 16, 150, 150, 150)
    DRAWTEXT("Camera X: " + STR(INT(cam_x)), 10, 130, 16, 150, 150, 150)
    DRAWTEXT("Camera Y: " + STR(INT(cam_y)), 10, 150, 16, 150, 150, 150)
    DRAWTEXT("Camera Z: " + STR(INT(cam_z)), 10, 170, 16, 150, 150, 150)
    DRAWTEXT("Press ESC to exit", 10, 200, 16, 200, 200, 200)
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN
        BREAK
    ENDIF
WEND

CLOSEWINDOW()
PRINT "Interactive 3D demo completed!"
