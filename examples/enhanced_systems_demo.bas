REM Enhanced Game Development Systems Demo
REM Demonstrates Level Editor, Asset Pipeline, and Sprite Animation systems

PRINT "=== Enhanced Game Development Systems Demo ==="

REM Test Level Editor System
PRINT "Testing Level Editor System..."
CONST LEVEL_WIDTH = 1000
CONST LEVEL_HEIGHT = 1000
CONST TILE_SIZE = 32

VAR level_id = CREATELEVEL("Test_Level", LEVEL_WIDTH, LEVEL_HEIGHT)
ADDLEVELLAYER(level_id, "background", 0)
ADDLEVELLAYER(level_id, "collision", 1)
ADDLEVELLAYER(level_id, "foreground", 2)

PLACELEVELOBJECT(level_id, "tree_01", "tree", 100, 200, 0)
PLACELEVELOBJECT(level_id, "enemy_spawn", "spawn_point", 500, 300, 0)
PLACELEVELOBJECT(level_id, "player_start", "player_spawn", 50, 50, 0)

PRINT "Level created with ID: " + STR(level_id)
PRINT "Level name: " + GETLEVELNAME(level_id)
PRINT "Level size: " + STR(GETLEVELWIDTH(level_id)) + "x" + STR(GETLEVELHEIGHT(level_id))

REM Test Asset Pipeline System
PRINT "Testing Asset Pipeline System..."
CONST ASSET_QUALITY = 0.8
CONST AUDIO_BITRATE = 128

VAR texture_id = LOADASSETTEXTURE("sprites/player.png")
VAR model_id = LOADASSETMODEL("models/character.obj")
VAR sound_id = LOADASSETSOUND("audio/jump.wav")

PRINT "Loaded assets:"
PRINT "  Texture ID: " + STR(texture_id)
PRINT "  Model ID: " + STR(model_id)
PRINT "  Sound ID: " + STR(sound_id)

PRINT "Asset filenames:"
PRINT "  Texture: " + GETASSETFILENAME(texture_id)
PRINT "  Model: " + GETASSETFILENAME(model_id)
PRINT "  Sound: " + GETASSETFILENAME(sound_id)

PRINT "Testing asset optimization..."
VAR batch_success = BATCHTEXTURES("sprites/", "atlases/player_atlas.png")
VAR optimize_success = OPTIMIZEMODEL(model_id, ASSET_QUALITY)
VAR compress_success = COMPRESSAUDIO(sound_id, "mp3", AUDIO_BITRATE)

PRINT "Optimization results:"
PRINT "  Texture batching: " + STR(batch_success)
PRINT "  Model optimization: " + STR(optimize_success)
PRINT "  Audio compression: " + STR(compress_success)

REM Test Sprite & Animation System
PRINT "Testing Sprite & Animation System..."
VAR sprite_id = CREATESPRITE("player", texture_id, 32, 32)
SETSPRITEORIGIN(sprite_id, 16, 16)
SETSPRITEPOSITION(sprite_id, 100, 100, 0)
SETSPRITEROTATION(sprite_id, 0.5)
SETSPRITESCALE(sprite_id, 2.0, 2.0)
SETSPRITEVISIBLE(sprite_id, TRUE)

PRINT "Sprite created with ID: " + STR(sprite_id)
PRINT "Sprite position: (" + STR(GETSPRITEX(sprite_id)) + ", " + STR(GETSPRITEY(sprite_id)) + ", " + STR(GETSPRITEZ(sprite_id)) + ")"

VAR anim_id = CREATEANIMATION("player_walk", sprite_id)
ADDANIMATIONFRAME(anim_id, 0, 0, 32, 32, 0.1)
ADDANIMATIONFRAME(anim_id, 1, 0, 32, 32, 0.1)
ADDANIMATIONFRAME(anim_id, 2, 0, 32, 32, 0.1)
SETANIMATIONLOOP(anim_id, TRUE)

PRINT "Animation created with ID: " + STR(anim_id)
PRINT "Playing animation..."
PLAYANIMATION(sprite_id, anim_id)

PRINT "Updating animations..."
UPDATEANIMATIONS(0.016)

PRINT "Stopping animation..."
STOPANIMATION(sprite_id)

REM Test level saving
PRINT "Testing level saving..."
VAR save_success = SAVELEVEL(level_id, "test_level.lvl")
PRINT "Level save result: " + STR(save_success)

REM Cleanup
PRINT "Cleaning up..."
DESTROYSPRITE(sprite_id)
DESTROYANIMATION(anim_id)
DESTROYLEVEL(level_id)
UNLOADASSET(texture_id)
UNLOADASSET(model_id)
UNLOADASSET(sound_id)

PRINT "=== Enhanced Systems Demo Completed Successfully! ==="
