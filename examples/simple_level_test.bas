REM Simple Level Editor Test
REM Tests only the Level Editor system

PRINT "=== Simple Level Editor Test ==="

VAR level_id = CREATELEVEL("Simple_Test", 800, 600)
PRINT "Level created with ID: " + STR(level_id)

ADDLEVELLAYER(level_id, "background", 0)
PRINT "Added background layer"

PLACELEVELOBJECT(level_id, "player", "spawn", 100, 100, 0)
PRINT "Placed player spawn point"

PRINT "Level name: " + GETLEVELNAME(level_id)
PRINT "Level width: " + STR(GETLEVELWIDTH(level_id))
PRINT "Level height: " + STR(GETLEVELHEIGHT(level_id))

DESTROYLEVEL(level_id)
PRINT "Level destroyed"

PRINT "=== Simple Level Test Completed Successfully! ==="
