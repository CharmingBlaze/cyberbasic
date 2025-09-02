INITWINDOW(800, 600, "Navigation System Demo")
SETTARGETFPS(60)

REM Initialize navigation system
INITNAVGRID(25, 19, 32)

REM Add some obstacles
ADDNAVOBSTACLE(100, 100, 200, 50)
ADDNAVOBSTACLE(300, 200, 150, 100)
ADDNAVOBSTACLE(500, 150, 100, 200)
ADDNAVOBSTACLE(200, 400, 300, 80)

REM Player position
LET player_x = 50
LET player_y = 50
LET player_speed = 3

REM Target position
LET target_x = 700
LET target_y = 500

REM Path variables
LET path_id = -1
LET current_waypoint = 0
LET path_found = 0

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle input
    IF ISKEYDOWN(87) THEN LET player_y = player_y - player_speed ENDIF
    IF ISKEYDOWN(83) THEN LET player_y = player_y + player_speed ENDIF
    IF ISKEYDOWN(65) THEN LET player_x = player_x - player_speed ENDIF
    IF ISKEYDOWN(68) THEN LET player_x = player_x + player_speed ENDIF
    
    REM Set target with mouse
    IF ISMOUSEBUTTONPRESSED(0) THEN
        LET target_x = GETMOUSEX()
        LET target_y = GETMOUSEY()
        LET path_found = 0
    ENDIF
    
    REM Find path when space is pressed
    IF ISKEYPRESSED(32) THEN
        LET path_id = CREATENAVPATH(player_x, player_y, target_x, target_y)
        IF path_id >= 0 THEN
            LET path_found = 1
            LET current_waypoint = 0
        ELSE
            LET path_found = 0
        ENDIF
    ENDIF
    
    REM Follow path if available
    IF path_found AND path_id >= 0 THEN
        IF NOT ISNAVPATHCOMPLETE(path_id, current_waypoint) THEN
            LET next_x = GETNEXTWAYPOINT(path_id, current_waypoint)
            REM Move towards waypoint (simplified)
            LET current_waypoint = current_waypoint + 1
        ENDIF
    ENDIF
    
    REM Keep player on screen
    IF player_x < 0 THEN LET player_x = 0 ENDIF
    IF player_x > 800 THEN LET player_x = 800 ENDIF
    IF player_y < 0 THEN LET player_y = 0 ENDIF
    IF player_y > 600 THEN LET player_y = 600 ENDIF
    
    REM Drawing
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw obstacles
    DRAWRECTANGLE(100, 100, 200, 50, 100, 50, 50)
    DRAWRECTANGLE(300, 200, 150, 100, 100, 50, 50)
    DRAWRECTANGLE(500, 150, 100, 200, 100, 50, 50)
    DRAWRECTANGLE(200, 400, 300, 80, 100, 50, 50)
    
    REM Draw player
    DRAWCIRCLE(player_x, player_y, 15, 0, 255, 0)
    DRAWTEXT("P", player_x - 5, player_y - 10, 20, 255, 255, 255)
    
    REM Draw target
    DRAWCIRCLE(target_x, target_y, 10, 255, 255, 0)
    DRAWTEXT("T", target_x - 5, target_y - 10, 16, 0, 0, 0)
    
    REM Draw instructions
    DRAWTEXT("Navigation System Demo", 10, 10, 24, 255, 255, 255)
    DRAWTEXT("WASD: Move player", 10, 40, 18, 200, 200, 200)
    DRAWTEXT("Mouse: Set target", 10, 65, 18, 200, 200, 200)
    DRAWTEXT("SPACE: Find path", 10, 90, 18, 200, 200, 200)
    
    IF path_found THEN
        DRAWTEXT("Path found! Following waypoints...", 10, 120, 18, 0, 255, 0)
    ELSE
        DRAWTEXT("Press SPACE to find path", 10, 120, 18, 255, 255, 0)
    ENDIF
    
    DRAWTEXT("Distance to target: " + STR(INT(NAVDISTANCE(player_x, player_y, target_x, target_y))), 10, 150, 16, 255, 255, 255)
    
    ENDDRAW()
WEND

CLOSEWINDOW()
PRINT "Navigation demo completed!"
