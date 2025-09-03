REM Simple System Test for CyberBasic
REM This tests basic functionality without complex array operations

PRINT "=== CyberBasic Simple Test ==="
PRINT "Testing basic game programming systems..."

REM Test 1: Game Systems Management
PRINT "Test 1: Game Systems Management"
INITGAMESYSTEMS()

LET graphics_ready = ISSYSTEMINITIALIZED("graphics")
LET audio_ready = ISSYSTEMINITIALIZED("audio")
LET physics_ready = ISSYSTEMINITIALIZED("physics")

PRINT "Graphics system: " + STR(graphics_ready)
PRINT "Audio system: " + STR(audio_ready)
PRINT "Physics system: " + STR(physics_ready)

REM Test 2: Window and Graphics
PRINT "Test 2: Window and Graphics"
INITWINDOW(800, 600, "Simple Test")

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

LET object_count = GETGAMEOBJECTCOUNT()
PRINT "Total objects: " + STR(object_count)

REM Test 4: Utility Functions
PRINT "Test 4: Utility Functions"
LET distance = DISTANCE(0, 0, 100, 100)
PRINT "Distance from (0,0) to (100,100): " + STR(distance)

LET angle = ANGLE(0, 0, 100, 100)
PRINT "Angle from (0,0) to (100,100): " + STR(angle)

LET lerp_result = LERP(0, 100, 0.5)
PRINT "LERP(0, 100, 0.5): " + STR(lerp_result)

REM Test 5: Game State
PRINT "Test 5: Game State"
SETGAMESTATE("playing")
LET current_state = GETGAMESTATE()
PRINT "Current game state: " + current_state

SETGAMESTATEDATA("score", 1000)
LET score = GETGAMESTATEDATA("score")
PRINT "Score: " + STR(score)

REM Test 6: Frame Count
PRINT "Test 6: Frame Count"
LET frame_count = GETFRAMECOUNT()
PRINT "Frame count: " + STR(frame_count)

PRINT "=== All tests completed successfully! ==="