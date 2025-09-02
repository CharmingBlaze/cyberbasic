s = "beta"
SELECT CASE s
CASE "alpha", "beta"
  PRINT "group1"
CASE IS > "m"
  PRINT "late"
CASE ELSE
  PRINT "other"
END SELECT
