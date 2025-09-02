REM Test new features: DRAWTRIANGLE and SPLIT
PRINT "Testing new features..."

INITWINDOW(800, 600, "New Features Test")
SETTARGETFPS(60)

LET running = TRUE
WHILE running
  BEGINDRAW()
  
  CLEARBACKGROUND(20, 20, 40)
  
  REM Test DRAWTRIANGLE
  DRAWTRIANGLE(400, 100, 300, 200, 500, 200, 255, 0, 0)
  DRAWTRIANGLE(400, 300, 350, 400, 450, 400, 0, 255, 0)
  
  REM Test SPLIT function
  LET test_string = "Hello,World,Test"
  LET pos1 = SPLIT(test_string, ",")
  LET pos2 = SPLIT(test_string, "World")
  LET pos3 = SPLIT(test_string, "XYZ")
  
  DRAWTEXT("SPLIT Test Results:", 10, 10, 20, 255, 255, 255)
  DRAWTEXT("String: " + test_string, 10, 40, 16, 200, 200, 200)
  DRAWTEXT("Position of ',': " + STR(pos1), 10, 70, 16, 255, 255, 255)
  DRAWTEXT("Position of 'World': " + STR(pos2), 10, 100, 16, 255, 255, 255)
  DRAWTEXT("Position of 'XYZ': " + STR(pos3), 10, 130, 16, 255, 255, 255)
  
  DRAWTEXT("Press ESC to exit", 10, 550, 18, 200, 200, 200)
  
  ENDDRAW()
  
  REM Exit on ESC
  IF ISKEYPRESSED(256) THEN LET running = FALSE ENDIF
WEND

CLOSEWINDOW()

PRINT "New features test completed!"
PRINT "SPLIT results:"
PRINT "  ',' found at: " + STR(pos1)
PRINT "  'World' found at: " + STR(pos2)
PRINT "  'XYZ' found at: " + STR(pos3)
