PRINT "Testing CyberBasic System"
PRINT "Hello, World!"

' Test basic arithmetic
a = 10
b = 20
c = a + b
PRINT "10 + 20 = " + STR(c)

' Test string operations
name = "CyberBasic"
PRINT "Hello, " + name + "!"

' Test arrays
DIM numbers(5)
FOR i = 0 TO 4
    numbers(i) = i * 2
NEXT i

PRINT "Array contents:"
FOR i = 0 TO 4
    PRINT "numbers(" + STR(i) + ") = " + STR(numbers(i))
NEXT i

PRINT "Basic system test completed successfully!"
