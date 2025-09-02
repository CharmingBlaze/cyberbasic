INITWINDOW(800, 600, "Simple Test")
SETTARGETFPS(60)

LET counter = 0
LET running = TRUE

WHILE running AND NOT WINDOWSHOULDCLOSE()
    LET counter = counter + 1
    
    IF counter > 100 THEN
        LET running = FALSE
    ENDIF
    
    BEGINDRAW()
    CLEARBACKGROUND(50, 50, 100)
    DRAWTEXT("Counter: " + STR(counter), 10, 10, 30, 255, 255, 255, 255)
    ENDDRAW()
    
    IF ISKEYPRESSED(256) THEN LET running = FALSE ENDIF
WEND

CLOSEWINDOW()
