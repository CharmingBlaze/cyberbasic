REM Smooth Animation Demo - CyberBasic Example
REM This example demonstrates smooth animation demo functionality
REM Use WASD or arrow keys to interact (where applicable)

PRINT "Starting Smooth Animation Demo..."
PRINT "Use WASD to move the player around"
PRINT "Watch the smooth animations and effects!"

INITWINDOW(800, 600, "CyberBasic Smooth Animation Demo")
SETTARGETFPS(60)

REM Variables for smooth animation
LET player_x = 400.0
LET player_y = 300.0
LET player_speed = 300.0
LET running = TRUE
LET score = 0
LET particles = 0

REM Get start time for animation
LET start_time = GETTIME()

WHILE running AND NOT WINDOWSHOULDCLOSE()
    REM Get delta time for smooth movement
    LET delta_time = GETDELTATIME()
    
    REM Handle input with smooth movement
    IF ISKEYDOWN(87) THEN LET player_y = player_y - player_speed * delta_time
    IF ISKEYDOWN(83) THEN LET player_y = player_y + player_speed * delta_time
    IF ISKEYDOWN(65) THEN LET player_x = player_x - player_speed * delta_time
    IF ISKEYDOWN(68) THEN LET player_x = player_x + player_speed * delta_time
    
    REM Increase score over time
    LET score = score + delta_time * 10
    
    REM Keep player on screen
    IF player_x < 25 THEN LET player_x = 25
    IF player_x > 775 THEN LET player_x = 775
    IF player_y < 25 THEN LET player_y = 25
    IF player_y > 575 THEN LET player_y = 575
    
    REM Calculate animation values
    LET current_time = GETTIME()
    LET animation_offset = SIN(current_time * 2) * 20
    LET color_pulse = ABS(SIN(current_time * 3)) * 255
    
    BEGINDRAWING()
    
    REM Clear with animated background
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw animated background elements
    LET bg_time = current_time * 0.5
    FOR i = 0 TO 10
        LET x = (i * 80) + (SIN(bg_time + i) * 30)
        LET y = 100 + (COS(bg_time + i * 0.7) * 50)
        LET size = 20 + (SIN(bg_time * 2 + i) * 10)
        DRAWCIRCLE(x, y, size, 50 + i * 20, 100 + i * 10, 150 + i * 20)
    NEXT
    
    REM Draw player with smooth movement
    DRAWCIRCLE(player_x, player_y, 25, 255, 255, 0)
    DRAWCIRCLE(player_x, player_y, 20, 255, 200, 0)
    
    REM Draw animated effects around player
    DRAWCIRCLE(player_x + animation_offset, player_y - 40, 8, 255, 100, 100)
    DRAWCIRCLE(player_x - animation_offset, player_y - 40, 8, 100, 255, 100)
    
    REM Draw UI elements
    DRAWTEXT("Smooth Animation Demo", 10, 10, 30, 255, 255, 255)
    DRAWTEXT("Use WASD to move", 10, 50, 20, 200, 200, 200)
    DRAWTEXT("Player Position: " + STR(INT(player_x)) + ", " + STR(INT(player_y)), 10, 80, 18, 255, 255)
    DRAWTEXT("Delta Time: " + STR(delta_time), 10, 110, 18, 255, 255, 255)
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 140, 18, 255, 255, 255)
    DRAWTEXT("Time: " + STR(INT(current_time)), 10, 170, 18, 255, 255, 255)
    DRAWTEXT("Score: " + STR(INT(score)), 10, 200, 18, 255, 255, 0)
    
    REM Draw animated border
    LET border_color = color_pulse
    DRAWRECTANGLE(0, 0, 800, 5, border_color, 0, 0)
    DRAWRECTANGLE(0, 595, 800, 5, border_color, 0, 0)
    DRAWRECTANGLE(0, 0, 5, 600, border_color, 0, 0)
    DRAWRECTANGLE(795, 0, 5, 600, border_color, 0, 0)
    
    REM Draw some animated lines
    FOR i = 0 TO 5
        LET line_x = 600 + (SIN(current_time + i) * 50)
        LET line_y = 200 + i * 60
        DRAWLINES(line_x, line_y, line_x + 100, line_y, 255, 255, 255)
    NEXT
    
    ENDDRAWING()
    
    REM Exit on ESC
    IF ISKEYPRESSED(256) THEN LET running = FALSE
WEND

CLOSEWINDOW()

PRINT "Smooth animation demo completed!"
PRINT "Total runtime: " + STR(GETTIME() - start_time) + " seconds"
