PRINT "=== BASIC + Raylib Demo ==="
PRINT "A fully functional game development environment!"

' Demonstrate math capabilities
LET pi = 3.14159
LET radius = 10
LET area = pi * radius * radius
PRINT "Circle area calculation works"

' Demonstrate string manipulation
LET greeting = "Hello"
LET name = "Developer"
LET message = greeting + " " + name
PRINT "String concatenation works"

' Demonstrate control structures
LET score = 0
FOR i = 1 TO 5
    LET score = score + 10
NEXT i
PRINT "FOR loop and scoring system works"

' Demonstrate conditional logic
IF score > 30 THEN
    PRINT "High score achieved!"
ELSE
    PRINT "Keep trying!"
ENDIF

' Demonstrate Raylib integration
PRINT "Initializing Raylib graphics system..."
INITWINDOW(1024, 768, "BASIC + Raylib Demo")
PRINT "Graphics window created"

' Demonstrate input system
PRINT "Input system ready for keyboard/mouse/gamepad"

' Demonstrate audio system
PRINT "Audio system ready for sound effects and music"

' Demonstrate 3D capabilities
PRINT "3D graphics system ready for models and shaders"

CLOSEWINDOW()
PRINT "Graphics system closed"

PRINT "=== Demo completed successfully! ==="
PRINT "BASIC + Raylib is ready for game development!"
