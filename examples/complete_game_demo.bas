REM Complete Game Programming Demo for CyberBasic
REM This demonstrates all the essential game programming systems

PRINT "=== CyberBasic Complete Game Programming Demo ==="
PRINT "Initializing all game systems..."

REM Initialize all game systems
INITGAMESYSTEMS()

REM Create a graphics window
INITWINDOW(1024, 768, "CyberBasic Game Demo")
SETTARGETFPS(60)

PRINT "Graphics system initialized!"

REM Initialize audio system
INITPHYSICS()

PRINT "Physics system initialized!"

REM Create game objects
LET player_id = CREATEGAMEOBJECT("Player")
LET enemy_id = CREATEGAMEOBJECT("Enemy")
LET collectible_id = CREATEGAMEOBJECT("Collectible")

PRINT "Game objects created!"

REM Set up player
SETGAMEOBJECTPOSITION(player_id, 100, 100, 0)
SETGAMEOBJECTSCALE(player_id, 50, 50, 1)

REM Set up enemy
SETGAMEOBJECTPOSITION(enemy_id, 500, 300, 0)
SETGAMEOBJECTSCALE(enemy_id, 40, 40, 1)

REM Set up collectible
SETGAMEOBJECTPOSITION(collectible_id, 800, 200, 0)
SETGAMEOBJECTSCALE(collectible_id, 20, 20, 1)

REM Create a scene
CREATESCENE("MainGame")
ADDOBJECTTOSCENE("MainGame", player_id)
ADDOBJECTTOSCENE("MainGame", enemy_id)
ADDOBJECTTOSCENE("MainGame", collectible_id)
LOADSCENE("MainGame")

REM Set game state
SETGAMESTATE("playing")
SETGAMESTATEDATA("score", 0)
SETGAMESTATEDATA("lives", 3)

PRINT "Game setup complete! Starting game loop..."
PRINT "Controls: WASD to move, ESC to exit"

REM Main game loop
WHILE NOT WINDOWSHOULDCLOSE()
    GAMELOOPSTART()
    
    REM Update game state
    GAMELOOPUPDATE()
    
    REM Clear screen
    CLEARBACKGROUND(0, 0, 50)
    
    REM Handle input
    LET player_pos = GETGAMEOBJECTPOSITION(player_id)
    LET new_x = player_pos(0)
    LET new_y = player_pos(1)
    
    IF ISKEYDOWN(87) THEN  REM W key
        LET new_y = new_y - 200 * GETDELTATIME()
    ENDIF
    
    IF ISKEYDOWN(83) THEN  REM S key
        LET new_y = new_y + 200 * GETDELTATIME()
    ENDIF
    
    IF ISKEYDOWN(65) THEN  REM A key
        LET new_x = new_x - 200 * GETDELTATIME()
    ENDIF
    
    IF ISKEYDOWN(68) THEN  REM D key
        LET new_x = new_x + 200 * GETDELTATIME()
    ENDIF
    
    REM Keep player on screen
    LET new_x = CLAMP(new_x, 25, 999)
    LET new_y = CLAMP(new_y, 25, 743)
    
    SETGAMEOBJECTPOSITION(player_id, new_x, new_y, 0)
    
    REM Simple AI for enemy (move towards player)
    LET enemy_pos = GETGAMEOBJECTPOSITION(enemy_id)
    LET player_pos = GETGAMEOBJECTPOSITION(player_id)
    
    LET dx = player_pos(0) - enemy_pos(0)
    LET dy = player_pos(1) - enemy_pos(1)
    LET distance = DISTANCE(enemy_pos(0), enemy_pos(1), player_pos(0), player_pos(1))
    
    IF distance > 0 THEN
        LET move_speed = 100 * GETDELTATIME()
        LET new_enemy_x = enemy_pos(0) + (dx / distance) * move_speed
        LET new_enemy_y = enemy_pos(1) + (dy / distance) * move_speed
        SETGAMEOBJECTPOSITION(enemy_id, new_enemy_x, new_enemy_y, 0)
    ENDIF
    
    REM Check collision between player and collectible
    IF CHECKCOLLISION(player_id, collectible_id) THEN
        LET current_score = GETGAMESTATEDATA("score")
        SETGAMESTATEDATA("score", current_score + 10)
        
        REM Move collectible to new random position
        LET new_collectible_x = RANDOM(900) + 50
        LET new_collectible_y = RANDOM(600) + 50
        SETGAMEOBJECTPOSITION(collectible_id, new_collectible_x, new_collectible_y, 0)
        
        PRINT "Score: " + STR(GETGAMESTATEDATA("score"))
    ENDIF
    
    REM Check collision between player and enemy
    IF CHECKCOLLISION(player_id, enemy_id) THEN
        LET current_lives = GETGAMESTATEDATA("lives")
        SETGAMESTATEDATA("lives", current_lives - 1)
        
        REM Reset player position
        SETGAMEOBJECTPOSITION(player_id, 100, 100, 0)
        
        PRINT "Lives remaining: " + STR(GETGAMESTATEDATA("lives"))
        
        IF GETGAMESTATEDATA("lives") <= 0 THEN
            PRINT "Game Over!"
            SETGAMESTATE("gameover")
        ENDIF
    ENDIF
    
    REM Draw game objects
    LET player_pos = GETGAMEOBJECTPOSITION(player_id)
    DRAWCIRCLE(INT(player_pos(0)), INT(player_pos(1)), 25, 0, 255, 0)
    
    LET enemy_pos = GETGAMEOBJECTPOSITION(enemy_id)
    DRAWCIRCLE(INT(enemy_pos(0)), INT(enemy_pos(1)), 20, 255, 0, 0)
    
    LET collectible_pos = GETGAMEOBJECTPOSITION(collectible_id)
    DRAWCIRCLE(INT(collectible_pos(0)), INT(collectible_pos(1)), 10, 255, 255, 0)
    
    REM Draw UI
    DRAWTEXT("Score: " + STR(GETGAMESTATEDATA("score")), 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Lives: " + STR(GETGAMESTATEDATA("lives")), 10, 40, 20, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 70, 20, 255, 255, 255)
    
    REM Draw instructions
    DRAWTEXT("WASD to move, ESC to exit", 10, 700, 16, 200, 200, 200)
    
    REM Check for game over state
    IF GETGAMESTATE() = "gameover" THEN
        DRAWTEXT("GAME OVER - Press ESC to exit", 400, 350, 30, 255, 0, 0)
    ENDIF
    
    GAMELOOPEND()
WEND

PRINT "Shutting down game systems..."
SHUTDOWNGAMESYSTEMS()
CLOSEWINDOW()

PRINT "Demo completed successfully!"
PRINT "Final Score: " + STR(GETGAMESTATEDATA("score"))
