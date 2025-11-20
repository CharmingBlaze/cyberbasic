// VAR vs LET Demo
// Both VAR and LET work identically for variable declarations

// Using LET (traditional BASIC style)
LET x = 10
LET y = 20
LET name = "CyberBasic"

// Using VAR (modern style - same as LET)
VAR a = 100
VAR b = 200
VAR greeting = "Hello"

// Both work the same way
PRINT "LET variables: x=" + STR(x) + ", y=" + STR(y) + ", name=" + name
PRINT "VAR variables: a=" + STR(a) + ", b=" + STR(b) + ", greeting=" + greeting

// You can mix them
LET mixed1 = 1
VAR mixed2 = 2
LET mixed3 = mixed1 + mixed2
PRINT "Mixed: " + STR(mixed3)

// Both work with objects
LET vec1 = Vector3(1, 2, 3)
VAR vec2 = Vector3(4, 5, 6)
LET sum = vec1.dot(vec2)
PRINT "Dot product: " + STR(sum)

// Both work with assignments
LET counter = 0
VAR maxCount = 10

WHILE counter < maxCount
    counter = counter + 1
    PRINT "Count: " + STR(counter)
WEND

PRINT "Both VAR and LET work identically!"

