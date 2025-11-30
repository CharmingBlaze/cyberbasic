// Test multiple includes with different syntax
INCLUDE "helper.bas"
INCLUDE math_utils.bas

PrintHelper()
PRINT "Addition:", AddNumbers(10, 5)
PRINT "Multiplication:", Multiply(4, 6)
PRINT "Square of 8:", Square(8)
PRINT "Helper constant:", helper_constant


