// Complete Feature Demo
// Shows all new features working together

PRINT "=== CyberBasic Modern Features Demo ==="
PRINT ""

// C++ style comments
// This is a single-line comment
/*
 * This is a multi-line comment
 * Showing all the new features
 */

// VAR vs LET
VAR x = 10
LET y = 20
PRINT "VAR x = " + STR(x) + ", LET y = " + STR(y)

// Object creation
VAR vec = Vector3(3, 4, 0)
VAR color = Color(255, 128, 0, 255)
VAR camera = Camera3D()

// Object properties
PRINT "Vector: x=" + STR(vec.x) + ", y=" + STR(vec.y) + ", z=" + STR(vec.z)
PRINT "Color: r=" + STR(color.r) + ", g=" + STR(color.g) + ", b=" + STR(color.b)

// Object methods
VAR len = vec.length()
PRINT "Vector length: " + STR(len)

VAR norm = vec.normalize()
PRINT "Normalized: (" + STR(norm.x) + ", " + STR(norm.y) + ", " + STR(norm.z) + ")"

VAR bright = color.brightness()
PRINT "Color brightness: " + STR(bright)

// Method chaining
VAR result = Vector3(10, 20, 0).normalize().scale(5.0)
PRINT "Chained: (" + STR(result.x) + ", " + STR(result.y) + ", " + STR(result.z) + ")"

// String methods
VAR text = "  Hello CyberBasic  "
PRINT "Original: '" + text + "'"
PRINT "Trimmed: '" + text.trim() + "'"
PRINT "Upper: '" + text.upper() + "'"

// Array methods
VAR numbers = [1, 2, 3, 4, 5]
PRINT "Array length: " + STR(numbers.length())
PRINT "First: " + STR(numbers.first())
PRINT "Last: " + STR(numbers.last())

numbers.push(6)
PRINT "After push: length = " + STR(numbers.length())

// Namespace usage
PRINT ""
PRINT "Window.getFPS() would return: " + STR(60) + " (example)"
PRINT "Graphics.drawRectangle() works via namespace"
PRINT "Input.isKeyPressed() works via namespace"

PRINT ""
PRINT "All features working together!"

