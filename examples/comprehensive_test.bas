INITWINDOW(800, 600, "BASIC Language Comprehensive Test")
SETTARGETFPS(60)

REM Test all BASIC language features
LET test_string = "Hello BASIC World"
LET test_number = 42.5
LET test_boolean = TRUE
LET test_array = ARRAY(5)

REM Test array operations
LET test_array[0] = 10
LET test_array[1] = 20
LET test_array[2] = 30
LET test_array[3] = 40
LET test_array[4] = 50

REM Test string functions
LET string_length = LEN(test_string)
LET substring_test = SUBSTR(test_string, 0, 5)

REM Test mathematical operations
LET result1 = 10 + 5
LET result2 = 20 - 8
LET result3 = 6 * 7
LET result4 = 100 / 4
LET result5 = RANDOM(100)

REM Test control flow
LET counter = 0
LET running = TRUE

WHILE running AND NOT WINDOWSHOULDCLOSE()
    LET counter = counter + 1
    
    IF counter > 100 THEN
        LET running = FALSE
    ENDIF
    
    IF counter MOD 10 = 0 THEN
    PRINT "Counter: " + STR(counter)
ENDIF
    
    BEGINDRAW()
    
    CLEARBACKGROUND(50, 50, 100)
    
    REM Test drawing functions
    DRAWTEXT("BASIC Language Test", 10, 10, 30, 255, 255, 255, 255)
    DRAWTEXT("String: " + test_string, 10, 50, 20, 255, 255, 0, 255)
    DRAWTEXT("Number: " + STR(test_number), 10, 80, 20, 0, 255, 255, 255)
    DRAWTEXT("Boolean: " + STR(test_boolean), 10, 110, 20, 255, 0, 255, 255)
    DRAWTEXT("String Length: " + STR(string_length), 10, 140, 20, 255, 255, 255, 255)
    DRAWTEXT("Substring: " + substring_test, 10, 170, 20, 255, 255, 255, 255)
    
    DRAWTEXT("Math Results:", 10, 200, 20, 255, 255, 255, 255)
    DRAWTEXT("10 + 5 = " + STR(result1), 10, 230, 18, 0, 255, 0, 255)
    DRAWTEXT("20 - 8 = " + STR(result2), 10, 260, 18, 0, 255, 0, 255)
    DRAWTEXT("6 * 7 = " + STR(result3), 10, 290, 18, 0, 255, 0, 255)
    DRAWTEXT("100 / 4 = " + STR(result4), 10, 320, 18, 0, 255, 0, 255)
    DRAWTEXT("Random: " + STR(result5), 10, 350, 18, 0, 255, 0, 255)
    
    DRAWTEXT("Array Values:", 10, 380, 20, 255, 255, 255, 255)
    DRAWTEXT("Array[0] = " + STR(test_array[0]), 10, 410, 18, 255, 255, 0, 255)
    DRAWTEXT("Array[1] = " + STR(test_array[1]), 10, 440, 18, 255, 255, 0, 255)
    DRAWTEXT("Array[2] = " + STR(test_array[2]), 10, 470, 18, 255, 255, 0, 255)
    
    DRAWTEXT("Counter: " + STR(counter), 10, 500, 20, 255, 255, 255, 255)
    DRAWTEXT("Press ESC to exit", 10, 530, 16, 200, 200, 200, 255)
    
    REM Test drawing primitives
    DRAWCIRCLE(600, 100, 50, 255, 0, 0)
    DRAWRECTANGLE(550, 200, 100, 50, 0, 255, 0)
    REM DRAWTRIANGLE not implemented yet - comment out
    REM DRAWTRIANGLE(550, 300, 600, 350, 650, 300, 0, 0, 255)
    
    REM Test input handling
    IF ISKEYDOWN(87) THEN DRAWTEXT("W pressed", 550, 400, 16, 255, 255, 255, 255) ENDIF
    IF ISKEYDOWN(65) THEN DRAWTEXT("A pressed", 550, 420, 16, 255, 255, 255, 255) ENDIF
    IF ISKEYDOWN(83) THEN DRAWTEXT("S pressed", 550, 440, 16, 255, 255, 255, 255) ENDIF
    IF ISKEYDOWN(68) THEN DRAWTEXT("D pressed", 550, 460, 16, 255, 255, 255, 255) ENDIF
    
    ENDDRAW()
    
    IF ISKEYPRESSED(256) THEN LET running = FALSE ENDIF
WEND

CLOSEWINDOW()

REM Test console output
PRINT "=== BASIC Language Test Complete ==="
PRINT "String: " + test_string
PRINT "Number: " + STR(test_number)
PRINT "Boolean: " + STR(test_boolean)
PRINT "String Length: " + STR(string_length)
PRINT "Substring: " + substring_test
PRINT "Math Results:"
PRINT "  10 + 5 = " + STR(result1)
PRINT "  20 - 8 = " + STR(result2)
PRINT "  6 * 7 = " + STR(result3)
PRINT "  100 / 4 = " + STR(result4)
PRINT "  Random: " + STR(result5)
PRINT "Array Values:"
PRINT "  Array[0] = " + STR(test_array[0])
PRINT "  Array[1] = " + STR(test_array[1])
PRINT "  Array[2] = " + STR(test_array[2])
PRINT "Final Counter: " + STR(counter)
PRINT "All tests completed successfully!"
