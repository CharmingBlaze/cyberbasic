REM Space Invaders - CyberBasic
REM Enhanced classic arcade game

INITWINDOW(800, 600, "Space Invaders - CyberBasic")
SETTARGETFPS(60)

REM Game constants
VAR PLAYER_SPEED = 5
VAR BULLET_SPEED = 10
VAR ENEMY_SPEED = 1
VAR ENEMY_DROP = 30
VAR ENEMY_ROWS = 5
VAR ENEMY_COLS = 10
VAR ENEMY_WIDTH = 40
VAR ENEMY_HEIGHT = 30
VAR PLAYER_WIDTH = 50
VAR PLAYER_HEIGHT = 30
VAR BULLET_SIZE = 4
VAR ENEMY_SHOOT_COOLDOWN = 90  REM Frames between enemy shots
VAR ENEMY_SHOOT_CHANCE = 0.003  REM Lower chance per frame

REM Game state
VAR gameState = 0  REM 0=playing, 1=game over, 2=win, 3=start screen
VAR score = 0
VAR lives = 3
VAR wave = 1
VAR enemySpeedMultiplier = 1.0

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
VAR enemyShootTimer = 0  REM Global cooldown for enemy shooting

REM Enemy positions (using arrays)
DIM enemyX(ENEMY_ROWS * ENEMY_COLS)
DIM enemyY(ENEMY_ROWS * ENEMY_COLS)
DIM enemyAlive(ENEMY_ROWS * ENEMY_COLS)
DIM enemyLastShot(ENEMY_ROWS * ENEMY_COLS)  REM Track when each enemy last shot

REM Enemy bullets (increased from 5 to 10)
DIM enemyBulletX(10)
DIM enemyBulletY(10)
DIM enemyBulletActive(10)

REM Initialize enemies
VAR enemyStartX = 100
VAR enemyStartY = 50
VAR enemyIndex = 0

FOR row = 0 TO ENEMY_ROWS - 1
    FOR col = 0 TO ENEMY_COLS - 1
        enemyX[enemyIndex] = enemyStartX + col * (ENEMY_WIDTH + 10)
        enemyY[enemyIndex] = enemyStartY + row * (ENEMY_HEIGHT + 10)
        enemyAlive[enemyIndex] = 1
        enemyLastShot[enemyIndex] = -1000  REM Initialize to allow shooting
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
VAR winR = 0
VAR winG = 255
VAR winB = 0

