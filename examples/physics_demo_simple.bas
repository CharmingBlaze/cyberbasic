REM Simple Physics System Demo
REM Demonstrates basic physics functionality

PRINT "Initializing Physics System..."
INITPHYSICS()

PRINT "Creating physics body..."
LET body_id = CREATEPHYSICSBODY(1, 100, 100)

PRINT "Physics demo setup complete!"
PRINT "Body ID: " + STR(body_id)

PRINT "Physics demo completed successfully!"
