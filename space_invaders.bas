REM Space Invaders - CyberBasic
REM Classic arcade game clone

INITWINDOW(800, 600, "Space Invaders - CyberBasic")
SETTARGETFPS(60)

REM Game constants
CONST PLAYER_SPEED = 5
CONST BULLET_SPEED = 8
CONST ENEMY_SPEED = 1
CONST ENEMY_DROP = 30
CONST ENEMY_ROWS = 5
CONST ENEMY_COLS = 10
CONST ENEMY_WIDTH = 40
CONST ENEMY_HEIGHT = 30
CONST PLAYER_WIDTH = 50
CONST PLAYER_HEIGHT = 30
CONST BULLET_SIZE = 4

REM Game state
VAR gameState = 0  REM 0=playing, 1=game over, 2=win
VAR score = 0
VAR lives = 3

REM Player
VAR playerX = 375
VAR playerY = 550
VAR playerBulletX = -100
VAR playerBulletY = -100
VAR playerBulletActive = 0

REM Enemies
VAR enemyDirection = 1  REM 1=right, -1=left
VAR enemyMoveTimer = 0
VAR enemyMoveDelay = 30
VAR enemiesLeft = ENEMY_ROWS * ENEMY_COLS

REM Enemy positions (array simulation using individual variables for simplicity)
DIM enemyX(ENEMY_ROWS * ENEMY_COLS)
DIM enemyY(ENEMY_ROWS * ENEMY_COLS)
DIM enemyAlive(ENEMY_ROWS * ENEMY_COLS)

REM Enemy bullets
DIM enemyBulletX(5)
DIM enemyBulletY(5)
DIM enemyBulletActive(5)

REM Initialize enemies
VAR enemyStartX = 100
VAR enemyStartY = 50
VAR enemyIndex = 0

FOR row = 0 TO ENEMY_ROWS - 1
    FOR col = 0 TO ENEMY_COLS - 1
        enemyX[enemyIndex] = enemyStartX + col * (ENEMY_WIDTH + 10)
        enemyY[enemyIndex] = enemyStartY + row * (ENEMY_HEIGHT + 10)
        enemyAlive[enemyIndex] = 1
        enemyIndex = enemyIndex + 1
    NEXT
NEXT

