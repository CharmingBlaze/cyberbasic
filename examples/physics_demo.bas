REM Physics Demo - CyberBasic Example
REM This example demonstrates physics demo functionality
REM Use WASD or arrow keys to interact (where applicable)

INITWINDOW(800, 600, "Physics System Demo")
SETTARGETFPS(60)

REM Initialize physics system
INITPHYSICS()
SETPHYSICSGRAVITY(0, 500)

REM Create physics bodies
LET ground = CREATEPHYSICSBODY(0, 400, 300)  REM Static ground
SETPHYSICSRECTANGLESHAPE(ground, 800, 100)

LET box1 = CREATEPHYSICSBODY(1, 200, 100)    REM Dynamic box
SETPHYSICSRECTANGLESHAPE(box1, 50, 50)
SETPHYSICSBODYMASS(box1, 1.0)

LET box2 = CREATEPHYSICSBODY(1, 300, 50)     REM Another dynamic box
SETPHYSICSRECTANGLESHAPE(box2, 40, 40)
SETPHYSICSBODYMASS(box2, 0.8)

LET ball = CREATEPHYSICSBODY(1, 500, 100)    REM Dynamic ball
SETPHYSICSCIRCLESHAPE(ball, 25)
SETPHYSICSBODYMASS(ball, 0.5)
SETPHYSICSBODYRESTITUTION(ball, 0.8)

REM Create spring joint between boxes
LET spring = CREATEPHYSICSSPRINGJOINT(box1, box2, 100, 10)

REM Game variables
LET spawn_timer = 0
LET spawn_interval = 60

WHILE NOT WINDOWSHOULDCLOSE()
    REM Handle input
    IF ISKEYPRESSED(32) THEN  REM Space to spawn new box
        LET new_box = CREATEPHYSICSBODY(1, 100, 50)
        SETPHYSICSRECTANGLESHAPE(new_box, 30, 30)
        SETPHYSICSBODYMASS(new_box, 0.6)
    ENDIF
    
    IF ISKEYPRESSED(69) THEN  REM E to spawn new ball
        LET new_ball = CREATEPHYSICSBODY(1, 600, 50)
        SETPHYSICSCIRCLESHAPE(new_ball, 20)
        SETPHYSICSBODYMASS(new_ball, 0.4)
        SETPHYSICSBODYRESTITUTION(new_ball, 0.9)
    ENDIF
    
    IF ISKEYPRESSED(82) THEN  REM R to reset
        REM Remove all bodies except ground
        REM (In a real implementation, you'd track body IDs)
        PRINT "Reset key pressed (not implemented)"
    ENDIF
    
    REM Apply forces with mouse
    IF ISMOUSEBUTTONPRESSED(0) THEN
        LET mouse_x = GETMOUSEX()
        LET mouse_y = GETMOUSEY()
        APPLYPHYSICSFORCE(box1, (mouse_x - 200) * 2, (mouse_y - 100) * 2)
    ENDIF
    
    REM Auto-spawn objects
    LET spawn_timer = spawn_timer + 1
    IF spawn_timer >= spawn_interval THEN
        LET spawn_timer = 0
        LET auto_box = CREATEPHYSICSBODY(1, 50, 50)
        SETPHYSICSRECTANGLESHAPE(auto_box, 25, 25)
        SETPHYSICSBODYMASS(auto_box, 0.5)
    ENDIF
    
    REM Step physics simulation
    PHYSICSSTEP()
    
    REM Drawing
    BEGINDRAWING()
    CLEARBACKGROUND(20, 20, 40)
    
    REM Draw ground
    DRAWRECTANGLE(0, 400, 800, 100, 100, 100, 100)
    DRAWTEXT("Ground", 10, 420, 16, 255, 255, 255)
    
    REM Draw physics bodies (simplified - in real implementation you'd get positions from physics system)
    DRAWRECTANGLE(175, 75, 50, 50, 0, 150, 255)   REM box1
    DRAWTEXT("Box1", 180, 90, 12, 255, 255, 255)
    
    DRAWRECTANGLE(280, 25, 40, 40, 255, 150, 0)   REM box2
    DRAWTEXT("Box2", 285, 40, 12, 255, 255, 255)
    
    DRAWCIRCLE(500, 75, 25, 255, 0, 150)          REM ball
    DRAWTEXT("Ball", 490, 85, 12, 255, 255, 255)
    
    REM Draw instructions
    DRAWTEXT("Physics System Demo", 10, 10, 24, 255, 255, 255)
    DRAWTEXT("SPACE: Spawn box", 10, 40, 16, 200, 200, 200)
    DRAWTEXT("E: Spawn ball", 10, 60, 16, 200, 200, 200)
    DRAWTEXT("R: Reset", 10, 80, 16, 200, 200, 200)
    DRAWTEXT("Mouse: Apply force to box1", 10, 100, 16, 200, 200, 200)
    
    DRAWTEXT("Objects auto-spawn every second", 10, 130, 14, 150, 150, 150)
    DRAWTEXT("Spring joint connects Box1 and Box2", 10, 150, 14, 150, 150, 150)
    
    ENDDRAWING()
WEND

CLOSEWINDOW()
PRINT "Physics demo completed!"
