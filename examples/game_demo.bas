REM Game Demo - CyberBasic Example
REM This example demonstrates game demo functionality
REM Use WASD or arrow keys to interact (where applicable)

INITWINDOW(800, 600, "Modern BASIC Game Demo")
SETTARGETFPS(60)

LET player_x = 400
LET player_y = 300
LET player_speed = 5
LET player_size = 20
LET score = 0
LET lives = 3
LET game_time = 0

LET enemy_x = 100
LET enemy_y = 100
LET enemy_speed = 2
LET enemy_size = 15

LET bullet_x = 0
LET bullet_y = 0
LET bullet_speed = 8
LET bullet_active = 0

LET game_running = 1

WHILE game_running AND NOT WINDOWSHOULDCLOSE()
    LET game_time = game_time + GETFRAMETIME()
    
    IF ISKEYDOWN(87) THEN LET player_y = player_y - player_speed ENDIF
    IF ISKEYDOWN(83) THEN LET player_y = player_y + player_speed ENDIF
    IF ISKEYDOWN(65) THEN LET player_x = player_x - player_speed ENDIF
    IF ISKEYDOWN(68) THEN LET player_x = player_x + player_speed ENDIF
    IF ISKEYDOWN(32) AND NOT bullet_active THEN
        LET bullet_x = player_x
        LET bullet_y = player_y
        LET bullet_active = 1
    ENDIF
    
    IF player_x < player_size THEN LET player_x = player_size ENDIF
    IF player_x > 800 - player_size THEN LET player_x = 800 - player_size ENDIF
    IF player_y < player_size THEN LET player_y = player_size ENDIF
    IF player_y > 600 - player_size THEN LET player_y = 600 - player_size ENDIF
    
    IF bullet_active THEN
        LET bullet_y = bullet_y - bullet_speed
        IF bullet_y < 0 THEN LET bullet_active = 0 ENDIF
    ENDIF
    
    IF enemy_x < player_x THEN LET enemy_x = enemy_x + enemy_speed ENDIF
    IF enemy_x > player_x THEN LET enemy_x = enemy_x - enemy_speed ENDIF
    IF enemy_y < player_y THEN LET enemy_y = enemy_y + enemy_speed ENDIF
    IF enemy_y > player_y THEN LET enemy_y = enemy_y - enemy_speed ENDIF
    
    IF bullet_active THEN
        IF CHECKCOLLISIONCIRCLES(bullet_x, bullet_y, 5, enemy_x, enemy_y, enemy_size) THEN
            LET bullet_active = 0
            LET enemy_x = RANDOM(50, 750)
            LET enemy_y = RANDOM(50, 550)
            LET score = score + 10
            LET enemy_speed = enemy_speed + 0.5
        ENDIF
    ENDIF
    
    IF CHECKCOLLISIONCIRCLES(player_x, player_y, player_size, enemy_x, enemy_y, enemy_size) THEN
        LET lives = lives - 1
        LET enemy_x = RANDOM(50, 750)
        LET enemy_y = RANDOM(50, 550)
        IF lives <= 0 THEN LET game_running = 0 ENDIF
    ENDIF
    
    BEGINDRAWING()
    
    CLEARBACKGROUND(20, 20, 40)
    
    DRAWCIRCLE(player_x, player_y, player_size, 0, 255, 0)
    
    DRAWCIRCLE(enemy_x, enemy_y, enemy_size, 255, 0, 0)
    
    IF bullet_active THEN
        DRAWCIRCLE(bullet_x, bullet_y, 5, 255, 255, 0)
    ENDIF
    
    DRAWTEXT("Score", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Lives", 10, 35, 20, 255, 255, 255)
    DRAWTEXT("Time", 10, 60, 20, 255, 255, 255)
    
    DRAWTEXT("WASD to move, SPACE to shoot", 10, 570, 16, 200, 200)
    
    IF NOT game_running THEN
        DRAWTEXT("GAME OVER!", 300, 250, 40, 255, 0, 0)
        DRAWTEXT("Final Score", 300, 300, 30, 255, 255, 255)
        DRAWTEXT("Press ESC to exit", 300, 350, 20, 200, 200, 200)
    ENDIF
    
    ENDDRAWING()
    
    IF ISKEYPRESSED(256) THEN LET game_running = 0 ENDIF
WEND

CLOSEWINDOW()
PRINT "Game ended. Final score: "