REM Colors (RGB values)
VAR bgR = 0
VAR bgG = 0
VAR bgB = 20
VAR playerR = 0
VAR playerG = 255
VAR playerB = 0
VAR enemyR = 255
VAR enemyG = 100
VAR enemyB = 100
VAR bulletR = 255
VAR bulletG = 255
VAR bulletB = 0
VAR textR = 255
VAR textG = 255
VAR textB = 255
VAR gameOverR = 255
VAR gameOverG = 0
VAR gameOverB = 0

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    IF gameState = 0 THEN
        REM Handle player input
        IF ISKEYDOWN(65) THEN playerX = playerX - PLAYER_SPEED  REM A key
        IF ISKEYDOWN(68) THEN playerX = playerX + PLAYER_SPEED  REM D key
        
        REM Keep player on screen
        IF playerX < 0 THEN playerX = 0
        IF playerX > 800 - PLAYER_WIDTH THEN playerX = 800 - PLAYER_WIDTH
        
        REM Player shooting
        IF ISKEYDOWN(32) AND playerBulletActive = 0 THEN  REM Spacebar
            playerBulletX = playerX + PLAYER_WIDTH / 2
            playerBulletY = playerY
            playerBulletActive = 1
        ENDIF
        
        REM Update player bullet
        IF playerBulletActive = 1 THEN
            playerBulletY = playerBulletY - BULLET_SPEED
            IF playerBulletY < 0 THEN
                playerBulletActive = 0
            ENDIF
        ENDIF
        
        REM Enemy movement
        enemyMoveTimer = enemyMoveTimer + 1
        IF enemyMoveTimer >= enemyMoveDelay THEN
            enemyMoveTimer = 0
            
            VAR shouldDrop = 0
            VAR minX = 1000
            VAR maxX = -1000
            
            REM Find leftmost and rightmost alive enemies
            FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
                VAR alive = enemyAlive[i]
                IF alive = 1 THEN
                    VAR ex = enemyX[i]
                    IF ex < minX THEN minX = ex
                    IF ex > maxX THEN maxX = ex
                ENDIF
            NEXT
            
            REM Check if enemies hit edge
            IF minX <= 0 OR maxX >= 800 - ENEMY_WIDTH THEN
                enemyDirection = -enemyDirection
                shouldDrop = 1
            ENDIF
            
            REM Move all alive enemies
            FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
                VAR alive = enemyAlive[i]
                IF alive = 1 THEN
                    VAR ex = enemyX[i]
                    VAR ey = enemyY[i]
                    enemyX[i] = ex + (ENEMY_SPEED * enemyDirection)
                    IF shouldDrop = 1 THEN
                        enemyY[i] = ey + ENEMY_DROP
                    ENDIF
                ENDIF
            NEXT
        ENDIF
        
        REM Enemy shooting (random)
        VAR randomChance = RND(1.0)
        IF randomChance < 0.02 THEN
            REM Find a random alive enemy to shoot from
            VAR foundShooter = 0
            VAR attempts = 0
            WHILE attempts < 50 AND foundShooter = 0
                VAR shooterIndex = INT(RND(1.0) * ENEMY_ROWS * ENEMY_COLS)
                IF shooterIndex >= 0 AND shooterIndex < ENEMY_ROWS * ENEMY_COLS THEN
                    VAR alive = enemyAlive[shooterIndex]
                    IF alive = 1 THEN
                        VAR sx = enemyX[shooterIndex]
                        VAR sy = enemyY[shooterIndex]
                        REM Find an available bullet slot
                        VAR bulletSlot = 0
                        WHILE bulletSlot < 5 AND foundShooter = 0
                            IF enemyBulletActive[bulletSlot] = 0 THEN
                                enemyBulletX[bulletSlot] = sx + ENEMY_WIDTH / 2
                                enemyBulletY[bulletSlot] = sy + ENEMY_HEIGHT
                                enemyBulletActive[bulletSlot] = 1
                                foundShooter = 1
                            ENDIF
                            bulletSlot = bulletSlot + 1
                        WEND
                    ENDIF
                ENDIF
                attempts = attempts + 1
            WEND
        ENDIF
        
        REM Update enemy bullets
        FOR i = 0 TO 4
            IF enemyBulletActive[i] = 1 THEN
                enemyBulletY[i] = enemyBulletY[i] + BULLET_SPEED
                IF enemyBulletY[i] > 600 THEN
                    enemyBulletActive[i] = 0
                ENDIF
            ENDIF
        NEXT
        
        REM Collision: Player bullet vs Enemies
        IF playerBulletActive = 1 THEN
            FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
                IF enemyAlive[i] = 1 THEN
                    VAR ex = enemyX[i]
                    VAR ey = enemyY[i]
                    IF playerBulletX >= ex AND playerBulletX <= ex + ENEMY_WIDTH AND playerBulletY >= ey AND playerBulletY <= ey + ENEMY_HEIGHT THEN
                        enemyAlive[i] = 0
                        playerBulletActive = 0
                        enemiesLeft = enemiesLeft - 1
                        score = score + 10
                        
                        REM Check win condition
                        IF enemiesLeft <= 0 THEN
                            gameState = 2
                        ENDIF
                    ENDIF
                ENDIF
            NEXT
        ENDIF
        
        REM Collision: Enemy bullets vs Player
        FOR i = 0 TO 4
            IF enemyBulletActive[i] = 1 THEN
                IF enemyBulletX[i] >= playerX AND enemyBulletX[i] <= playerX + PLAYER_WIDTH AND enemyBulletY[i] >= playerY AND enemyBulletY[i] <= playerY + PLAYER_HEIGHT THEN
                    enemyBulletActive[i] = 0
                    lives = lives - 1
                    IF lives <= 0 THEN
                        gameState = 1
                    ENDIF
                ENDIF
            ENDIF
        NEXT
        
        REM Check if enemies reached bottom
        FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
            VAR alive = enemyAlive[i]
            IF alive = 1 THEN
                VAR ey = enemyY[i]
                IF ey >= playerY THEN
                    gameState = 1
                ENDIF
            ENDIF
        NEXT
    ENDIF
    
    REM Reset game on R key
    IF ISKEYDOWN(82) AND gameState <> 0 THEN  REM R key
        gameState = 0
        score = 0
        lives = 3
        playerX = 375
        playerY = 550
        playerBulletActive = 0
        enemyDirection = 1
        enemyMoveTimer = 0
        enemiesLeft = ENEMY_ROWS * ENEMY_COLS
        
        REM Reset enemies
        enemyIndex = 0
        FOR row = 0 TO ENEMY_ROWS - 1
            FOR col = 0 TO ENEMY_COLS - 1
                enemyX[enemyIndex] = enemyStartX + col * (ENEMY_WIDTH + 10)
                enemyY[enemyIndex] = enemyStartY + row * (ENEMY_HEIGHT + 10)
                enemyAlive[enemyIndex] = 1
                enemyIndex = enemyIndex + 1
            NEXT
        NEXT
        
        REM Reset enemy bullets
        FOR i = 0 TO 4
            enemyBulletActive[i] = 0
        NEXT
    ENDIF
    
    REM Begin drawing
    BEGINDRAW()
    
    REM Clear background
    CLEARBACKGROUND(bgR, bgG, bgB)
    
    IF gameState = 0 THEN
        REM Draw player
        DRAWRECTANGLE(playerX, playerY, PLAYER_WIDTH, PLAYER_HEIGHT, playerR, playerG, playerB)
        
        REM Draw player bullet
        IF playerBulletActive = 1 THEN
            VAR halfSize = BULLET_SIZE / 2
            VAR doubleSize = BULLET_SIZE * 2
            DRAWRECTANGLE(playerBulletX - halfSize, playerBulletY, BULLET_SIZE, doubleSize, bulletR, bulletG, bulletB)
        ENDIF
        
        REM Draw enemies
        FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
            IF enemyAlive[i] = 1 THEN
                VAR ex = enemyX[i]
                VAR ey = enemyY[i]
                IF ex <> 0 OR ey <> 0 THEN
                    DRAWRECTANGLE(ex, ey, ENEMY_WIDTH, ENEMY_HEIGHT, enemyR, enemyG, enemyB)
                ENDIF
            ENDIF
        NEXT
        
        REM Draw enemy bullets
        FOR i = 0 TO 4
            IF enemyBulletActive[i] = 1 THEN
                VAR bx = enemyBulletX[i]
                VAR by = enemyBulletY[i]
                IF bx <> 0 OR by <> 0 THEN
                    VAR halfSize = BULLET_SIZE / 2
                    VAR doubleSize = BULLET_SIZE * 2
                    DRAWRECTANGLE(bx - halfSize, by, BULLET_SIZE, doubleSize, bulletR, bulletG, bulletB)
                ENDIF
            ENDIF
        NEXT
        
        REM Draw UI
        DRAWTEXT("SCORE: " + STR(score), 10, 10, 20, textR, textG, textB)
        DRAWTEXT("LIVES: " + STR(lives), 10, 35, 20, textR, textG, textB)
        DRAWTEXT("A/D: Move | SPACE: Shoot", 10, 570, 16, textR, textG, textB)
    ELSEIF gameState = 1 THEN
        REM Game Over
        DRAWTEXT("GAME OVER", 250, 250, 40, gameOverR, gameOverG, gameOverB)
        DRAWTEXT("Final Score: " + STR(score), 280, 300, 24, textR, textG, textB)
        DRAWTEXT("Press R to Restart", 250, 330, 20, textR, textG, textB)
    ELSEIF gameState = 2 THEN
        REM Victory
        DRAWTEXT("YOU WIN!", 280, 250, 40, playerR, playerG, playerB)
        DRAWTEXT("Final Score: " + STR(score), 280, 300, 24, textR, textG, textB)
        DRAWTEXT("Press R to Play Again", 240, 330, 20, textR, textG, textB)
    ENDIF
    
    REM End drawing
    ENDDRAW()
WEND

CLOSEWINDOW()
PRINT "Game ended!"
PRINT "Final Score: " + STR(score)

