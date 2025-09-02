#pragma dialect agklite
SETWINDOWSIZE 640, 360
SETWINDOWTITLE "Input demo: KEYHIT/KEYUP/MOUSEWHEEL"
CLS
PRINT "Press SPACE to toggle, ESC to quit. Roll mouse wheel."
PRINT "Hold any key to see KEYDOWN state."

let toggled = 0

repeat
  if KEYHIT(32) then toggled = 1 - toggled : PRINT "SPACE hit: toggled = "; STR(toggled)
  if KEYUP(32) then PRINT "SPACE released"
  if KEYDOWN(27) then end

  let dw = MOUSEWHEEL()
  if dw <> 0 then PRINT "Mouse wheel: "; STR(dw)

  ' simple visual indicator using TEXT when window exists
  TEXT 10, 40, "KEYDOWN(32): "+STR(KEYDOWN(32))
  TEXT 10, 60, "KEYHIT(32): "+STR(KEYHIT(32))
  TEXT 10, 80, "KEYUP(32):  "+STR(KEYUP(32))

  SYNC
forever
