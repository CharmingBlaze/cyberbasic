REM Enhanced Pong Game - CyberBasic
INITWINDOW(800, 600, "CyberBasic Pong")
SETTARGETFPS(60)

REM Game constants
CONST PADDLE_SPEED = 6
CONST BALL_SPEED = 4
CONST PADDLE_WIDTH = 15
CONST PADDLE_HEIGHT = 100
CONST BALL_SIZE = 12

REM Game variables
VAR leftPaddleY = 250
VAR rightPaddleY = 250
VAR ballX = 400
VAR ballY = 300
VAR ballSpeedX = BALL_SPEED
VAR ballSpeedY = BALL_SPEED
VAR leftScore = 0
VAR rightScore = 0

REM Colors
VAR bgColor = Color(20, 25, 40, 255)
VAR paddleColor = Color(100, 200, 255, 255)
VAR ballColor = Color(255, 220, 100, 255)
VAR textColor = Color(255, 255, 255, 255)
VAR scoreColor = Color(200, 255, 200, 255)
VAR lineColor = Color(60, 70, 90, 255)

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle input
    IF ISKEYDOWN(87) THEN leftPaddleY = leftPaddleY - PADDLE_SPEED
    IF ISKEYDOWN(83) THEN leftPaddleY = leftPaddleY + PADDLE_SPEED
    IF ISKEYDOWN(265) THEN rightPaddleY = rightPaddleY - PADDLE_SPEED
    IF ISKEYDOWN(264) THEN rightPaddleY = rightPaddleY + PADDLE_SPEED
    
    REM Keep paddles on screen
    IF leftPaddleY < 0 THEN leftPaddleY = 0
    IF leftPaddleY > 600 - PADDLE_HEIGHT THEN leftPaddleY = 600 - PADDLE_HEIGHT
    IF rightPaddleY < 0 THEN rightPaddleY = 0
    IF rightPaddleY > 600 - PADDLE_HEIGHT THEN rightPaddleY = 600 - PADDLE_HEIGHT
    
    REM Update ball
    ballX = ballX + ballSpeedX
    ballY = ballY + ballSpeedY
    
    REM Ball bouncing off top and bottom walls
    IF ballY < 0 THEN ballSpeedY = -ballSpeedY
    IF ballY > 600 - BALL_SIZE THEN ballSpeedY = -ballSpeedY
    
    REM Ball collision with left paddle
    IF ballX <= 50 AND ballX >= 30 AND ballY + BALL_SIZE >= leftPaddleY AND ballY <= leftPaddleY + PADDLE_HEIGHT THEN
        ballSpeedX = -ballSpeedX
        ballX = 50
    ENDIF
    
    REM Ball collision with right paddle
    IF ballX + BALL_SIZE >= 750 AND ballX <= 770 AND ballY + BALL_SIZE >= rightPaddleY AND ballY <= rightPaddleY + PADDLE_HEIGHT THEN
        ballSpeedX = -ballSpeedX
        ballX = 750 - BALL_SIZE
    ENDIF
    
    REM Ball out of bounds - score and reset
    IF ballX < 0 THEN
        rightScore = rightScore + 1
        ballX = 400
        ballY = 300
        ballSpeedX = BALL_SPEED
        ballSpeedY = BALL_SPEED
    ENDIF
    
    IF ballX > 800 THEN
        leftScore = leftScore + 1
        ballX = 400
        ballY = 300
        ballSpeedX = -BALL_SPEED
        ballSpeedY = BALL_SPEED
    ENDIF
    
    REM Begin drawing
    BEGINDRAW()
    
    REM Clear background with dark blue
    CLEARBACKGROUND(bgColor.r, bgColor.g, bgColor.b)
    
    REM Draw center line (dashed effect)
    FOR i = 0 TO 600 STEP 20
        DRAWRECTANGLE(395, i, 10, 10, lineColor.r, lineColor.g, lineColor.b)
    NEXT
    
    REM Draw paddles with rounded corners
    DRAWRECTANGLEROUNDED(30, leftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 0.3, 8, paddleColor.r, paddleColor.g, paddleColor.b)
    DRAWRECTANGLEROUNDED(755, rightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 0.3, 8, paddleColor.r, paddleColor.g, paddleColor.b)
    
    REM Draw ball as circle
    DRAWCIRCLE(ballX + BALL_SIZE/2, ballY + BALL_SIZE/2, BALL_SIZE/2, ballColor.r, ballColor.g, ballColor.b)
    
    REM Draw scores
    DRAWTEXT(STR(leftScore), 300, 50, 60, scoreColor.r, scoreColor.g, scoreColor.b)
    DRAWTEXT(STR(rightScore), 480, 50, 60, scoreColor.r, scoreColor.g, scoreColor.b)
    
    REM Draw instructions
    DRAWTEXT("W / S", 30, 550, 18, textColor.r, textColor.g, textColor.b)
    DRAWTEXT("UP / DOWN", 680, 550, 18, textColor.r, textColor.g, textColor.b)
    
    REM Draw FPS
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 16, textColor.r, textColor.g, textColor.b)
    
    REM End drawing
    ENDDRAW()
WEND

CLOSEWINDOW()
PRINT "Game ended!"
