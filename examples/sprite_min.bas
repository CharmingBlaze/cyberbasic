REM Minimal sprite demo
REM Ensure you have an image file, e.g. player.png, in project root

SETWINDOWSIZE 640, 360
sid = LOADIMAGE("player.png")
sp = CREATESPRITE(sid)
SETSPRITESIZE sp, 128, 128
x = 100 : y = 100
SETSYNCRATE 60
DO
  CLS
  SETSPRITEPOSITION sp, x, y
  DRAWSPRITE sp
  TEXT 10, 10, "Use arrow keys to move (ESC to quit)"
  IF KEYDOWN(37) THEN x = x - 3
  IF KEYDOWN(39) THEN x = x + 3
  IF KEYDOWN(38) THEN y = y - 3
  IF KEYDOWN(40) THEN y = y + 3
  SYNC
UNTIL KEYDOWN(27)
