INITWINDOW(800, 600, "AI System Demo")
SETTARGETFPS(60)

REM Initialize AI system
INITAISYSTEM()

REM Create AI agents
LET player = CREATEAIAGENT("Player")
SETAIAGENTPOSITION(player, 400, 300)
SETAIAGENTSPEED(player, 150)

LET enemy1 = CREATEAIAGENT("Enemy1")
SETAIAGENTPOSITION(enemy1, 100, 100)
SETAIAGENTSPEED(enemy1, 80)

LET enemy2 = CREATEAIAGENT("Enemy2")
SETAIAGENTPOSITION(enemy2, 700, 100)
SETAIAGENTSPEED(enemy2, 90)

LET enemy3 = CREATEAIAGENT("Enemy3")
SETAIAGENTPOSITION(enemy3, 400, 500)
SETAIAGENTSPEED(enemy3, 70)

REM Create behavior trees
LET patrol_behavior = CREATEPATROLBEHAVIOR(100, 100, 700, 100)
LET chase_behavior = CREATEPATROLBEHAVIOR(400, 300, 400, 300)  REM Simplified chase

REM Assign behaviors to agents
SETAIAGENTBEHAVIOR(enemy1, "patrol")
SETAIAGENTBEHAVIOR(enemy2, "patrol")
SETAIAGENTBEHAVIOR(enemy3, "chase")

REM Game variables
LET game_time = 0
LET score = 0
LET player_health = 100
LET max_health = 100

WHILE NOT WINDOWSHOULDCLOSE()
    REM Update game time
    LET game_time = game_time + 1
    
    REM Handle player input
    IF ISKEYDOWN(87) THEN
        LET current_x = GETAIAGENTPOSITION(player)
        LET current_y = GETAIAGENTPOSITION(player)  REM Simplified - would need separate x/y functions
        SETAIAGENTPOSITION(player, current_x, current_y - 2)
    ENDIF
    IF ISKEYDOWN(83) THEN
        LET current_x = GETAIAGENTPOSITION(player)
        LET current_y = GETAIAGENTPOSITION(player)
        SETAIAGENTPOSITION(player, current_x, current_y + 2)
    ENDIF
    IF ISKEYDOWN(65) THEN
        LET current_x = GETAIAGENTPOSITION(player)
        LET current_y = GETAIAGENTPOSITION(player)
        SETAIAGENTPOSITION(player, current_x - 2, current_y)
    ENDIF
    IF ISKEYDOWN(68) THEN
        LET current_x = GETAIAGENTPOSITION(player)
        LET current_y = GETAIAGENTPOSITION(player)
        SETAIAGENTPOSITION(player, current_x + 2, current_y)
    ENDIF
    
    REM Set player target with mouse
    IF ISMOUSEBUTTONPRESSED(0) THEN
        LET mouse_x = GETMOUSEX()
        LET mouse_y = GETMOUSEY()
        SETAIAGENTTARGET(player, mouse_x, mouse_y)
    ENDIF
    
    REM Update AI system
    UPDATEAISYSTEM(1.0/60.0)
    
    REM Simple collision detection (distance-based)
    LET player_x = GETAIAGENTPOSITION(player)
    LET player_y = GETAIAGENTPOSITION(player)
    
    LET enemy1_x = GETAIAGENTPOSITION(enemy1)
    LET enemy1_y = GETAIAGENTPOSITION(enemy1)
    LET dist1 = NAVDISTANCE(player_x, player_y, enemy1_x, enemy1_y)
    
    LET enemy2_x = GETAIAGENTPOSITION(enemy2)
    LET enemy2_y = GETAIAGENTPOSITION(enemy2)
    LET dist2 = NAVDISTANCE(player_x, player_y, enemy2_x, enemy2_y)
    
    LET enemy3_x = GETAIAGENTPOSITION(enemy3)
    LET enemy3_y = GETAIAGENTPOSITION(enemy3)
    LET dist3 = NAVDISTANCE(player_x, player_y, enemy3_x, enemy3_y)
    
    REM Check collisions
    IF dist1 < 30 THEN
        LET player_health = player_health - 1
        LET score = score + 10
    ENDIF
    IF dist2 < 30 THEN
        LET player_health = player_health - 1
        LET score = score + 10
    ENDIF
    IF dist3 < 30 THEN
        LET player_health = player_health - 2
        LET score = score + 20
    ENDIF
    
    REM Keep health in bounds
    IF player_health < 0 THEN
        LET player_health = 0
    ENDIF
    IF player_health > max_health THEN
        LET player_health = max_health
    ENDIF
    
    REM Drawing
    BEGINDRAW()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw player
    DRAWCIRCLE(player_x, player_y, 20, 0, 255, 0)
    DRAWTEXT("P", player_x - 5, player_y - 10, 20, 255, 255, 255)
    
    REM Draw enemies
    DRAWCIRCLE(enemy1_x, enemy1_y, 15, 255, 0, 0)
    DRAWTEXT("E1", enemy1_x - 8, enemy1_y - 10, 12, 255, 255, 255)
    
    DRAWCIRCLE(enemy2_x, enemy2_y, 15, 255, 100, 0)
    DRAWTEXT("E2", enemy2_x - 8, enemy2_y - 10, 12, 255, 255, 255)
    
    DRAWCIRCLE(enemy3_x, enemy3_y, 18, 255, 0, 255)
    DRAWTEXT("E3", enemy3_x - 8, enemy3_y - 10, 12, 255, 255, 255)
    
    REM Draw UI
    DRAWTEXT("AI System Demo", 10, 10, 24, 255, 255, 255)
    DRAWTEXT("WASD: Move player", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("Mouse: Set target", 10, 60, 16, 200, 200, 200)
    
    DRAWTEXT("Score: " + STR(score), 10, 90, 18, 255, 255, 0)
    DRAWTEXT("Health: " + STR(player_health) + "/" + STR(max_health), 10, 115, 18, 0, 255, 0)
    DRAWTEXT("Game Time: " + STR(game_time), 10, 140, 16, 255, 255, 255)
    
    DRAWTEXT("AI Agents: " + STR(GETAIAGENTCOUNT()), 10, 165, 16, 255, 255, 255)
    
    REM Draw behavior info
    DRAWTEXT("Enemy1: Patrol behavior", 10, 190, 14, 200, 200, 200)
    DRAWTEXT("Enemy2: Patrol behavior", 10, 210, 14, 200, 200, 200)
    DRAWTEXT("Enemy3: Chase behavior", 10, 230, 14, 200, 200, 200)
    
    REM Draw health bar
    LET health_percent = player_health / max_health
    DRAWRECTANGLE(10, 260, 200, 20, 100, 0, 0)
    DRAWRECTANGLE(10, 260, 200 * health_percent, 20, 0, 255, 0)
    DRAWTEXT("Health Bar", 10, 285, 14, 255, 255, 255)
    
    REM Game over check
    IF player_health <= 0 THEN
        DRAWTEXT("GAME OVER!", 300, 250, 40, 255, 0, 0)
        DRAWTEXT("Final Score: " + STR(score), 300, 300, 24, 255, 255, 255)
        DRAWTEXT("Press ESC to exit", 300, 330, 18, 200, 200, 200)
    ENDIF
    
    ENDDRAW()
WEND

CLOSEWINDOW()
PRINT "AI demo completed!"
PRINT "Final Score: " + STR(score)
