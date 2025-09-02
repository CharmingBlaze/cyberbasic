REM Audio System Demo
REM Demonstrates 3D audio, music management, and audio effects

PRINT "Initializing Audio System..."
INITAUDIO

PRINT "Creating audio sources..."

REM Create sound effects
explosion_sound = CREATESOUNDEFFECT("explosion", "sounds/explosion.wav")
jump_sound = CREATESOUNDEFFECT("jump", "sounds/jump.wav")
collect_sound = CREATESOUNDEFFECT("collect", "sounds/collect.wav")

PRINT "Sound effects created:"
PRINT "  Explosion: "; explosion_sound
PRINT "  Jump: "; jump_sound
PRINT "  Collect: "; collect_sound

REM Create music tracks
background_music = CREATEMUSICTRACK("background", "music/background.ogg")
battle_music = CREATEMUSICTRACK("battle", "music/battle.ogg")
victory_music = CREATEMUSICTRACK("victory", "music/victory.ogg")

PRINT "Music tracks created:"
PRINT "  Background: "; background_music
PRINT "  Battle: "; battle_music
PRINT "  Victory: "; victory_music

REM Create audio effects
reverb_effect = CREATEREVERBEFFECT("cave_reverb")
PRINT "Reverb effect created: "; reverb_effect

PRINT "Audio system initialized successfully!"
PRINT "Audio sources: "; GETAUDIOSOURCECOUNT
PRINT "Music tracks: "; GETMUSICTRACKCOUNT

REM Set volume levels
SETMASTERVOLUME(0.8)
SETMUSICVOLUME(0.6)
SETSFXVOLUME(0.9)

PRINT "Volume levels set:"
PRINT "  Master: 0.8"
PRINT "  Music: 0.6"
PRINT "  SFX: 0.9"

REM Main audio demo loop
PRINT "Running audio demo..."
FOR i = 1 TO 100
    REM Update audio system
    UPDATEAUDIOSYSTEM
    
    REM Play sound effects at different times
    IF i = 10 THEN
        PLAYSOUND(explosion_sound)
        PRINT "Played explosion sound"
    END IF
    
    IF i = 25 THEN
        PLAYSOUND(jump_sound)
        PRINT "Played jump sound"
    END IF
    
    IF i = 40 THEN
        PLAYSOUND(collect_sound)
        PRINT "Played collect sound"
    END IF
    
    REM Play music tracks
    IF i = 15 THEN
        PLAYMUSIC(background_music)
        PRINT "Started background music"
    END IF
    
    IF i = 50 THEN
        PLAYMUSIC(battle_music)
        PRINT "Started battle music"
    END IF
    
    IF i = 80 THEN
        PLAYMUSIC(victory_music)
        PRINT "Started victory music"
    END IF
    
    REM Simulate volume changes
    IF i = 60 THEN
        SETMUSICVOLUME(0.3)
        PRINT "Reduced music volume to 0.3"
    END IF
    
    IF i = 70 THEN
        SETMUSICVOLUME(0.8)
        PRINT "Increased music volume to 0.8"
    END IF
NEXT i

PRINT "Audio demo completed!"
PRINT "Final audio sources: "; GETAUDIOSOURCECOUNT
PRINT "Final music tracks: "; GETMUSICTRACKCOUNT
