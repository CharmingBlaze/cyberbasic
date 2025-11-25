n = 7
SELECT CASE n
  CASE IS < 0
    PRINT "neg"
  CASE 1,2,3
    PRINT "small"
  CASE IS >= 10
    PRINT "big"
  CASE ELSE
    PRINT "mid"
END SELECT
