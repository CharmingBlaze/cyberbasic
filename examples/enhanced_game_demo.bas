INITWINDOW(800, 600, "Enhanced BASIC Game Demo")
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
LET game_paused = 0

LET explosion_x = 0
LET explosion_y = 0
LET explosion_active = 0
LET explosion_timer = 0

LET powerup_x = 0
LET powerup_y = 0
LET powerup_active = 0
LET powerup_type = 0

WHILE game_running AND NOT WINDOWSHOULDCLOSE()
    IF NOT game_paused THEN
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
                LET explosion_x = enemy_x
                LET explosion_y = enemy_y
                LET explosion_active = 1
                LET explosion_timer = 30
                LET enemy_x = RANDOM(50, 750)
                LET enemy_y = RANDOM(50, 550)
                LET score = score + 10
                LET enemy_speed = enemy_speed + 0.5
            ENDIF
        ENDIF
        
        IF CHECKCOLLISIONCIRCLES(player_x, player_y, player_size, enemy_x, enemy_y, enemy_size) THEN
            LET lives = lives - 1
            LET explosion_x = enemy_x
            LET explosion_y = enemy_y
            LET explosion_active = 1
            LET explosion_timer = 30
            LET enemy_x = RANDOM(50, 750)
            LET enemy_y = RANDOM(50, 550)
            IF lives <= 0 THEN LET game_running = 0 ENDIF
        ENDIF
        
        IF explosion_active THEN
            LET explosion_timer = explosion_timer - 1
            IF explosion_timer <= 0 THEN LET explosion_active = 0 ENDIF
        ENDIF
        
        IF NOT powerup_active AND RANDOM(1, 100) < 2 THEN
            LET powerup_x = RANDOM(50, 750)
            LET powerup_y = RANDOM(50, 550)
            LET powerup_active = 1
            LET powerup_type = RANDOM(1, 3)
        ENDIF
        
        IF powerup_active THEN
            IF CHECKCOLLISIONCIRCLES(player_x, player_y, player_size, powerup_x, powerup_y, 15) THEN
                IF powerup_type = 1 THEN LET lives = lives + 1 ENDIF
                IF powerup_type = 2 THEN LET score = score + 50 ENDIF
                IF powerup_type = 3 THEN LET player_speed = player_speed + 1 ENDIF
                LET powerup_active = 0
            ENDIF
        ENDIF
    ENDIF
    
    IF ISKEYPRESSED(80) THEN LET game_paused = NOT game_paused ENDIF
    
    BEGINDRAW()
    
    CLEARBACKGROUND(20, 20, 40)
    
    DRAWCIRCLE(player_x, player_y, player_size, 0, 255, 0)
    DRAWRECTANGLE(player_x - 2, player_y - 2, 4, 4, 255, 255, 255)
    
    DRAWCIRCLE(enemy_x, enemy_y, enemy_size, 255, 0, 0)
    DRAWRECTANGLE(enemy_x - 2, enemy_y - 2, 4, 4, 255, 255, 255)
    
    IF bullet_active THEN
        DRAWCIRCLE(bullet_x, bullet_y, 5, 255, 255, 0)
        DRAWCIRCLE(bullet_x, bullet_y, 3, 255, 255, 255)
    ENDIF
    
    IF explosion_active THEN
        LET explosion_size = 30 - explosion_timer
        DRAWCIRCLE(explosion_x, explosion_y, explosion_size, 255, 165, 0)
        DRAWCIRCLE(explosion_x, explosion_y, explosion_size - 5, 255, 255, 0)
    ENDIF
    
    IF powerup_active THEN
        IF powerup_type = 1 THEN
            DRAWCIRCLE(powerup_x, powerup_y, 15, 0, 255, 255)
            DRAWTEXT("H", powerup_x - 5, powerup_y - 10, 20, 255, 255, 255)
        ENDIF
        IF powerup_type = 2 THEN
            DRAWCIRCLE(powerup_x, powerup_y, 15, 255, 0, 255)
            DRAWTEXT("S", powerup_x - 5, powerup_y - 10, 20, 255, 255, 255)
        ENDIF
        IF powerup_type = 3 THEN
            DRAWCIRCLE(powerup_x, powerup_y, 15, 255, 255, 0)
            DRAWTEXT("F", powerup_x - 5, powerup_y - 10, 20, 255, 255, 255)
        ENDIF
    ENDIF
    
    DRAWTEXT("Score:", 10, 10, 20, 255, 255, 255)
    IF score = 0 THEN DRAWTEXT("0", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 10 THEN DRAWTEXT("10", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 20 THEN DRAWTEXT("20", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 30 THEN DRAWTEXT("30", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 40 THEN DRAWTEXT("40", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 50 THEN DRAWTEXT("50", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 60 THEN DRAWTEXT("60", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 70 THEN DRAWTEXT("70", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 80 THEN DRAWTEXT("80", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 90 THEN DRAWTEXT("90", 80, 10, 20, 255, 255, 0) ENDIF
    IF score = 100 THEN DRAWTEXT("100", 80, 10, 20, 255, 255, 0) ENDIF
    IF score > 100 THEN DRAWTEXT("100+", 80, 10, 20, 255, 255, 0) ENDIF
    
    DRAWTEXT("Lives:", 10, 35, 20, 255, 255, 255)
    IF lives = 1 THEN DRAWTEXT("1", 80, 35, 20, 0, 255, 0) ENDIF
    IF lives = 2 THEN DRAWTEXT("2", 80, 35, 20, 0, 255, 0) ENDIF
    IF lives = 3 THEN DRAWTEXT("3", 80, 35, 20, 0, 255, 0) ENDIF
    IF lives = 4 THEN DRAWTEXT("4", 80, 35, 20, 0, 255, 0) ENDIF
    IF lives = 5 THEN DRAWTEXT("5", 80, 35, 20, 0, 255, 0) ENDIF
    IF lives = 0 THEN DRAWTEXT("0", 80, 35, 20, 255, 0, 0) ENDIF
    
    DRAWTEXT("Speed:", 10, 85, 20, 255, 255, 255)
    IF player_speed = 5 THEN DRAWTEXT("5", 80, 85, 20, 0, 255, 255) ENDIF
    IF player_speed = 6 THEN DRAWTEXT("6", 80, 85, 20, 0, 255, 255) ENDIF
    IF player_speed = 7 THEN DRAWTEXT("7", 80, 85, 20, 0, 255, 255) ENDIF
    IF player_speed = 8 THEN DRAWTEXT("8", 80, 85, 20, 0, 255, 255) ENDIF
    IF player_speed = 9 THEN DRAWTEXT("9", 80, 85, 20, 0, 255, 255) ENDIF
    IF player_speed = 10 THEN DRAWTEXT("10", 80, 85, 20, 0, 255, 255) ENDIF
    
    DRAWTEXT("WASD to move, SPACE to shoot, P to pause", 10, 570, 16, 200, 200, 200)
    
    IF game_paused THEN
        DRAWTEXT("PAUSED", 350, 250, 40, 255, 255, 0)
        DRAWTEXT("Press P to resume", 320, 300, 20, 200, 200, 200)
    ENDIF
    
    IF NOT game_running THEN
        DRAWTEXT("GAME OVER!", 300, 200, 40, 255, 0, 0)
        DRAWTEXT("Final Score:", 300, 250, 30, 255, 255, 255)
        DRAWTEXT("Press ESC to exit", 300, 320, 20, 200, 200, 200)
    ENDIF
    
    ENDDRAW()
    
    IF ISKEYPRESSED(256) THEN LET game_running = 0 ENDIF
WEND

CLOSEWINDOW()
PRINT "Game ended. Final score: "
PRINT "Time survived: seconds"
