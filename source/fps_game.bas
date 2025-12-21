rem 3d fps game - cyberbasic
rem awesome first person shooter with smooth controls

initwindow(1280, 720, "3D FPS Game - CyberBasic")
settargetfps(60)

rem player state
var playerx = 0.0
var playery = 1.0
var playerz = 0.0
var playerspeed = 0.1
var playerhealth = 100
var playerammo = 30
var playerscore = 0

rem first person camera angles
var camerayaw = 0.0
var camerapitch = 0.0
var camerasensitivity = 0.3

rem mouse state
var lastmousex = 640
var lastmousey = 360
setmouseposition(640, 360)
disablecursor()

rem enemy system
var enemycount = 5
dim enemyx(10)
dim enemyy(10)
dim enemyz(10)
dim enemyhealth(10)
dim enemyactive(10)
dim enemyspeed(10)

rem initialize enemies
for i = 0 to enemycount - 1
    enemyx[i] = (rnd() - 0.5) * 20
    enemyy[i] = 1.0
    enemyz[i] = (rnd() - 0.5) * 20
    enemyhealth[i] = 100
    enemyactive[i] = 1
    enemyspeed[i] = 0.02 + rnd() * 0.03
next i

rem weapon state
var weaponcooldown = 0
var weaponfirerate = 10
var weapondamage = 25

rem projectile system
var maxprojectiles = 50
dim projectilex(50)
dim projectiley(50)
dim projectilez(50)
dim projectilevx(50)
dim projectilevy(50)
dim projectilevz(50)
dim projectileactive(50)
var projectilecount = 0
var projectilespeed = 0.5
var projectilelifetime = 200

rem game state
var gametime = 0.0
var wavenumber = 1
var enemieskilled = 0

rem lighting
var ambientlight = 0.3

print "3d fps game started!"
print "controls: w/s = move forward/back, a/d = turn, mouse = look, left click = shoot, r = reload, esc = exit"

