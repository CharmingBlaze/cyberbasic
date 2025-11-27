REM String Array Methods - CyberBasic Example
REM This example demonstrates string array methods functionality
REM Use WASD or arrow keys to interact (where applicable)

// String and Array Methods Demo

PRINT "=== String Methods ==="

VAR str = "  Hello World  "
PRINT "Original: '" + str + "'"

VAR upper = str.upper()
PRINT "Upper: '" + upper + "'"

VAR lower = str.lower()
PRINT "Lower: '" + lower + "'"

VAR trimmed = str.trim()
PRINT "Trimmed: '" + trimmed + "'"

VAR words = str.split(" ")
PRINT "Split into " + STR(words.length()) + " words:"
FOR i = 0 TO words.length() - 1
    PRINT "  [" + STR(i) + "] = '" + words[i] + "'"
NEXT

VAR contains = str.contains("World")
PRINT "Contains 'World': " + STR(contains)

PRINT ""
PRINT "=== Array Methods ==="

VAR arr = [10, 20, 30, 40, 50]
PRINT "Array: " + STR(arr)

VAR len = arr.length()
PRINT "Length: " + STR(len)

VAR first = arr.first()
PRINT "First: " + STR(first)

VAR last = arr.last()
PRINT "Last: " + STR(last)

arr.push(60)
PRINT "After push(60): length = " + STR(arr.length())

VAR popped = arr.pop()
PRINT "Popped: " + STR(popped) + ", new length: " + STR(arr.length())

PRINT ""
PRINT "String and array methods make data manipulation easy!"

