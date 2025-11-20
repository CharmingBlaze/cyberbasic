// 2D Game Template
// Complete template for a 2D game using CyberBasic

// Initialize game
VAR screenWidth = 800
VAR screenHeight = 600
GAME.init(screenWidth, screenHeight, "My 2D Game", 60)

// Create camera
VAR camera = Camera2D()
camera.setTarget(screenWidth / 2, screenHeight / 2)
camera.setZoom(1.0)

// Create player sprite
VAR player = Sprite("player.png")
player.x = screenWidth / 2
player.y = screenHeight / 2
player.scale = 1.0

// Create player animation
VAR walkAnim = Animation("walk", 8, 32, 32, 0.1)
walkAnim.setLooping(true)

// Create timer
VAR gameTimer = Timer()
gameTimer.start()

// Game loop
WHILE NOT GAME.shouldClose()
    // Begin frame
    GAME.beginFrame()
    
    // Get delta time
    VAR delta = Clock.getDelta()
    
    // Update animation
    walkAnim.update(delta)
    
    // Handle input
    IF Input.getKey("W") THEN
        player.y = player.y - 200 * delta
    END IF
    IF Input.getKey("S") THEN
        player.y = player.y + 200 * delta
    END IF
    IF Input.getKey("A") THEN
        player.x = player.x - 200 * delta
    END IF
    IF Input.getKey("D") THEN
        player.x = player.x + 200 * delta
    END IF
    
    // Update camera to follow player
    camera.setTarget(player.x, player.y)
    
    // Begin 2D camera mode
    camera.begin()
    
    // Draw game objects
    player.draw()
    
    // End 2D camera mode
    camera.end()
    
    // Draw UI (not affected by camera)
    // DrawText("FPS: " + STR(Clock.getFPS()), 10, 10, 20, WHITE)
    
    // End frame
    GAME.endFrame()
WEND

// Cleanup
GAME.close()

