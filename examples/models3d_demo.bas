REM 3D Model Management System Demo
REM This demonstrates the 3D model, material, and animation systems

PRINT "=== 3D Model Management System Demo ==="

REM Initialize the model system
PRINT "Creating 3D models..."

REM Create some basic 3D shapes
LET cube_id = CREATECUBE3D("MyCube", 2.0)
LET sphere_id = CREATESPHERE3D("MySphere", 1.5)

PRINT "Created cube with ID: ", cube_id
PRINT "Created sphere with ID: ", sphere_id

REM Position the models
SETMODEL3DPOSITION cube_id, 0, 0, 0
SETMODEL3DPOSITION sphere_id, 5, 0, 0

PRINT "Positioned models"

REM Create materials
LET metal_material = CREATEMATERIAL3D("Metal", "PBR")
LET plastic_material = CREATEMATERIAL3D("Plastic", "PBR")

PRINT "Created materials with IDs: ", metal_material, " and ", plastic_material

REM Set material colors
SETMATERIAL3DCOLOR metal_material, 0.8, 0.8, 0.9, 1.0
SETMATERIAL3DCOLOR plastic_material, 0.2, 0.8, 0.2, 1.0

PRINT "Set material colors"

REM Apply materials to models
SETMODEL3DMATERIAL cube_id, metal_material
SETMODEL3DMATERIAL sphere_id, plastic_material

PRINT "Applied materials to models"

REM Create animations
LET rotation_anim = CREATEANIMATION3D("Rotate", "ROTATION")
LET scale_anim = CREATEANIMATION3D("Scale", "SCALE")

PRINT "Created animations with IDs: ", rotation_anim, " and ", scale_anim

REM Set up rotation animation for the cube
REM Note: In a real implementation, we would add keyframes here
PRINT "Set up animations"

REM Make models visible
SETMODEL3DVISIBLE cube_id, 1
SETMODEL3DVISIBLE sphere_id, 1

PRINT "Made models visible"

REM Get model positions
LET cube_pos = GETMODEL3DPOSITION cube_id
PRINT "Cube position: ", cube_pos

LET sphere_pos = GETMODEL3DPOSITION sphere_id
PRINT "Sphere position: ", sphere_pos

REM Get model count
LET model_count = GETMODELS3DCOUNT
PRINT "Total models: ", model_count

REM Animation loop
PRINT "Starting animation loop..."
LET time = 0.0
LET delta_time = 0.016

FOR i = 1 TO 100
    REM Update the model system
    UPDATEMODELS3D delta_time
    
    REM Render models
    RENDERMODEL3D cube_id
    RENDERMODEL3D sphere_id
    
    REM Simple rotation animation
    LET rotation_y = time * 45.0
    SETMODEL3DROTATION cube_id, 0, rotation_y, 0
    
    REM Simple scale animation
    LET scale_factor = 1.0 + 0.3 * SIN(time * 2.0)
    SETMODEL3DSCALE sphere_id, scale_factor, scale_factor, scale_factor
    
    LET time = time + delta_time
    
    REM Print progress every 20 frames
    IF i MOD 20 = 0 THEN
        PRINT "Frame ", i, " - Time: ", time
    END IF
NEXT i

PRINT "Animation loop completed"

REM Clean up
UNLOADMODEL3D cube_id
UNLOADMODEL3D sphere_id

PRINT "Cleaned up models"
PRINT "3D Model Management System Demo completed!"
