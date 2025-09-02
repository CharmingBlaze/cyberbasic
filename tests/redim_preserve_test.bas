DIM a(3)
LET a[0] = 1
LET a[1] = 2
LET a[2] = 3
REDIM PRESERVE a(5)
PRINT STR(a[0]) + "," + STR(a[1]) + "," + STR(a[2]) + "," + STR(a[3]) + "," + STR(a[4])

DIM b(2,2)
LET b[0][0] = 10
LET b[1][1] = 20
REDIM PRESERVE b(3,1)
PRINT STR(b[0][0]) + "," + STR(b[1][1]) + "," + STR(b[2][0])