while not windowshouldclose()
    gametime = gametime + 0.016
    
    rem handle mouse look (first person)
    var mousex = getmousex()
    var mousey = getmousey()
    var deltax = mousex - lastmousex
    var deltay = mousey - lastmousey
    
    camerayaw = camerayaw + deltax * camerasensitivity
    camerapitch = camerapitch - deltay * camerasensitivity
    
    rem clamp pitch
    if camerapitch > 89.0 then camerapitch = 89.0
    if camerapitch < -89.0 then camerapitch = -89.0
    
    lastmousex = mousex
    lastmousey = mousey
    setmouseposition(640, 360)
    
    rem handle turning (a/d keys turn left/right)
    var turnamount = 0.0
    if iskeydown(65) then turnamount = 2.0   rem a - turn left
    if iskeydown(68) then turnamount = -2.0  rem d - turn right
    
    camerayaw = camerayaw + turnamount
    
    rem handle movement (w/s move forward/backward in camera direction)
    var moveforward = 0.0
    
    if iskeydown(87) then moveforward = 1.0   rem w - move forward
    if iskeydown(83) then moveforward = -1.0  rem s - move backward
    
    rem calculate movement direction based on camera yaw
    var yawrad = camerayaw * 0.0174533
    var forwardx = sin(yawrad)
    var forwardz = cos(yawrad)
    
    rem move in the direction camera is facing
    playerx = playerx + forwardx * moveforward * playerspeed
    playerz = playerz + forwardz * moveforward * playerspeed
    
    rem keep player on ground
    playery = 1.0
    
    rem handle shooting (left mouse button) - create projectile
    if ismousebuttonpressed(1) and weaponcooldown <= 0 and playerammo > 0 then
        playerammo = playerammo - 1
        weaponcooldown = weaponfirerate
        
        rem find available projectile slot
        var projslot = -1
        var foundslot = 0
        for i = 0 to maxprojectiles - 1
            if foundslot = 0 and projectileactive[i] = 0 then
                projslot = i
                foundslot = 1
            endif
        next i
        
        rem if no slot available, reuse oldest projectile
        if projslot < 0 then
            projslot = projectilecount mod maxprojectiles
        endif
        
        rem calculate gun position (slightly in front of camera)
        var yawrad = camerayaw * 0.0174533
        var pitchrad = camerapitch * 0.0174533
        var gunoffsetx = sin(yawrad) * cos(pitchrad) * 0.5
        var gunoffsety = sin(pitchrad) * 0.5 + 0.3
        var gunoffsetz = cos(yawrad) * cos(pitchrad) * 0.5
        
        rem calculate projectile direction (camera look direction)
        var dirx = sin(yawrad) * cos(pitchrad)
        var diry = sin(pitchrad)
        var dirz = cos(yawrad) * cos(pitchrad)
        
        rem create projectile
        projectilex[projslot] = playerx + gunoffsetx
        projectiley[projslot] = playery + gunoffsety
        projectilez[projslot] = playerz + gunoffsetz
        projectilevx[projslot] = dirx * projectilespeed
        projectilevy[projslot] = diry * projectilespeed
        projectilevz[projslot] = dirz * projectilespeed
        projectileactive[projslot] = projectilelifetime
        
        projectilecount = projectilecount + 1
    endif
    
    rem update projectiles
    for i = 0 to maxprojectiles - 1
        if projectileactive[i] > 0 then
            rem move projectile
            projectilex[i] = projectilex[i] + projectilevx[i]
            projectiley[i] = projectiley[i] + projectilevy[i]
            projectilez[i] = projectilez[i] + projectilevz[i]
            
            rem decrease lifetime
            projectileactive[i] = projectileactive[i] - 1
            
            rem check collision with enemies
            for j = 0 to enemycount - 1
                if enemyactive[j] = 1 then
                    var dx = enemyx[j] - projectilex[i]
                    var dy = enemyy[j] - projectiley[i]
                    var dz = enemyz[j] - projectilez[i]
                    var dist = sqrt(dx*dx + dy*dy + dz*dz)
                    
                    if dist < 1.0 then
                        rem hit enemy!
                        enemyhealth[j] = enemyhealth[j] - weapondamage
                        projectileactive[i] = 0
                        
                        if enemyhealth[j] <= 0 then
                            enemyactive[j] = 0
                            playerscore = playerscore + 100
                            enemieskilled = enemieskilled + 1
                            
                            rem spawn new enemy
                            enemyx[j] = (rnd() - 0.5) * 30
                            enemyz[j] = (rnd() - 0.5) * 30
                            enemyhealth[j] = 100
                            enemyactive[j] = 1
                        endif
                        
                        rem break out of enemy loop by setting j to max
                        j = enemycount
                    endif
                endif
            next j
            
            rem remove projectile if too far away or lifetime expired
            var distfromplayer = sqrt((projectilex[i] - playerx)^2 + (projectiley[i] - playery)^2 + (projectilez[i] - playerz)^2)
            if distfromplayer > 50.0 or projectileactive[i] <= 0 then
                projectileactive[i] = 0
            endif
        endif
    next i
    
    rem reload
    if iskeypressed(82) then
        playerammo = 30
    endif
    
    rem update weapon cooldown
    if weaponcooldown > 0 then
        weaponcooldown = weaponcooldown - 1
    endif
    
    rem update enemies (simple ai - move toward player)
    for i = 0 to enemycount - 1
        if enemyactive[i] = 1 then
            var dx = playerx - enemyx[i]
            var dz = playerz - enemyz[i]
            var dist = sqrt(dx*dx + dz*dz)
            
            if dist > 0.1 then
                enemyx[i] = enemyx[i] + (dx / dist) * enemyspeed[i]
                enemyz[i] = enemyz[i] + (dz / dist) * enemyspeed[i]
            endif
            
            rem check collision with player
            if dist < 1.5 then
                playerhealth = playerhealth - 0.5
                if playerhealth <= 0 then
                    playerhealth = 0
                endif
            endif
        endif
    next i
    
    rem setup first person camera
    var camposx = playerx
    var camposy = playery + 0.5
    var camposz = playerz
    
    rem calculate camera target based on yaw and pitch
    var yawrad = camerayaw * 0.0174533
    var pitchrad = camerapitch * 0.0174533
    var camtargetx = playerx + sin(yawrad) * cos(pitchrad) * 10.0
    var camtargety = playery + 0.5 + sin(pitchrad) * 10.0
    var camtargetz = playerz + cos(yawrad) * cos(pitchrad) * 10.0
    
    var camera = camera3d(camposx, camposy, camposz, camtargetx, camtargety, camtargetz, 0, 1, 0, 75)
    
    rem begin drawing
    begindrawing()
    clearbackground(10, 10, 15)
    
    rem begin 3d mode with custom camera
    camera3d_begin(camera)
    
    rem draw floor
    for i = -15 to 15 step 2
        for j = -15 to 15 step 2
            var floorcolor = 30
            if (i + j) mod 4 = 0 then floorcolor = 25
            drawcube(i, 0, j, 2, 0.1, 2, floorcolor, floorcolor, floorcolor + 5)
        next j
    next i
    
    rem draw walls
    drawcube(0, 2, -15, 30, 4, 1, 40, 40, 50)
    drawcube(0, 2, 15, 30, 4, 1, 40, 40, 50)
    drawcube(-15, 2, 0, 1, 4, 30, 40, 40, 50)
    drawcube(15, 2, 0, 1, 4, 30, 40, 40, 50)
    
    rem draw projectiles
    for i = 0 to maxprojectiles - 1
        if projectileactive[i] > 0 then
            rem draw projectile as small bright cube
            drawcube(projectilex[i], projectiley[i], projectilez[i], 0.1, 0.1, 0.1, 255, 255, 100)
        endif
    next i
    
    rem draw enemies
    for i = 0 to enemycount - 1
        if enemyactive[i] = 1 then
            var enemycolorr = 255
            var enemycolorg = 50
            var enemycolorb = 50
            
            rem flash red when hit
            if enemyhealth[i] < 100 then
                enemycolorr = 255
                enemycolorg = 100 + enemyhealth[i]
                enemycolorb = 100 + enemyhealth[i]
            endif
            
            rem draw enemy cube
            drawcube(enemyx[i], enemyy[i], enemyz[i], 1, 2, 1, enemycolorr, enemycolorg, enemycolorb)
            
            rem draw enemy "eyes"
            drawcube(enemyx[i] - 0.2, enemyy[i] + 0.3, enemyz[i] + 0.6, 0.2, 0.2, 0.1, 255, 255, 0)
            drawcube(enemyx[i] + 0.2, enemyy[i] + 0.3, enemyz[i] + 0.6, 0.2, 0.2, 0.1, 255, 255, 0)
        endif
    next i
    
    rem draw player weapon (simple gun model)
    var gunx = 0.3
    var guny = -0.2
    var gunz = 0.5
    drawcube(gunx, guny, gunz, 0.1, 0.3, 0.4, 50, 50, 50)
    drawcube(gunx, guny + 0.15, gunz + 0.1, 0.15, 0.1, 0.2, 60, 60, 60)
    
    rem draw muzzle flash
    if weaponcooldown > weaponfirerate - 3 then
        drawcube(gunx, guny, gunz + 0.5, 0.15, 0.15, 0.3, 255, 255, 100)
    endif
    
    rem draw collectibles (ammo/health packs)
    for i = 0 to 3
        var packx = sin(gametime + i * 1.57) * 8
        var packz = cos(gametime + i * 1.57) * 8
        var packy = 0.5 + sin(gametime * 2 + i) * 0.2
        
        if i mod 2 = 0 then
            rem health pack
            drawcube(packx, packy, packz, 0.5, 0.5, 0.5, 0, 255, 0)
        else
            rem ammo pack
            drawcube(packx, packy, packz, 0.5, 0.5, 0.5, 255, 200, 0)
        endif
    next i
    
    camera3d_end()
    
    rem draw hud
    var hudy = 10
    
    rem health bar background
    drawrectangle(10, hudy, 200, 25, 20, 20, 20)
    var healthwidth = (playerhealth / 100.0) * 196
    var healthcolorr = 255 - (playerhealth / 100.0) * 255
    var healthcolorg = (playerhealth / 100.0) * 255
    drawrectangle(12, hudy + 2, healthwidth, 21, healthcolorr, healthcolorg, 0)
    drawtext("health: " + str(int(playerhealth)), 15, hudy + 5, 18, 255, 255, 255)
    
    rem ammo counter
    hudy = hudy + 35
    drawtext("ammo: " + str(playerammo) + " / 30", 10, hudy, 20, 255, 255, 255)
    
    rem score
    hudy = hudy + 30
    drawtext("score: " + str(playerscore), 10, hudy, 20, 255, 255, 100)
    
    rem enemies killed
    hudy = hudy + 25
    drawtext("kills: " + str(enemieskilled), 10, hudy, 18, 200, 200, 200)
    
    rem crosshair
    drawline(635, 360, 645, 360, 255, 255, 255)
    drawline(640, 355, 640, 365, 255, 255, 255)
    drawcircle(640, 360, 15, 255, 255, 255)
    
    rem weapon status
    if weaponcooldown > 0 then
        drawtext("reloading...", 600, 650, 16, 255, 200, 0)
    endif
    
    rem instructions
    drawtext("w/s: move forward/back | a/d: turn left/right | mouse: look | right click: shoot | r: reload | esc: exit", 10, 690, 14, 150, 150, 150)
    
    rem game over check
    if playerhealth <= 0 then
        drawrectangle(0, 0, 1280, 720, 0, 0, 0)
        drawtext("game over!", 500, 300, 50, 255, 0, 0)
        drawtext("final score: " + str(playerscore), 500, 360, 30, 255, 255, 255)
        drawtext("press esc to exit", 500, 410, 20, 200, 200, 200)
    endif
    
    enddrawing()
wend

enablecursor()
closewindow()

