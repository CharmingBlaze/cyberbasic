REM 3D Model Viewer - CyberBasic
REM Load and view 3D models with smooth mouse controls
REM
REM Controls:
REM   - Left Mouse Button + Drag: Orbit around the model
REM   - Mouse Wheel: Zoom in/out
REM   - W: Toggle wireframe mode
REM   - R: Reset camera position
REM   - L: Reload current model
REM   - ESC: Exit

INITWINDOW(1280, 720, "3D Model Viewer - CyberBasic")
SETTARGETFPS(60)

REM Camera setup - create camera looking at origin
VAR camera = CAMERA3D(0, 5, 10, 0, 0, 0, 0, 1, 0, 45)

REM Model state
VAR currentModel = -1
VAR modelLoaded = 0
VAR modelPath$ = "3Dmodels/object.obj"  REM Change this to load different models
VAR wireframeMode = 0  REM 0 = solid, 1 = wireframe

REM Camera orbit controls
VAR cameraDistance = 10.0
VAR cameraAngleX = 0.0
VAR cameraAngleY = 45.0
VAR minDistance = 2.0
VAR maxDistance = 50.0
VAR mouseSensitivity = 0.3  REM Reduced for smoother control
VAR zoomSpeed = 0.5

REM Mouse state
VAR lastMouseX = 0
VAR lastMouseY = 0
VAR isDragging = 0
VAR mouseButtonLeft = 1  REM ISMOUSEBUTTONDOWN: 1=LEFT, 2=RIGHT, 3=MIDDLE

REM Initial camera angles (stored for reset)
VAR initialAngleX = 0.0
VAR initialAngleY = 45.0
VAR initialDistance = 10.0

REM Load initial model
currentModel = LOADMODEL(modelPath$)
IF currentModel >= 0 THEN
    modelLoaded = 1
    PRINT "Loaded model: " + modelPath$
ELSE
    modelLoaded = 0
    PRINT "Failed to load model: " + modelPath$
    PRINT "Make sure the file exists: " + modelPath$
