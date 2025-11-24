REM AI Pong Game - CyberBasic
REM Player controls left paddle, AI controls right paddle
INITWINDOW(800, 600, "AI Pong - CyberBasic")
SETTARGETFPS(60)

REM Game constants
CONST PADDLE_SPEED = 6
CONST AI_PADDLE_SPEED = 5
CONST BALL_SPEED = 4
CONST PADDLE_WIDTH = 15
CONST PADDLE_HEIGHT = 100
CONST BALL_SIZE = 12
CONST AI_PREDICTION_OFFSET = 20

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
VAR aiPaddleColor = Color(255, 150, 100, 255)
VAR ballColor = Color(255, 220, 100, 255)
VAR textColor = Color(255, 255, 255, 255)
VAR scoreColor = Color(200, 255, 200, 255)
VAR lineColor = Color(60, 70, 90, 255)

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle player input (left paddle)
    IF ISKEYDOWN(87) THEN leftPaddleY = leftPaddleY - PADDLE_SPEED
    IF ISKEYDOWN(83) THEN leftPaddleY = leftPaddleY + PADDLE_SPEED
    
    REM Keep left paddle on screen
    IF leftPaddleY < 0 THEN leftPaddleY = 0
    IF leftPaddleY > 600 - PADDLE_HEIGHT THEN leftPaddleY = 600 - PADDLE_HEIGHT
    
    REM AI controls right paddle
    REM Simple AI: follow the ball's Y position when ball is moving towards AI
    IF ballSpeedX > 0 THEN
        REM Ball moving towards AI paddle - follow ball's Y position
        VAR paddleCenter = rightPaddleY + PADDLE_HEIGHT / 2
        VAR ballCenter = ballY + BALL_SIZE / 2
        
        REM Move towards ball center with slight offset for better gameplay
        VAR targetCenter = ballCenter + AI_PREDICTION_OFFSET
        
        REM Move paddle towards target
        IF paddleCenter < targetCenter - 5 THEN
            rightPaddleY = rightPaddleY + AI_PADDLE_SPEED
        ELSEIF paddleCenter > targetCenter + 5 THEN
            rightPaddleY = rightPaddleY - AI_PADDLE_SPEED
        ENDIF
    ELSE
        REM Ball moving away, move towards center
        VAR paddleCenter = rightPaddleY + PADDLE_HEIGHT / 2
        VAR screenCenter = 300
        
        IF paddleCenter < screenCenter - 10 THEN
            rightPaddleY = rightPaddleY + AI_PADDLE_SPEED
        ELSEIF paddleCenter > screenCenter + 10 THEN
            rightPaddleY = rightPaddleY - AI_PADDLE_SPEED
        ENDIF
    ENDIF
    
    REM Keep right paddle on screen
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
        REM Add slight angle variation based on where ball hits paddle
        VAR hitPos = (ballY - leftPaddleY) / PADDLE_HEIGHT
        ballSpeedY = ballSpeedY + (hitPos - 0.5) * 2
    ENDIF
    
    REM Ball collision with right paddle (AI)
    IF ballX + BALL_SIZE >= 750 AND ballX <= 770 AND ballY + BALL_SIZE >= rightPaddleY AND ballY <= rightPaddleY + PADDLE_HEIGHT THEN
        ballSpeedX = -ballSpeedX
        ballX = 750 - BALL_SIZE
        REM Add slight angle variation
        VAR hitPos = (ballY - rightPaddleY) / PADDLE_HEIGHT
        ballSpeedY = ballSpeedY + (hitPos - 0.5) * 2
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
    
    REM Limit ball speed
    IF ballSpeedY > 8 THEN ballSpeedY = 8
    IF ballSpeedY < -8 THEN ballSpeedY = -8
    
    REM Begin drawing
    BEGINDRAWING()
    
    REM Clear background with dark blue
    CLEARBACKGROUND(bgColor.r, bgColor.g, bgColor.b)
    
    REM Draw center line (dashed effect)
    FOR i = 0 TO 600 STEP 20
        DRAWRECTANGLE(395, i, 10, 10, lineColor.r, lineColor.g, lineColor.b)
    NEXT
    
    REM Draw paddles with rounded corners
    DRAWRECTANGLEROUNDED(30, leftPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 0.3, 8, paddleColor.r, paddleColor.g, paddleColor.b)
    DRAWRECTANGLEROUNDED(755, rightPaddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 0.3, 8, aiPaddleColor.r, aiPaddleColor.g, aiPaddleColor.b)
    
    REM Draw ball as circle
    DRAWCIRCLE(ballX + BALL_SIZE/2, ballY + BALL_SIZE/2, BALL_SIZE/2, ballColor.r, ballColor.g, ballColor.b)
    
    REM Draw scores
    DRAWTEXT(STR(leftScore), 300, 50, 60, scoreColor.r, scoreColor.g, scoreColor.b)
    DRAWTEXT(STR(rightScore), 480, 50, 60, scoreColor.r, scoreColor.g, scoreColor.b)
    
    REM Draw labels
    DRAWTEXT("PLAYER", 200, 50, 20, paddleColor.r, paddleColor.g, paddleColor.b)
    DRAWTEXT("AI", 580, 50, 20, aiPaddleColor.r, aiPaddleColor.g, aiPaddleColor.b)
    
    REM Draw instructions
    DRAWTEXT("W / S: Move", 30, 550, 18, textColor.r, textColor.g, textColor.b)
    
    REM Draw FPS
    DRAWTEXT("FPS: " + STR(GETFPS()), 10, 10, 16, textColor.r, textColor.g, textColor.b)
    
    REM End drawing
    ENDDRAWING()
WEND

CLOSEWINDOW()
PRINT "Game ended!"
PRINT "Final Score - Player: " + STR(leftScore) + " | AI: " + STR(rightScore)

