// Complete Game Template
// Shows how to use modern CyberBasic features in a game

// Initialize window
Window.init(800, 600, "My Game")
Window.setTargetFPS(60)

// Create camera
LET camera = Camera3D()
camera.position = Vector3(0, 10, 10)
camera.target = Vector3(0, 0, 0)
camera.up = Vector3(0, 1, 0)

// Game state
LET playerPos = Vector2(400, 300)
LET playerSpeed = 5.0
LET score = 0

// Colors
LET bgColor = Color(30, 30, 40, 255)
LET playerColor = Color(100, 200, 255, 255)
LET textColor = Color(255, 255, 255, 255)

// Main game loop
WHILE NOT Window.shouldClose()
    // Input handling
    IF Input.isKeyDown(KEY_LEFT) THEN
        playerPos.x = playerPos.x - playerSpeed
    ENDIF
    IF Input.isKeyDown(KEY_RIGHT) THEN
        playerPos.x = playerPos.x + playerSpeed
    ENDIF
    IF Input.isKeyDown(KEY_UP) THEN
        playerPos.y = playerPos.y - playerSpeed
    ENDIF
    IF Input.isKeyDown(KEY_DOWN) THEN
        playerPos.y = playerPos.y + playerSpeed
    ENDIF
    
    // Keep player on screen
    IF playerPos.x < 0 THEN playerPos.x = 0
    IF playerPos.x > 800 THEN playerPos.x = 800
    IF playerPos.y < 0 THEN playerPos.y = 0
    IF playerPos.y > 600 THEN playerPos.y = 600
    
    // Drawing
    Graphics.beginDrawing()
    Graphics.clearBackground(bgColor)
    
    // Draw player
    Graphics.drawCircle(playerPos.x, playerPos.y, 20, playerColor)
    
    // Draw UI
    Graphics.drawText("Score: " + STR(score), 10, 10, 20, textColor)
    Graphics.drawText("FPS: " + STR(Window.getFPS()), 10, 35, 20, textColor)
    
    Graphics.endDrawing()
WEND

Window.close()

