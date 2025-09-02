DIM a(3)
LET a[0] = 1
LET a[1] = 2
LET a[2] = 3
REDIM PRESERVE a(5)
PRINT STR(a[0]) + "," + STR(a[1]) + "," + STR(a[2]) + "," + STR(a[3]) + "," + STR(a[4])
