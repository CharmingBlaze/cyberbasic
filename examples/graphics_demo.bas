REM Graphics System Demo
REM Demonstrates particle systems, post-processing effects, and shaders

PRINT "Initializing Graphics System..."
INITGRAPHICS

PRINT "Creating particle systems..."

REM Create fire effect
fire_id = CREATEFIREEFFECT(400, 300)
PRINT "Fire effect created with ID: "; fire_id

REM Create explosion effect
explosion_id = CREATEEXPLOSIONEFFECT(200, 200)
PRINT "Explosion effect created with ID: "; explosion_id

REM Create custom particle system (sparkles)
sparkle_id = CREATEPARTICLESYSTEM("sparkles", 5)
PRINT "Sparkle system created with ID: "; sparkle_id

REM Configure sparkle system
SETPARTICLESYSTEMPOSITION(sparkle_id, 300, 150)
SETPARTICLESYSTEMEMISSIONRATE(sparkle_id, 20)
SETPARTICLESYSTEMCOLOR(sparkle_id, 1, 1, 0, 1)

REM Create post-processing effects
blur_id = CREATEPOSTEFFECT("blur", 0)
PRINT "Blur effect created with ID: "; blur_id

bloom_id = CREATEPOSTEFFECT("bloom", 5)
PRINT "Bloom effect created with ID: "; bloom_id

PRINT "Graphics system initialized successfully!"
PRINT "Particle systems: "; GETPARTICLESYSTEMCOUNT

REM Main loop simulation
PRINT "Running graphics simulation..."
FOR i = 1 TO 100
    REM Update graphics system
    UPDATEGRAPHICSSYSTEM(0.016)
    
    REM Render graphics
    RENDERGRAPHICSSYSTEM
    
    REM Move fire effect
    IF i = 50 THEN
        SETPARTICLESYSTEMPOSITION(fire_id, 500, 400)
        PRINT "Moved fire effect to new position"
    END IF
    
    REM Trigger explosion
    IF i = 75 THEN
        explosion_id = CREATEEXPLOSIONEFFECT(600, 300)
        PRINT "Triggered explosion at position 600, 300"
    END IF
NEXT i

PRINT "Graphics demo completed!"
PRINT "Final particle systems: "; GETPARTICLESYSTEMCOUNT
