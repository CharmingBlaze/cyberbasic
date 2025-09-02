DIM m(2,3)
c = 0
FOR i = 0 TO 1
  FOR j = 0 TO 2
    m[i][j] = c
    c = c + 1
  NEXT
NEXT
PRINT STR(m[0][0]) + "," + STR(m[1][2])
