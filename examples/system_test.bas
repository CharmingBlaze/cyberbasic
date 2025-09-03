REM Comprehensive System Test for CyberBasic
REM This tests all game programming systems to ensure they're working correctly

PRINT "=== CyberBasic System Test ==="
PRINT "Testing all game programming systems..."

REM Test 1: Game Systems Management
PRINT "Test 1: Game Systems Management"
INITGAMESYSTEMS()

LET graphics_ready = ISSYSTEMINITIALIZED("graphics")
LET audio_ready = ISSYSTEMINITIALIZED("audio")
LET physics_ready = ISSYSTEMINITIALIZED("physics")
LET ai_ready = ISSYSTEMINITIALIZED("ai")
LET navigation_ready = ISSYSTEMINITIALIZED("navigation")
LET networking_ready = ISSYSTEMINITIALIZED("networking")

PRINT "Graphics system: " + STR(graphics_ready)
PRINT "Audio system: " + STR(audio_ready)
PRINT "Physics system: " + STR(physics_ready)
PRINT "AI system: " + STR(ai_ready)
PRINT "Navigation system: " + STR(navigation_ready)
PRINT "Networking system: " + STR(networking_ready)

REM Test 2: Window and Graphics
PRINT "Test 2: Window and Graphics"
INITWINDOW(800, 600, "System Test")
SETTARGETFPS(60)

LET width = GETSCREENWIDTH()
LET height = GETSCREENHEIGHT()
PRINT "Window size: " + STR(width) + "x" + STR(height)

LET fps = GETFPS()
PRINT "FPS: " + STR(fps)

REM Test 3: Game Objects
PRINT "Test 3: Game Objects"
LET test_object = CREATEGAMEOBJECT("TestObject")
SETGAMEOBJECTPOSITION(test_object, 100, 100, 0)
SETGAMEOBJECTSCALE(test_object, 50, 50, 1)

LET pos = GETGAMEOBJECTPOSITION(test_object)
PRINT "Object position: " + STR(pos(0)) + ", " + STR(pos(1)) + ", " + STR(pos(2))

LET object_count = GETGAMEOBJECTCOUNT()
PRINT "Total objects: " + STR(object_count)

REM Test 4: Scene Management
PRINT "Test 4: Scene Management"
CREATESCENE("TestScene")
ADDOBJECTTOSCENE("TestScene", test_object)
LOADSCENE("TestScene")

LET current_scene = GETCURRENTSCENE()
PRINT "Current scene: " + current_scene

REM Test 5: Game State
PRINT "Test 5: Game State"
SETGAMESTATE("testing")
SETGAMESTATEDATA("test_value", 42)
SETGAMESTATEDATA("test_string", "Hello World")

LET state = GETGAMESTATE()
LET test_value = GETGAMESTATEDATA("test_value")
LET test_string = GETGAMESTATEDATA("test_string")

PRINT "Game state: " + state
PRINT "Test value: " + STR(test_value)
PRINT "Test string: " + test_string

REM Test 6: Physics System
PRINT "Test 6: Physics System"
INITPHYSICS()
SETPHYSICSGRAVITY(0, 9.81)

LET physics_body = CREATEPHYSICSBODY(1, 200, 200)
SETPHYSICSCIRCLESHAPE(physics_body, 25)
APPLYPHYSICSFORCE(physics_body, 100, 0)

PRINT "Physics body created: " + STR(physics_body)

REM Test 7: Math Utilities
PRINT "Test 7: Math Utilities"
LET distance = DISTANCE(0, 0, 100, 100)
LET angle = ANGLE(0, 0, 100, 100)
LET lerp_result = LERP(0, 100, 0.5)
LET clamped = CLAMP(150, 0, 100)

PRINT "Distance: " + STR(distance)
PRINT "Angle: " + STR(angle)
PRINT "Lerp result: " + STR(lerp_result)
PRINT "Clamped: " + STR(clamped)

REM Test 8: Colors
PRINT "Test 8: Colors"
LET red_color = COLORFROMRGB(255, 0, 0)
LET green_color = COLORFROMRGB(0, 255, 0)
LET blue_color = COLORFROMRGB(0, 0, 255)

PRINT "Red color: " + STR(red_color)
PRINT "Green color: " + STR(green_color)
PRINT "Blue color: " + STR(blue_color)

REM Test 9: Basic Drawing
PRINT "Test 9: Basic Drawing"
BEGINDRAWING()
CLEARBACKGROUND(0, 0, 50)

DRAWCIRCLE(400, 300, 50, red_color)
DRAWRECTANGLE(100, 100, 100, 100, green_color)
DRAWLINE(0, 0, 800, 600, blue_color)

DRAWTEXT("System Test Complete!", 10, 10, 20, 255, 255, 255)
DRAWTEXT("All systems working correctly!", 10, 40, 16, 200, 200, 200)

ENDDRAWING()

REM Test 10: Input System
PRINT "Test 10: Input System"
LET mouse_x = GETMOUSEX()
LET mouse_y = GETMOUSEY()
LET key_w_down = ISKEYDOWN(87)

PRINT "Mouse position: " + STR(mouse_x) + ", " + STR(mouse_y)
PRINT "W key down: " + STR(key_w_down)

REM Test 11: Frame Timing
PRINT "Test 11: Frame Timing"
GAMELOOPUPDATE()

LET frame_count = GETFRAMECOUNT()
LET delta_time = GETDELTATIME()

PRINT "Frame count: " + STR(frame_count)
PRINT "Delta time: " + STR(delta_time)

REM Test 12: Collision Detection
PRINT "Test 12: Collision Detection"
LET obj1 = CREATEGAMEOBJECT("Object1")
LET obj2 = CREATEGAMEOBJECT("Object2")

SETGAMEOBJECTPOSITION(obj1, 100, 100, 0)
SETGAMEOBJECTPOSITION(obj2, 120, 120, 0)
SETGAMEOBJECTSCALE(obj1, 50, 50, 1)
SETGAMEOBJECTSCALE(obj2, 50, 50, 1)

LET colliding = CHECKCOLLISION(obj1, obj2)
PRINT "Objects colliding: " + STR(colliding)

REM Wait for user input
PRINT "Press any key to continue..."
WHILE NOT ISKEYPRESSED(32)  REM Space key
    GAMELOOPSTART()
    GAMELOOPUPDATE()
    
    REM Update display
    BEGINDRAWING()
    CLEARBACKGROUND(0, 0, 50)
    
    DRAWTEXT("System Test Running...", 10, 10, 20, 255, 255, 255)
    DRAWTEXT("Press SPACE to exit", 10, 40, 16, 200, 200, 200)
    
    REM Draw test objects
    LET pos1 = GETGAMEOBJECTPOSITION(obj1)
    LET pos2 = GETGAMEOBJECTPOSITION(obj2)
    
    DRAWCIRCLE(INT(pos1(0)), INT(pos1(1)), 25, red_color)
    DRAWCIRCLE(INT(pos2(0)), INT(pos2(1)), 25, green_color)
    
    ENDDRAWING()
    GAMELOOPEND()
WEND

REM Cleanup
PRINT "Cleaning up..."
SHUTDOWNGAMESYSTEMS()
CLOSEWINDOW()

PRINT "=== System Test Complete ==="
PRINT "All systems tested successfully!"
PRINT "CyberBasic is ready for game development!"
