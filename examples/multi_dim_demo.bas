PRINT "-- Multi-DIM + REDIM Demo --"
DIM a(2,3)
' Initialize
FOR i = 0 TO 1
  FOR j = 0 TO 2
    a[i][j] = i*10 + j
  NEXT j
NEXT i
PRINT "a[1][2] = " + STR(a[1][2])

' REDIM to new shape (values not preserved)
REDIM a(3,2)
PRINT "After REDIM, a[1][1] = " + STR(a[1][1])
