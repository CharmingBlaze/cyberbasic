INITWINDOW(800, 600, "Math Utilities Test")
SETTARGETFPS(60)

LET running = TRUE

WHILE running AND NOT WINDOWSHOULDCLOSE()
    BEGINDRAW()
    
    CLEARBACKGROUND(30, 30, 50)
    
    REM Test math utility functions
    LET test_value = -42.5
    LET abs_result = ABS(test_value)
    LET min_result = MIN(10, 20)
    LET max_result = MAX(10, 20)
    LET clamp_result = CLAMP(15, 10, 20)
    LET clamp_low = CLAMP(5, 10, 20)
    LET clamp_high = CLAMP(25, 10, 20)
    
    REM Display results
    DRAWTEXT("Math Utilities Test", 10, 10, 30, 255, 255, 255, 255)
    DRAWTEXT("ABS(-42.5) = " + STR(abs_result), 10, 50, 20, 255, 255, 255, 255)
    DRAWTEXT("MIN(10, 20) = " + STR(min_result), 10, 80, 20, 255, 255, 255, 255)
    DRAWTEXT("MAX(10, 20) = " + STR(max_result), 10, 110, 20, 255, 255, 255, 255)
    DRAWTEXT("CLAMP(15, 10, 20) = " + STR(clamp_result), 10, 140, 20, 255, 255, 255, 255)
    DRAWTEXT("CLAMP(5, 10, 20) = " + STR(clamp_low), 10, 170, 20, 255, 255, 255, 255)
    DRAWTEXT("CLAMP(25, 10, 20) = " + STR(clamp_high), 10, 200, 20, 255, 255, 255, 255)
    
    REM Test with time-based values
    LET current_time = GETTIME()
    LET time_abs = ABS(SIN(current_time))
    LET time_min = MIN(SIN(current_time), COS(current_time))
    LET time_max = MAX(SIN(current_time), COS(current_time))
    LET time_clamp = CLAMP(SIN(current_time), -0.5, 0.5)
    
    DRAWTEXT("Time-based tests:", 10, 250, 20, 255, 255, 255, 255)
    DRAWTEXT("ABS(SIN(time)) = " + STR(time_abs), 10, 280, 18, 255, 255, 255, 255)
    DRAWTEXT("MIN(SIN, COS) = " + STR(time_min), 10, 310, 18, 255, 255, 255, 255)
    DRAWTEXT("MAX(SIN, COS) = " + STR(time_max), 10, 340, 18, 255, 255, 255, 255)
    DRAWTEXT("CLAMP(SIN, -0.5, 0.5) = " + STR(time_clamp), 10, 370, 18, 255, 255, 255, 255)
    
    REM Draw some visual examples
    LET x = 400
    LET y = 300
    LET size = 50 + (time_abs * 30)
    
    DRAWCIRCLE(x, y, size, 255, 255, 0)
    DRAWTEXT("Dynamic Circle", x - 40, y + size + 10, 16, 255, 255, 255, 255)
    
    REM Draw a line that changes length based on CLAMP
    LET line_length = CLAMP(time_clamp * 200, 50, 150)
    DRAWLINES(x - 100, y + 100, x - 100 + line_length, y + 100, 0, 255, 255)
    DRAWTEXT("Clamped Line", x - 100, y + 120, 16, 255, 255, 255, 255)
    
    REM Instructions
    DRAWTEXT("Press ESC to exit", 10, 550, 18, 200, 200, 200, 255)
    
    ENDDRAW()
    
    IF ISKEYPRESSED(256) THEN LET running = FALSE ENDIF
WEND

CLOSEWINDOW()

PRINT "Math utilities test completed!"
PRINT "ABS(-42.5) = " + STR(ABS(-42.5))
PRINT "MIN(10, 20) = " + STR(MIN(10, 20))
PRINT "MAX(10, 20) = " + STR(MAX(10, 20))
PRINT "CLAMP(15, 10, 20) = " + STR(CLAMP(15, 10, 20))
