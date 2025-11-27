REM Complete 3D Game Demo
REM Demonstrates all the new 3D features, camera system, and game development APIs

PRINT "=== Complete 3D Game Demo ==="
PRINT "Initializing game..."

' Initialize the game
INITGAME(800, 600, "CyberBasic 3D Game Demo")
SETTARGETFPS(60)

PRINT "Setting up 3D camera..."
' Setup first-person camera
SETUP3DCAMERAFIRSTPERSON(0, 2, 10, 0, 2, 0, 60)

PRINT "Loading 3D models..."
' Load some 3D models (these would be actual model files in a real game)
' For demo purposes, we'll use basic shapes
LET cube_mesh = GENERATEMESHCUBE(2.0, 2.0, 2.0)
LET sphere_mesh = GENERATEMESHSPHERE(1.0, 16, 16)
LET cylinder_mesh = GENERATEMESHCYLINDER(1.0, 3.0, 16)

PRINT "Creating particle effects..."
' Create some particle effects
LET fire_effect = CREATEFIREEFFECT(0, 0)
LET explosion_effect = CREATEEXPLOSIONEFFECT(5, 0)
LET sparkle_effect = CREATESPARKLEEFFECT(-5, 0)

PRINT "Setting up post-processing effects..."
' Create post-processing effects
LET blur_effect = CREATEBLUREFFECT(2)
LET bloom_effect = CREATEBLOOMEFFECT(3)
LET color_effect = CREATECOLOREFFECT(100, 100, 100, 0)

PRINT "Loading game assets..."
' Load sprites and sounds (placeholder IDs)
LET player_sprite = LOADSPRITE("player.png")
LET background_music = LOADGAMEMUSIC("background.ogg")
LET jump_sound = LOADGAMESOUND("jump.wav")

PRINT "Starting game loop..."
LET game_time = 0.0
LET player_x = 0.0
LET player_y = 0.0
LET player_z = 0.0
LET camera_yaw = 0.0
LET camera_pitch = 0.0

' Main game loop
WHILE NOT GAMESHOULDCLOSE()
    LET delta_time = GETGAMEDELTATIME()
    LET game_time = game_time + delta_time
    
    ' Begin game frame
    BEGINGAMEFRAME()
    
    ' Update camera
    UPDATECAMERA3D()
    
    ' Handle input
    IF ISKEYHELD(87) THEN ' W key
        CAMERA3DMOVEFORWARD(5.0 * delta_time)
    END IF
    IF ISKEYHELD(83) THEN ' S key
        CAMERA3DMOVEBACKWARD(5.0 * delta_time)
    END IF
    IF ISKEYHELD(65) THEN ' A key
        CAMERA3DROTATE(-90.0 * delta_time, 0.0)
    END IF
    IF ISKEYHELD(68) THEN ' D key
        CAMERA3DROTATE(90.0 * delta_time, 0.0)
    END IF
    IF ISKEYHELD(32) THEN ' Space key
        CAMERA3DMOVEUP(5.0 * delta_time)
    END IF
    IF ISKEYHELD(16) THEN ' Shift key
        CAMERA3DMOVEDOWN(5.0 * delta_time)
    END IF
    
    ' Mouse look
    LET mouse_pos = GETMOUSEPOSITION()
    ' Simple mouse look implementation would go here
    
    ' Update particle systems
    UPDATEPARTICLESYSTEM(fire_effect, delta_time)
    UPDATEPARTICLESYSTEM(explosion_effect, delta_time)
    UPDATEPARTICLESYSTEM(sparkle_effect, delta_time)
    
    ' Begin 3D drawing
    BEGINMODE3D()
    
    ' Draw 3D scene
    ' Draw a grid
    DRAWGRID(20, 1.0)
    
    ' Draw some 3D shapes
    DRAWCUBE(0, 0, 0, 2, 2, 2, 255, 0, 0)
    DRAWSPHERE(5, 0, 0, 1.0, 0, 255, 0)
    DRAWCYLINDER(-5, 0, 0, 1.0, 1.0, 3.0, 16, 0, 0, 255)
    
    ' Draw some wireframe shapes
    DRAWCUBEWIRES(0, 3, 0, 1, 1, 1, 255, 255, 0)
    DRAWSPHEREWIRES(3, 3, 0, 0.5, 16, 16, 255, 0, 255)
    
    ' Draw meshes
    DRAWMESH(cube_mesh, 0, 6, 0, 1.0, 128, 128, 128)
    DRAWMESH(sphere_mesh, 6, 6, 0, 1.0, 200, 100, 100)
    DRAWMESH(cylinder_mesh, -6, 6, 0, 1.0, 100, 200, 100)
    
    ' End 3D drawing
    ENDMODE3D()
    
    ' Render particle systems
    RENDERPARTICLESYSTEM(fire_effect)
    RENDERPARTICLESYSTEM(explosion_effect)
    RENDERPARTICLESYSTEM(sparkle_effect)
    
    ' Draw 2D UI elements
    DRAWTEXT("CyberBasic 3D Game Demo", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("WASD: Move, Space: Up, Shift: Down", 10, 40, 16, 200)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 60, 16, 200, 200, 200)
    
    ' Draw player sprite (2D overlay)
    DRAWSPRITE(player_sprite, 700, 500, 64, 64, 0, 255, 255, 255)
    
    ' Apply post-processing effects
    BEGINPOSTPROCESSING()
    RENDERPOSTEFFECTS()
    ENDPOSTPROCESSING()
    
    ' End game frame
    ENDGAMEFRAME()
    
    ' Play background music
    IF game_time > 1.0 THEN
        PLAYGAMEMUSIC(background_music)
    END IF
    
    ' Trigger explosion effect occasionally
    IF RANDOMINT(1, 1000) = 1 THEN
        LET explosion_effect = CREATEEXPLOSIONEFFECT(RANDOMINT(-10, 10), RANDOMINT(-10, 10))
        PLAYGAMESOUND(jump_sound)
    END IF
WEND

PRINT "Cleaning up..."
' Clean up resources
CLEARALLPARTICLESYSTEMS()
CLEARALLPOSTEFFECTS()
STOPGAMEMUSIC(background_music)
STOPGAMESOUND(jump_sound)

PRINT "Closing game..."
CLOSEGAME()

PRINT "Complete 3D Game Demo finished!"
PRINT "All new features demonstrated successfully!"
