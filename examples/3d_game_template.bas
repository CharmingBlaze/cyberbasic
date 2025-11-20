// 3D Game Template
// Complete template for a 3D game using CyberBasic

// Initialize game
VAR screenWidth = 1280
VAR screenHeight = 720
GAME.init(screenWidth, screenHeight, "My 3D Game", 60)

// Create 3D camera
VAR camera = Camera3D()
camera.setPosition(0, 10, 10)
camera.setTarget(0, 0, 0)

// Create scene
VAR scene = Scene("MainScene")

// Create player entity
VAR player = scene.createEntity("Player")
player.setPosition(0, 0, 0)
player.addComponent("Model3D")

// Create ground entity
VAR ground = scene.createEntity("Ground")
ground.setPosition(0, -1, 0)
ground.addComponent("Model3D")

// Create timer
VAR gameTimer = Timer()
gameTimer.start()

// Game loop
WHILE NOT GAME.shouldClose()
    // Begin frame
    GAME.beginFrame()
    
    // Get delta time
    VAR delta = Clock.getDelta()
    
    // Handle input
    IF Input.getKey("W") THEN
        // Move player forward
    END IF
    IF Input.getKey("S") THEN
        // Move player backward
    END IF
    IF Input.getKey("A") THEN
        // Rotate player left
    END IF
    IF Input.getKey("D") THEN
        // Rotate player right
    END IF
    
    // Update scene
    scene.update()
    
    // Begin 3D camera mode
    camera.begin()
    
    // Draw scene
    scene.draw()
    
    // Draw 3D primitives
    // DrawGrid(10, 1.0)
    // DrawCube(Vector3(0, 0, 0), 2, 2, 2, RED)
    
    // End 3D camera mode
    camera.end()
    
    // Draw UI
    // DrawText("FPS: " + STR(Clock.getFPS()), 10, 10, 20, WHITE)
    
    // End frame
    GAME.endFrame()
WEND

// Cleanup
GAME.close()
