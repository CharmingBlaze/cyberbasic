INITWINDOW(800, 600, "Collision Detection Demo")
SETTARGETFPS(60)

REM Game objects
LET player_x = 400
LET player_y = 300
LET player_w = 40
LET player_h = 40
LET player_speed = 5

LET obstacle_x = 200
LET obstacle_y = 150
LET obstacle_w = 80
LET obstacle_h = 60

LET target_x = 600
LET target_y = 400
LET target_w = 30
LET target_h = 30

LET running = TRUE
LET score = 0
LET game_time = 0

WHILE running AND NOT WINDOWSHOULDCLOSE()
    REM Update game time
    LET game_time = game_time + 1
    
    REM Handle input
    IF ISKEYDOWN(87) THEN LET player_y = player_y - player_speed ENDIF
    IF ISKEYDOWN(83) THEN LET player_y = player_y + player_speed ENDIF
    IF ISKEYDOWN(65) THEN LET player_x = player_x - player_speed ENDIF
    IF ISKEYDOWN(68) THEN LET player_x = player_x + player_speed ENDIF
    
    REM Keep player on screen
    IF player_x < 0 THEN LET player_x = 0 ENDIF
    IF player_x > 800 - player_w THEN LET player_x = 800 - player_w ENDIF
    IF player_y < 0 THEN LET player_y = 0 ENDIF
    IF player_y > 600 - player_h THEN LET player_y = 0 ENDIF
    
    REM Check collisions
    LET hit_obstacle = COLLISIONRECTANGLES(player_x, player_y, player_w, player_h, obstacle_x, obstacle_y, obstacle_w, obstacle_h)
    LET hit_target = COLLISIONRECTANGLES(player_x, player_y, player_w, player_h, target_x, target_y, target_w, target_h)
    
    REM Check if mouse is over player (using new mouse functions)
    LET mouse_x = GETMOUSEX()
    LET mouse_y = GETMOUSEY()
    LET mouse_over_player = POINTINRECTANGLE(mouse_x, mouse_y, player_x, player_y, player_w, player_h)
    
    REM Calculate distances
    LET player_center_x = player_x + player_w/2
    LET player_center_y = player_y + player_h/2
    LET obstacle_center_x = obstacle_x + obstacle_w/2
    LET obstacle_center_y = obstacle_y + obstacle_h/2
    LET target_center_x = target_x + target_w/2
    LET target_center_y = target_y + target_h/2
    
    LET dist_to_obstacle = DISTANCE(player_center_x, player_center_y, obstacle_center_x, obstacle_center_y)
    LET dist_to_target = DISTANCE(player_center_x, player_center_y, target_center_x, target_center_y)
    
    REM Handle target collision
    IF hit_target THEN
        LET score = score + 10
        LET target_x = RANDOM(700)
        LET target_y = RANDOM(500)
    ENDIF
    
    REM Handle obstacle collision
    IF hit_obstacle THEN
        LET player_x = 400
        LET player_y = 300
        LET score = MAX(0, score - 5)
    ENDIF
    
    BEGINDRAW()
    
    REM Clear background
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw obstacle (red)
    LET obstacle_color = 255
    IF hit_obstacle THEN LET obstacle_color = 255 ELSE LET obstacle_color = 200 ENDIF
    DRAWRECTANGLE(obstacle_x, obstacle_y, obstacle_w, obstacle_h, obstacle_color, 0, 0)
    DRAWTEXT("Obstacle", obstacle_x, obstacle_y - 20, 16, 255, 255, 255)
    
    REM Draw target (green)
    LET target_color = 0
    IF hit_target THEN LET target_color = 100 ELSE LET target_color = 255 ENDIF
    DRAWRECTANGLE(target_x, target_y, target_w, target_h, 0, target_color, 0)
    DRAWTEXT("Target", target_x, target_y - 20, 16, 255, 255, 255)
    
    REM Draw player (blue, changes color on collision)
    LET player_color = 0
    IF hit_obstacle THEN LET player_color = 255 ELSE LET player_color = 100 ENDIF
    DRAWRECTANGLE(player_x, player_y, player_w, player_h, 0, 0, player_color)
    DRAWTEXT("Player", player_x, player_y - 20, 16, 255, 255, 255)
    
    REM Draw collision indicators
    IF hit_obstacle THEN
        DRAWTEXT("COLLISION!", player_x, player_y - 40, 20, 255, 255, 0)
    ENDIF
    
    IF hit_target THEN
        DRAWTEXT("TARGET HIT!", target_x, target_y - 40, 20, 255, 255, 0)
    ENDIF
    
    REM Draw UI
    DRAWTEXT("Collision Detection Demo", 10, 10, 24, 255, 255, 255)
    DRAWTEXT("Use WASD to move", 10, 40, 18, 200, 200, 200)
    DRAWTEXT("Score: " + STR(score), 10, 70, 20, 255, 255, 255)
    DRAWTEXT("Game Time: " + STR(game_time), 10, 100, 18, 255, 255, 255)
    
    REM Draw collision info
    DRAWTEXT("Collision Info:", 10, 130, 18, 255, 255, 255)
    DRAWTEXT("Hit Obstacle: " + STR(hit_obstacle), 10, 160, 16, 255, 255, 255)
    DRAWTEXT("Hit Target: " + STR(hit_target), 10, 190, 16, 255, 255, 255)
    DRAWTEXT("Mouse Over Player: " + STR(mouse_over_player), 10, 220, 16, 255, 255, 255)
    
    REM Draw distance info
    DRAWTEXT("Distance Info:", 10, 250, 18, 255, 255, 255)
    DRAWTEXT("To Obstacle: " + STR(INT(dist_to_obstacle)), 10, 280, 16, 255, 255, 255)
    DRAWTEXT("To Target: " + STR(INT(dist_to_target)), 10, 310, 16, 255, 255, 255)
    
    REM Draw instructions
    DRAWTEXT("Avoid red obstacle, collect green targets", 10, 350, 16, 200, 200, 200)
    DRAWTEXT("Press ESC to exit", 10, 550, 18, 200, 200, 200)
    
    ENDDRAW()
    
    REM Exit on ESC
    IF ISKEYPRESSED(256) THEN LET running = FALSE ENDIF
WEND

CLOSEWINDOW()

PRINT "Collision demo completed!"
PRINT "Final Score: " + STR(score)
PRINT "Game Time: " + STR(game_time) + " frames"
