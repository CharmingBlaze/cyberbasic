REM Collision Detection Demo
REM Demonstrates rectangle collision detection and basic game mechanics

INITWINDOW(800, 600, "Collision Detection Demo")
SETTARGETFPS(60)

REM Game objects
VAR player_x = 400
VAR player_y = 300
VAR player_w = 40
VAR player_h = 40
VAR player_speed = 5

VAR obstacle_x = 200
VAR obstacle_y = 150
VAR obstacle_w = 80
VAR obstacle_h = 60

VAR target_x = 600
VAR target_y = 400
VAR target_w = 30
VAR target_h = 30

VAR running = TRUE
VAR score = 0
VAR game_time = 0

WHILE running AND NOT WINDOWSHOULDCLOSE()
    REM Update game time
    game_time = game_time + 1
    
    REM Handle input
    IF ISKEYDOWN(KEY_W) THEN
        player_y = player_y - player_speed
    ENDIF
    IF ISKEYDOWN(KEY_S) THEN
        player_y = player_y + player_speed
    ENDIF
    IF ISKEYDOWN(KEY_A) THEN
        player_x = player_x - player_speed
    ENDIF
    IF ISKEYDOWN(KEY_D) THEN
        player_x = player_x + player_speed
    ENDIF
    
    REM Keep player on screen
    IF player_x < 0 THEN
        player_x = 0
    ENDIF
    IF player_x > 800 - player_w THEN
        player_x = 800 - player_w
    ENDIF
    IF player_y < 0 THEN
        player_y = 0
    ENDIF
    IF player_y > 600 - player_h THEN
        player_y = 600 - player_h
    ENDIF
    
    REM Check collisions using rectangle collision
    VAR hit_obstacle = COLLISIONRECTANGLES(player_x, player_y, player_w, player_h, obstacle_x, obstacle_y, obstacle_w, obstacle_h)
    VAR hit_target = COLLISIONRECTANGLES(player_x, player_y, player_w, player_h, target_x, target_y, target_w, target_h)
    
    REM Check if mouse is over player
    VAR mouse_x = GETMOUSEX()
    VAR mouse_y = GETMOUSEY()
    VAR mouse_over_player = POINTINRECTANGLE(mouse_x, mouse_y, player_x, player_y, player_w, player_h)
    
    REM Calculate distances using simple distance formula
    VAR player_center_x = player_x + player_w / 2
    VAR player_center_y = player_y + player_h / 2
    VAR obstacle_center_x = obstacle_x + obstacle_w / 2
    VAR obstacle_center_y = obstacle_y + obstacle_h / 2
    VAR target_center_x = target_x + target_w / 2
    VAR target_center_y = target_y + target_h / 2
    
    VAR dx_obstacle = player_center_x - obstacle_center_x
    VAR dy_obstacle = player_center_y - obstacle_center_y
    VAR dist_to_obstacle = SQRT(dx_obstacle * dx_obstacle + dy_obstacle * dy_obstacle)
    
    VAR dx_target = player_center_x - target_center_x
    VAR dy_target = player_center_y - target_center_y
    VAR dist_to_target = SQRT(dx_target * dx_target + dy_target * dy_target)
    
    REM Handle target collision
    IF hit_target THEN
        score = score + 10
        target_x = INT(RND() * 700) + 50
        target_y = INT(RND() * 500) + 50
    ENDIF
    
    REM Handle obstacle collision
    IF hit_obstacle THEN
        player_x = 400
        player_y = 300
        IF score > 5 THEN
            score = score - 5
        ELSE
            score = 0
        ENDIF
    ENDIF
    
    BEGINDRAWING()
    
    REM Clear background
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw obstacle (red)
    VAR obstacle_color_r = 200
    IF hit_obstacle THEN
        obstacle_color_r = 255
    ENDIF
    DRAWRECTANGLE(obstacle_x, obstacle_y, obstacle_w, obstacle_h, obstacle_color_r, 0, 0)
    DRAWTEXT("Obstacle", obstacle_x, obstacle_y - 20, 16, 255, 255, 255, 255)
    
    REM Draw target (green)
    VAR target_color_g = 255
    IF hit_target THEN
        target_color_g = 100
    ENDIF
    DRAWRECTANGLE(target_x, target_y, target_w, target_h, 0, target_color_g, 0)
    DRAWTEXT("Target", target_x, target_y - 20, 16, 255, 255, 255, 255)
    
    REM Draw player (blue, changes color on collision)
    VAR player_color_b = 100
    IF hit_obstacle THEN
        player_color_b = 255
    ENDIF
    DRAWRECTANGLE(player_x, player_y, player_w, player_h, 0, 0, player_color_b)
    DRAWTEXT("Player", player_x, player_y - 20, 16, 255, 255, 255, 255)
    
    REM Draw collision indicators
    IF hit_obstacle THEN
        DRAWTEXT("COLLISION!", player_x, player_y - 40, 20, 255, 255, 0, 255)
    ENDIF
    
    IF hit_target THEN
        DRAWTEXT("TARGET HIT!", target_x, target_y - 40, 20, 255, 255, 0, 255)
    ENDIF
    
    REM Draw UI
    DRAWTEXT("Collision Detection Demo", 10, 10, 24, 255, 255, 255, 255)
    DRAWTEXT("Use WASD to move", 10, 40, 18, 200, 200, 200, 255)
    DRAWTEXT("Score: " + STR(score), 10, 70, 20, 255, 255, 255, 255)
    DRAWTEXT("Game Time: " + STR(game_time), 10, 100, 18, 255, 255, 255, 255)
    
    REM Draw collision info
    DRAWTEXT("Collision Info:", 10, 130, 18, 255, 255, 255, 255)
    DRAWTEXT("Hit Obstacle: " + STR(hit_obstacle), 10, 160, 16, 255, 255, 255, 255)
    DRAWTEXT("Hit Target: " + STR(hit_target), 10, 190, 16, 255, 255, 255, 255)
    DRAWTEXT("Mouse Over Player: " + STR(mouse_over_player), 10, 220, 16, 255, 255, 255, 255)
    
    REM Draw distance info
    DRAWTEXT("Distance Info:", 10, 250, 18, 255, 255, 255, 255)
    DRAWTEXT("To Obstacle: " + STR(INT(dist_to_obstacle)), 10, 280, 16, 255, 255, 255, 255)
    DRAWTEXT("To Target: " + STR(INT(dist_to_target)), 10, 310, 16, 255, 255, 255, 255)
    
    REM Draw instructions
    DRAWTEXT("Avoid red obstacle, collect green targets", 10, 350, 16, 200, 200, 200, 255)
    DRAWTEXT("Press ESC to exit", 10, 550, 18, 200, 200, 200, 255)
    
    ENDDRAWING()
    
    REM Exit on ESC
    IF ISKEYPRESSED(KEY_ESCAPE) THEN
        running = FALSE
    ENDIF
WEND

CLOSEWINDOW()

PRINT "Collision demo completed!"
PRINT "Final Score: " + STR(score)
PRINT "Game Time: " + STR(game_time) + " frames"
