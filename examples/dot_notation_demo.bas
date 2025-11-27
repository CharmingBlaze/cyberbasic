REM Dot Notation Demo - CyberBasic Example
REM This example demonstrates dot notation demo functionality
REM Use WASD or arrow keys to interact (where applicable)

// Dot Notation Demo
// This example demonstrates the new dot notation features in CyberBasic

// Initialize window using dot notation
Window.init(800, 600, "Dot Notation Demo")
Window.setTargetFPS(60)

// Create objects
LET vec1 = Vector3(3, 4, 0)
LET vec2 = Vector3(1, 2, 3)
LET color = Color(255, 128, 0, 255)

// Access object properties
PRINT "Vector1: x=" + STR(vec1.x) + ", y=" + STR(vec1.y) + ", z=" + STR(vec1.z)

// Use object methods
LET len = vec1.length()
PRINT "Length of vec1: " + STR(len)

LET normalized = vec1.normalize()
PRINT "Normalized: x=" + STR(normalized.x) + ", y=" + STR(normalized.y)

LET dotProduct = vec1.dot(vec2)
PRINT "Dot product: " + STR(dotProduct)

LET crossProduct = vec1.cross(vec2)
PRINT "Cross product: x=" + STR(crossProduct.x) + ", y=" + STR(crossProduct.y)

// Color methods
LET brightness = color.brightness()
PRINT "Color brightness: " + STR(brightness)

LET darker = color.darken(0.5)
PRINT "Darker color: r=" + STR(darker.r) + ", g=" + STR(darker.g)

// Graphics namespace
Graphics.beginDrawing()
Graphics.clearBackground(Color(50, 50, 50, 255))
Graphics.drawText("Dot Notation Demo", 10, 10, 20, Color(255, 255, 255, 255))
Graphics.drawRectangle(100, 100, 200, 150, color)
Graphics.drawCircle(400, 300, 50, darker)
Graphics.endDrawing()

// Wait a bit
FOR i = 1 TO 60
    Window.getFPS() // Just to show method call
NEXT

Window.close()