ENDIF

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle mouse input for orbiting
    VAR mouseX = GETMOUSEX()
    VAR mouseY = GETMOUSEY()
    
    REM Check if left mouse button is down
    IF ISMOUSEBUTTONDOWN(mouseButtonLeft) THEN
        IF isDragging = 0 THEN
            REM Just started dragging - initialize
            isDragging = 1
            lastMouseX = mouseX
            lastMouseY = mouseY
        ELSE
            REM Continue dragging - calculate delta and update camera
            VAR deltaX = mouseX - lastMouseX
            VAR deltaY = mouseY - lastMouseY
            
            REM Update camera angles (invert Y for natural feel)
            cameraAngleX = cameraAngleX + deltaX * mouseSensitivity
            cameraAngleY = cameraAngleY - deltaY * mouseSensitivity
            
            REM Clamp vertical angle to prevent flipping
            IF cameraAngleY > 89 THEN cameraAngleY = 89
            IF cameraAngleY < -89 THEN cameraAngleY = -89
            
            REM Update last mouse position
            lastMouseX = mouseX
            lastMouseY = mouseY
        ENDIF
    ELSE
        REM Mouse button not down - stop dragging
        IF isDragging = 1 THEN
            isDragging = 0
        ENDIF
    ENDIF
    
    REM Handle mouse wheel for zooming
    VAR wheelMove = GETMOUSEWHEELMOVE()
    IF wheelMove <> 0 THEN
        cameraDistance = cameraDistance - wheelMove * zoomSpeed
        IF cameraDistance < minDistance THEN cameraDistance = minDistance
        IF cameraDistance > maxDistance THEN cameraDistance = maxDistance
    ENDIF
    
    REM Calculate camera position based on orbit angles and distance
    REM Convert angles to radians
    VAR angleXRad = cameraAngleX * 3.14159 / 180.0
    VAR angleYRad = cameraAngleY * 3.14159 / 180.0
    
    REM Calculate camera position using spherical coordinates
    REM X rotation (horizontal): rotates around Y axis
    REM Y rotation (vertical): rotates around X axis
    VAR camX = cameraDistance * COS(angleYRad) * SIN(angleXRad)
    VAR camY = cameraDistance * SIN(angleYRad)
    VAR camZ = cameraDistance * COS(angleYRad) * COS(angleXRad)
    
    REM Update camera position
    camera = CAMERA3D_SETPOSITION(camera, camX, camY, camZ)
    
    REM Toggle wireframe mode (W key = 87)
    IF ISKEYPRESSED(87) THEN
        wireframeMode = 1 - wireframeMode
        IF wireframeMode = 1 THEN
            PRINT "Wireframe mode ON"
        ELSE
            PRINT "Wireframe mode OFF"
        ENDIF
    ENDIF
    
    REM Reset camera (R key = 82)
    IF ISKEYPRESSED(82) THEN
        cameraAngleX = initialAngleX
        cameraAngleY = initialAngleY
        cameraDistance = initialDistance
        PRINT "Camera reset"
    ENDIF
    
    REM Handle model loading
    REM Press L to reload current model (L key = 76)
    IF ISKEYPRESSED(76) THEN
        IF currentModel >= 0 THEN
            UNLOADMODEL(currentModel)
        ENDIF
        currentModel = LOADMODEL(modelPath$)
        IF currentModel >= 0 THEN
            modelLoaded = 1
            PRINT "Reloaded model: " + modelPath$
        ELSE
            modelLoaded = 0
            PRINT "Failed to reload model: " + modelPath$
        ENDIF
    ENDIF
    
    REM Begin drawing
    BEGINDRAWING()
    
    REM Clear background
    CLEARBACKGROUND(30, 30, 40)
    
    REM Begin 3D mode
    CAMERA3D_BEGIN(camera)
    
    REM Draw grid floor for reference (optional - can be removed if DRAWLINE3D doesn't work)
    REM Note: Grid drawing removed for now - can be added back if DRAWLINE3D is available
    
    REM Draw model if loaded
    IF modelLoaded = 1 AND currentModel >= 0 THEN
        IF wireframeMode = 1 THEN
            REM Draw wireframe
            DRAWMODELWIRES(currentModel, 0, 0, 0, 1.0, 200, 200, 200)
        ELSE
            REM Draw solid
            DRAWMODEL(currentModel, 0, 0, 0, 1.0, 255, 255, 255)
        ENDIF
    ENDIF
    
    REM End 3D mode
    CAMERA3D_END()
    
    REM Draw UI
    DRAWTEXT("3D Model Viewer", 10, 10, 30, 255, 255, 255)
    IF modelLoaded = 1 THEN
        DRAWTEXT("Model: " + modelPath$, 10, 50, 20, 200, 200, 255)
        DRAWTEXT("Status: Loaded", 10, 75, 18, 0, 255, 0)
    ELSE
        DRAWTEXT("Model: " + modelPath$, 10, 50, 20, 200, 200, 255)
        DRAWTEXT("Status: Failed to load", 10, 75, 18, 255, 0, 0)
    ENDIF
    
    DRAWTEXT("Controls:", 10, 110, 20, 255, 255, 255)
    DRAWTEXT("  Left Mouse + Drag: Orbit camera", 10, 135, 16, 200, 200, 200)
    DRAWTEXT("  Mouse Wheel: Zoom in/out", 10, 155, 16, 200, 200, 200)
    DRAWTEXT("  W: Toggle wireframe", 10, 175, 16, 200, 200, 200)
    DRAWTEXT("  R: Reset camera", 10, 195, 16, 200, 200, 200)
    DRAWTEXT("  L: Reload model", 10, 215, 16, 200, 200, 200)
    DRAWTEXT("  ESC: Exit", 10, 235, 16, 200, 200, 200)
    
    REM Camera info
    DRAWTEXT("Camera Info:", 10, 270, 18, 255, 255, 255)
    DRAWTEXT("Distance: " + STR(INT(cameraDistance * 10) / 10), 10, 295, 16, 200, 200, 200)
    DRAWTEXT("Angle X: " + STR(INT(cameraAngleX)) + " deg", 10, 315, 16, 200, 200, 200)
    DRAWTEXT("Angle Y: " + STR(INT(cameraAngleY)) + " deg", 10, 335, 16, 200, 200, 200)
    
    REM Mode indicator
    IF wireframeMode = 1 THEN
        DRAWTEXT("Mode: WIREFRAME", 10, 360, 16, 0, 255, 0)
    ELSE
        DRAWTEXT("Mode: SOLID", 10, 360, 16, 200, 200, 200)
    ENDIF
    
    REM Mouse debug info (can be removed if not needed)
    DRAWTEXT("Mouse: " + STR(GETMOUSEX()) + ", " + STR(GETMOUSEY()), 10, 385, 14, 150, 150)
    IF isDragging = 1 THEN
        DRAWTEXT("Dragging: YES", 10, 405, 14, 0, 255, 0)
    ELSE
        DRAWTEXT("Dragging: NO", 10, 405, 14, 150, 150, 150)
    ENDIF
    
    REM End drawing
    ENDDRAWING()
WEND

REM Cleanup
IF currentModel >= 0 THEN
    UNLOADMODEL(currentModel)
ENDIF

CLOSEWINDOW()
PRINT "Model viewer closed"
