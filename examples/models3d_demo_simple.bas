REM Simple 3D Models Demo
REM Demonstrates basic 3D model functionality

PRINT "Initializing 3D Models System..."

PRINT "Creating 3D models..."

REM Create basic 3D shapes
LET cube_id = CREATECUBE3D("Cube", 1.0)
LET sphere_id = CREATESPHERE3D("Sphere", 0.5)

PRINT "3D models created:"
PRINT "  Cube ID: " + STR(cube_id)
PRINT "  Sphere ID: " + STR(sphere_id)

PRINT "3D models demo completed successfully!"