REM Game loop
WHILE NOT WINDOWSHOULDCLOSE()
    IF gameState = 3 THEN
        REM Start screen
        IF ISKEYPRESSED(32) THEN  REM Spacebar to start
            gameState = 0
        ENDIF
    ELSEIF gameState = 0 THEN
        REM Handle player input
        IF ISKEYDOWN(65) THEN playerX = playerX - PLAYER_SPEED  REM A key
        IF ISKEYDOWN(68) THEN playerX = playerX + PLAYER_SPEED  REM D key
        
        REM Keep player on screen
        IF playerX < 0 THEN playerX = 0
        IF playerX > 800 - PLAYER_WIDTH THEN playerX = 800 - PLAYER_WIDTH
        
        REM Player shooting (use ISKEYPRESSED to prevent continuous shooting)
        IF ISKEYPRESSED(32) AND playerBulletActive = 0 THEN  REM Spacebar
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
        VAR currentMoveDelay = INT(enemyMoveDelay / enemySpeedMultiplier)
        IF currentMoveDelay < 5 THEN currentMoveDelay = 5  REM Minimum speed
        
        IF enemyMoveTimer >= currentMoveDelay THEN
            enemyMoveTimer = 0
            
            VAR shouldDrop = 0
            VAR minX = 1000
            VAR maxX = -1000
            
            REM Find leftmost and rightmost alive enemies
            FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
                IF enemyAlive[i] = 1 THEN
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
            VAR currentEnemySpeed = INT(ENEMY_SPEED * enemySpeedMultiplier)
            IF currentEnemySpeed < 1 THEN currentEnemySpeed = 1
            FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
                IF enemyAlive[i] = 1 THEN
                    VAR ex = enemyX[i]
                    VAR ey = enemyY[i]
                    enemyX[i] = ex + (currentEnemySpeed * enemyDirection)
                    IF shouldDrop = 1 THEN
                        enemyY[i] = ey + ENEMY_DROP
                    ENDIF
                ENDIF
            NEXT
        ENDIF
        
        REM Enemy shooting (improved logic)
        enemyShootTimer = enemyShootTimer + 1
        IF enemyShootTimer >= ENEMY_SHOOT_COOLDOWN THEN
            VAR randomChance = RND(1.0)
            IF randomChance < ENEMY_SHOOT_CHANCE THEN
                REM Find bottom-most alive enemies (they shoot)
                DIM bottomEnemies(ENEMY_COLS)
                VAR bottomCount = 0
                
                REM For each column, find the bottom-most alive enemy
                FOR col = 0 TO ENEMY_COLS - 1
                    VAR bottomIndex = -1
                    VAR bottomY = -1
                    FOR row = ENEMY_ROWS - 1 TO 0 STEP -1
                        VAR idx = row * ENEMY_COLS + col
                        IF enemyAlive[idx] = 1 THEN
                            VAR ey = enemyY[idx]
                            IF ey > bottomY THEN
                                bottomY = ey
                                bottomIndex = idx
                            ENDIF
                        ENDIF
                    NEXT
                    IF bottomIndex >= 0 THEN
                        bottomEnemies[bottomCount] = bottomIndex
                        bottomCount = bottomCount + 1
                    ENDIF
                NEXT
                
                REM Pick a random bottom enemy to shoot
                IF bottomCount > 0 THEN
                    VAR randomEnemy = INT(RND(1.0) * bottomCount)
                    VAR shooterIndex = bottomEnemies[randomEnemy]
                    
                    REM Check if this enemy can shoot (cooldown)
                    VAR frameCount = enemyShootTimer
                    IF frameCount - enemyLastShot[shooterIndex] >= ENEMY_SHOOT_COOLDOWN THEN
                        VAR sx = enemyX[shooterIndex]
                        VAR sy = enemyY[shooterIndex]
                        
                        REM Find an available bullet slot
                        VAR foundSlot = 0
                        VAR bulletSlot = 0
                        WHILE bulletSlot < 10 AND foundSlot = 0
                            IF enemyBulletActive[bulletSlot] = 0 THEN
                                enemyBulletX[bulletSlot] = sx + ENEMY_WIDTH / 2
                                enemyBulletY[bulletSlot] = sy + ENEMY_HEIGHT
                                enemyBulletActive[bulletSlot] = 1
                                enemyLastShot[shooterIndex] = frameCount
                                foundSlot = 1
                            ENDIF
                            bulletSlot = bulletSlot + 1
                        WEND
                    ENDIF
                ENDIF
                enemyShootTimer = 0  REM Reset cooldown
            ENDIF
        ENDIF
        
        REM Update enemy bullets
        FOR i = 0 TO 9
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
                    REM Better collision detection (center-based)
                    VAR bulletCenterX = playerBulletX
                    VAR bulletCenterY = playerBulletY + BULLET_SIZE
                    IF bulletCenterX >= ex AND bulletCenterX <= ex + ENEMY_WIDTH AND bulletCenterY >= ey AND bulletCenterY <= ey + ENEMY_HEIGHT THEN
                        enemyAlive[i] = 0
                        playerBulletActive = 0
                        enemiesLeft = enemiesLeft - 1
                        
                        REM Score based on row (top rows worth more)
                        VAR row = INT(i / ENEMY_COLS)
                        VAR points = (ENEMY_ROWS - row) * 10
                        score = score + points
                        
                        REM Speed up enemies as they're destroyed
                        VAR alivePercent = enemiesLeft / (ENEMY_ROWS * ENEMY_COLS)
                        enemySpeedMultiplier = 1.0 + (1.0 - alivePercent) * 1.5
                        
                        REM Check win condition
                        IF enemiesLeft <= 0 THEN
                            gameState = 2
                        ENDIF
                    ENDIF
                ENDIF
            NEXT
        ENDIF
        
        REM Collision: Enemy bullets vs Player
        FOR i = 0 TO 9
            IF enemyBulletActive[i] = 1 THEN
                VAR bulletCenterX = enemyBulletX[i]
                VAR bulletCenterY = enemyBulletY[i] + BULLET_SIZE
                IF bulletCenterX >= playerX AND bulletCenterX <= playerX + PLAYER_WIDTH AND bulletCenterY >= playerY AND bulletCenterY <= playerY + PLAYER_HEIGHT THEN
                    enemyBulletActive[i] = 0
                    lives = lives - 1
                    IF lives <= 0 THEN
                        gameState = 1
                    ELSE
                        REM Brief invincibility - reset player position
                        playerX = 375
                        playerBulletActive = 0
                    ENDIF
                ENDIF
            ENDIF
        NEXT
        
        REM Check if enemies reached bottom
        FOR i = 0 TO ENEMY_ROWS * ENEMY_COLS - 1
            IF enemyAlive[i] = 1 THEN
                VAR ey = enemyY[i]
                IF ey >= playerY - 10 THEN
                    gameState = 1
                ENDIF
            ENDIF
        NEXT
    ENDIF
    
    REM Reset game on R key
    IF ISKEYPRESSED(82) AND gameState <> 0 AND gameState <> 3 THEN  REM R key
        gameState = 0
        score = 0
        lives = 3
        playerX = 375
        playerY = 550
        playerBulletActive = 0
        enemyDirection = 1
        enemyMoveTimer = 0
        enemyShootTimer = 0
        enemiesLeft = ENEMY_ROWS * ENEMY_COLS
        enemySpeedMultiplier = 1.0
        wave = 1
        
        REM Reset enemies
        enemyIndex = 0
        FOR row = 0 TO ENEMY_ROWS - 1
            FOR col = 0 TO ENEMY_COLS - 1
                enemyX[enemyIndex] = enemyStartX + col * (ENEMY_WIDTH + 10)
                enemyY[enemyIndex] = enemyStartY + row * (ENEMY_HEIGHT + 10)
                enemyAlive[enemyIndex] = 1
                enemyLastShot[enemyIndex] = -1000
                enemyIndex = enemyIndex + 1
            NEXT
        NEXT
        
        REM Reset enemy bullets
        FOR i = 0 TO 9
            enemyBulletActive[i] = 0
        NEXT
    ENDIF
    
    REM Begin drawing
    BEGINDRAW()
    
    REM Clear background
    CLEARBACKGROUND(bgR, bgG, bgB)
    
    IF gameState = 3 THEN
        REM Start screen
        DRAWTEXT("SPACE INVADERS", 200, 200, 50, textR, textG, textB)
        DRAWTEXT("Press SPACE to Start", 250, 300, 24, textR, textG, textB)
        DRAWTEXT("A/D: Move | SPACE: Shoot", 220, 350, 20, textR, textG, textB)
    ELSEIF gameState = 0 THEN
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
                REM Different colors for different rows
                VAR row = INT(i / ENEMY_COLS)
                VAR r = enemyR
                VAR g = enemyG + row * 20
                VAR b = enemyB
                IF g > 255 THEN g = 255
                DRAWRECTANGLE(ex, ey, ENEMY_WIDTH, ENEMY_HEIGHT, r, g, b)
            ENDIF
        NEXT
        
        REM Draw enemy bullets
        FOR i = 0 TO 9
            IF enemyBulletActive[i] = 1 THEN
                VAR bx = enemyBulletX[i]
                VAR by = enemyBulletY[i]
                VAR halfSize = BULLET_SIZE / 2
                VAR doubleSize = BULLET_SIZE * 2
                DRAWRECTANGLE(bx - halfSize, by, BULLET_SIZE, doubleSize, enemyR, enemyG, enemyB)
            ENDIF
        NEXT
        
        REM Draw UI
        DRAWTEXT("SCORE: " + STR(score), 10, 10, 20, textR, textG, textB)
        DRAWTEXT("LIVES: " + STR(lives), 10, 35, 20, textR, textG, textB)
        DRAWTEXT("WAVE: " + STR(wave), 10, 60, 20, textR, textG, textB)
        DRAWTEXT("A/D: Move | SPACE: Shoot | R: Restart", 10, 570, 16, textR, textG, textB)
    ELSEIF gameState = 1 THEN
        REM Game Over
        DRAWTEXT("GAME OVER", 250, 250, 40, gameOverR, gameOverG, gameOverB)
        DRAWTEXT("Final Score: " + STR(score), 280, 300, 24, textR, textG, textB)
        DRAWTEXT("Press R to Restart", 250, 330, 20, textR, textG, textB)
    ELSEIF gameState = 2 THEN
        REM Victory
        DRAWTEXT("YOU WIN!", 280, 250, 40, winR, winG, winB)
        DRAWTEXT("Final Score: " + STR(score), 280, 300, 24, textR, textG, textB)
        DRAWTEXT("Press R to Play Again", 240, 330, 20, textR, textG, textB)
    ENDIF
    
    REM End drawing
    ENDDRAW()
WEND

CLOSEWINDOW()
PRINT "Game ended!"
PRINT "Final Score: " + STR(score)
