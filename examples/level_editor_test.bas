REM Level Editor System Test
REM Tests the Level Editor system without asset loading

PRINT "=== Level Editor System Test ==="

REM Test Level Editor System
PRINT "Testing Level Editor System..."
CONST LEVEL_WIDTH = 1000
CONST LEVEL_HEIGHT = 1000

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

REM Test level properties
SETLEVELPROPERTY(level_id, "theme", "forest")
SETLEVELPROPERTY(level_id, "difficulty", "easy")
PRINT "Level theme: " + GETLEVELPROPERTY(level_id, "theme")
PRINT "Level difficulty: " + GETLEVELPROPERTY(level_id, "difficulty")

REM Test level saving
PRINT "Testing level saving..."
VAR save_success = SAVELEVEL(level_id, "test_level.lvl")
PRINT "Level save result: " + STR(save_success)

REM Cleanup
PRINT "Cleaning up..."
DESTROYLEVEL(level_id)

PRINT "=== Level Editor Test Completed Successfully! ==="
