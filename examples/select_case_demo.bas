REM SELECT CASE Demo
REM Demonstrates the SELECT CASE statement for multi-way branching

PRINT "-- SELECT CASE Demo --"
PRINT "Enter a number (1, 2, or 3):"

REM For demo purposes, we'll use a fixed value
REM In a real program, you would use INPUT() if available
VAR value = 2

SELECT CASE value
    CASE 1
        PRINT "Got one"
    CASE 2
        PRINT "Got two"
    CASE 3
        PRINT "Got three"
    CASE ELSE
        PRINT "Other: " + STR(value)
END SELECT

PRINT ""
PRINT "Testing with string-like values:"
VAR test_value = "hello"

SELECT CASE test_value
    CASE "hello"
        PRINT "Got hello"
    CASE "world"
        PRINT "Got world"
    CASE ELSE
        PRINT "Other: " + test_value
END SELECT

PRINT ""
PRINT "Demo completed!"
