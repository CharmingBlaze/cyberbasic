REM Minimal Game Loop - CyberBasic Example
REM This example demonstrates minimal game loop functionality
REM Use WASD or arrow keys to interact (where applicable)

INITWINDOW(800, 450, "Minimal Game Loop")
SETTARGETFPS(60)
LET x = 100
LET y = 200
LET vx = 2
WHILE NOT WINDOWSHOULDCLOSE()
  LET x = x + vx
  IF x > 750 THEN LET vx = -2 ENDIF
  IF x < 50 THEN LET vx = 2 ENDIF
  BEGINDRAWING()
  CLEARBACKGROUND(30, 30, 46)
  DRAWRECTANGLE(x-50, y-25, 100, 50, 0, 199, 140)
  DRAWTEXT("Press ESC to exit", 10, 10, 20, 255, 255, 255)
  ENDDRAWING()
WEND
CLOSEWINDOW()
