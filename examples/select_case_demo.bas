PRINT "-- SELECT CASE Demo --"
LET value = INPUT("Enter a value (1, 2, hello): ")
SELECT CASE value
CASE "1", 1
  PRINT "Got one"
CASE "2", 2
  PRINT "Got two"
CASE ELSE
  PRINT "Other: " + value
END SELECT
