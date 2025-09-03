REM Simple Pong Game
PRINT "Starting Pong Game..."

REM Initialize window
INITWINDOW(800, 600, "CyberBasic Pong")

REM Set target FPS
SETTARGETFPS(60)

REM Game variables
LET leftPaddleY = 250
LET rightPaddleY = 250
LET ballX = 400
LET ballY = 300
LET ballSpeedX = 3
LET ballSpeedY = 3
LET leftScore = 0
LET rightScore = 0

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle input
    IF ISKEYHELD(87) THEN LET leftPaddleY = leftPaddleY - 5
    IF ISKEYHELD(83) THEN LET leftPaddleY = leftPaddleY + 5
    IF ISKEYHELD(265) THEN LET rightPaddleY = rightPaddleY - 5
    IF ISKEYHELD(264) THEN LET rightPaddleY = rightPaddleY + 5
    
    REM Keep paddles on screen
    IF leftPaddleY < 0 THEN LET leftPaddleY = 0
    IF leftPaddleY > 500 THEN LET leftPaddleY = 500
    IF rightPaddleY < 0 THEN LET rightPaddleY = 0
    IF rightPaddleY > 500 THEN LET rightPaddleY = 500
    
    REM Update ball
    LET ballX = ballX + ballSpeedX
    LET ballY = ballY + ballSpeedY
    
    REM Ball bouncing off top and bottom walls
    IF ballY < 0 THEN LET ballSpeedY = -ballSpeedY
    IF ballY > 590 THEN LET ballSpeedY = -ballSpeedY
    
    REM Ball collision with left paddle
    IF ballX <= 50 AND ballX >= 30 AND ballY >= leftPaddleY AND ballY <= leftPaddleY + 100 THEN LET ballSpeedX = -ballSpeedX
    IF ballX <= 50 AND ballX >= 30 AND ballY >= leftPaddleY AND ballY <= leftPaddleY + 100 THEN LET ballX = 50
    
    REM Ball collision with right paddle
    IF ballX >= 740 AND ballX <= 770 AND ballY >= rightPaddleY AND ballY <= rightPaddleY + 100 THEN LET ballSpeedX = -ballSpeedX
    IF ballX >= 740 AND ballX <= 770 AND ballY >= rightPaddleY AND ballY <= rightPaddleY + 100 THEN LET ballX = 740
    
    REM Ball out of bounds - reset to center
    IF ballX < 0 THEN LET ballX = 400
    IF ballX < 0 THEN LET ballY = 300
    IF ballX < 0 THEN LET ballSpeedX = 3
    IF ballX < 0 THEN LET ballSpeedY = 3
    
    IF ballX > 800 THEN LET ballX = 400
    IF ballX > 800 THEN LET ballY = 300
    IF ballX > 800 THEN LET ballSpeedX = -3
    IF ballX > 800 THEN LET ballSpeedY = 3
    
    REM Begin drawing
    BEGINDRAW()
    
    REM Clear background
    CLEARBACKGROUND(0, 0, 0)
    
    REM Draw paddles
    DRAWRECTANGLE(30, leftPaddleY, 20, 100, 255, 255, 255)
    DRAWRECTANGLE(750, rightPaddleY, 20, 100, 255, 255, 255)
    
    REM Draw ball
    DRAWRECTANGLE(ballX, ballY, 10, 10, 255, 255, 255)
    
    REM Draw instructions
    DRAWTEXT("W/S", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("UP/DOWN", 700, 10, 20, 255, 255, 255)
    
    REM End drawing
    ENDDRAW()
WEND

REM Close window
CLOSEWINDOW()

PRINT "Game ended!"
