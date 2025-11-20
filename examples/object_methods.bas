// Object Methods Demo
// Demonstrates object methods like vec.length(), vec.normalize(), etc.

PRINT "=== Object Methods Demo ==="

// Vector3 methods
LET v1 = Vector3(3, 4, 0)
LET v2 = Vector3(1, 1, 1)

PRINT "Vector 1: (" + STR(v1.x) + ", " + STR(v1.y) + ", " + STR(v1.z) + ")"
PRINT "Vector 2: (" + STR(v2.x) + ", " + STR(v2.y) + ", " + STR(v2.z) + ")"

LET len1 = v1.length()
PRINT "Length of v1: " + STR(len1)

LET len2 = v2.length()
PRINT "Length of v2: " + STR(len2)

LET norm1 = v1.normalize()
PRINT "Normalized v1: (" + STR(norm1.x) + ", " + STR(norm1.y) + ", " + STR(norm1.z) + ")"

LET dot = v1.dot(v2)
PRINT "Dot product v1·v2: " + STR(dot)

LET cross = v1.cross(v2)
PRINT "Cross product v1×v2: (" + STR(cross.x) + ", " + STR(cross.y) + ", " + STR(cross.z) + ")"

LET dist = v1.distance(v2)
PRINT "Distance between v1 and v2: " + STR(dist)

// Vector2 methods
LET v2d1 = Vector2(3, 4)
LET v2d2 = Vector2(1, 1)

PRINT ""
PRINT "2D Vector 1: (" + STR(v2d1.x) + ", " + STR(v2d1.y) + ")"
PRINT "2D Vector 2: (" + STR(v2d2.x) + ", " + STR(v2d2.y) + ")"

LET len2d1 = v2d1.length()
PRINT "Length of 2D v1: " + STR(len2d1)

LET norm2d1 = v2d1.normalize()
PRINT "Normalized 2D v1: (" + STR(norm2d1.x) + ", " + STR(norm2d1.y) + ")"

LET dot2d = v2d1.dot(v2d2)
PRINT "2D Dot product: " + STR(dot2d)

// Color methods
LET col = Color(255, 128, 64, 255)
PRINT ""
PRINT "Color: r=" + STR(col.r) + ", g=" + STR(col.g) + ", b=" + STR(col.b)

LET bright = col.brightness()
PRINT "Brightness: " + STR(bright)

LET dark = col.darken(0.3)
PRINT "Darkened (30%): r=" + STR(dark.r) + ", g=" + STR(dark.g) + ", b=" + STR(dark.b)

PRINT ""
PRINT "Demo complete!"

