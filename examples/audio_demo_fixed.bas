REM Audio System Demo
REM Demonstrates 3D audio, music management, and audio effects

PRINT "Initializing Audio System..."
INITAUDIO()

PRINT "Creating audio sources..."

REM Create sound effects
LET explosion_sound = CREATESOUNDEFFECT("explosion", "sounds/explosion.wav")
LET jump_sound = CREATESOUNDEFFECT("jump", "sounds/jump.wav")
LET collect_sound = CREATESOUNDEFFECT("collect", "sounds/collect.wav")

PRINT "Sound effects created:"
PRINT "  Explosion: " + STR(explosion_sound)
PRINT "  Jump: " + STR(jump_sound)
PRINT "  Collect: " + STR(collect_sound)

REM Create music tracks
LET background_music = CREATEMUSICTRACK("background", "music/background.ogg")
LET battle_music = CREATEMUSICTRACK("battle", "music/battle.ogg")
LET victory_music = CREATEMUSICTRACK("victory", "music/victory.ogg")

PRINT "Music tracks created:"
PRINT "  Background: " + STR(background_music)
PRINT "  Battle: " + STR(battle_music)
PRINT "  Victory: " + STR(victory_music)

REM Set audio volumes
SETMASTERVOLUME(0.8)

PRINT "Audio system initialized successfully!"
PRINT "Demo completed!"
