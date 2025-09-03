REM Demo of DIM, INPUT, arrays, and comments
PRINT "-- DIM/INPUT Demo --"
LET name = INPUT("Enter your name: ")
PRINT "Hello, " + name

DIM A(5)
A[0] = 10
A[1] = 20
A[2] = 30
PRINT "LEN(A) = " + STR(LEN(A))

' Non-mutating push returns new array
LET B = PUSH(A, 99)
PRINT "LEN(B) = " + STR(LEN(B))
PRINT "A[0] = " + STR(A[0])
PRINT "B[5] = " + STR(B[5])

# End of demo
