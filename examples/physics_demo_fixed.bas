REM Physics System Demo
REM Demonstrates basic physics functionality

PRINT "Initializing Physics System..."
INITPHYSICS()

PRINT "Creating physics world..."
LET world_id = CREATEPHYSICSWORLD()

PRINT "Creating ground plane..."
LET ground = CREATEPHYSICSBODY(world_id, 0, 0, 0)
SETPHYSICSBODYTYPE(ground, 0)  REM Static body

PRINT "Creating physics ball..."
LET ball = CREATEPHYSICSBODY(world_id, 100, 100, 0)
SETPHYSICSBODYTYPE(ball, 1)  REM Dynamic body

PRINT "Physics demo setup complete!"
PRINT "World ID: " + STR(world_id)
PRINT "Ground body: " + STR(ground)
PRINT "Ball body: " + STR(ball)

PRINT "Physics demo completed successfully!"
