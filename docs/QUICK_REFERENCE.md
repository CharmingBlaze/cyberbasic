# CyberBasic Quick Reference

**Quick reference for Modern BASIC game development**

## Language Syntax

### Variables
```basic
VAR x = 10              REM Modern declaration
LET y = 20              REM Classic declaration
CONST PI = 3.14159      REM Constant
```

### Arrays
```basic
DIM arr[10]             REM Classic
VAR arr[10]             REM Modern
VAR arr = [1, 2, 3]     REM Initialize
arr.length = 20         REM Resize
arr.insert(4)           REM Add element
arr.remove()            REM Remove last
arr.sort()              REM Sort
arr.find(3)             REM Search
```

### Dictionaries
```basic
VAR dict = {"key": "value", "number": 42, 2: 3}     REM JSON-style
VAR dict2 = {key = "value", number = 42}            REM BASIC-style
VAR value = dict["key"]                             REM Access value
dict["new_key"] = 100                               REM Add/update
```

### Tuples
```basic
VAR pos = (100, 200)              REM Create tuple
VAR (x, y) = pos                 REM Destructure
VAR rgb = (255, 128, 64)         REM Mixed types
PRINT pos[0]                     REM Access by index
```

### Enums
```basic
ENUM Direction
    North, South, East, West
END ENUM

ENUM Status
    IDLE = 0
    WALKING = 1
END ENUM

VAR dir = North                  REM Use enum value
```

### Coroutines
```basic
FUNCTION animate() AS coroutine
    YIELD                        REM Pause execution
END FUNCTION

WAIT(2.0)                        REM Wait 2 seconds
TIMER(1000, repeat = true) -> PRINT "Tick"
```

### ECS
```basic
COMPONENT Position {x = 0, y = 0}
VAR player = ENTITY()
player.ADD(Position(100, 200))

SYSTEM Movement(Position, Velocity)
    Position.x += Velocity.dx
END SYSTEM

RUN Movement
```

### Control Flow
```basic
IF condition THEN
    REM code
ELSEIF condition2 THEN
    REM code
ELSE
    REM code
ENDIF

WHILE condition
    REM code
WEND

FOR i = 1 TO 10
    REM code
NEXT

FOR i = 1 TO 10 STEP 2
    REM code
NEXT

SELECT CASE value
    CASE 1
        REM code
    CASE 2 TO 5
        REM code
    CASE ELSE
        REM code
ENDSELECT
```

### Functions
```basic
FUNCTION name(params)
    REM code
    RETURN value
ENDFUNCTION

FUNCTION name(params)
    REM code
END value  REM Alternative return
```

### Types
```basic
TYPE Name
    field AS TYPE
END TYPE

VAR obj = Name()
obj.field = value
```

## Game Development

### Window
```basic
INITWINDOW(width, height, "Title")
SETWINDOWICON("icon.png")      REM Set window icon
SETWINDOWTITLE("New Title")    REM Change title
SETWINDOWPOSITION(x, y)        REM Set position
SETWINDOWSIZE(width, height)   REM Resize
SETWINDOWOPACITY(0.0 to 1.0)   REM Set transparency
MAXIMIZEWINDOW()               REM Maximize
MINIMIZEWINDOW()               REM Minimize
TOGGLEFULLSCREEN()             REM Toggle fullscreen
SETTARGETFPS(60)
BEGINDRAW()
CLEARBACKGROUND(r, g, b)
REM Draw here
ENDDRAW()
CLOSEWINDOW()
```

### Input
```basic
ISKEYDOWN(KEY_W)
ISKEYPRESSED(KEY_SPACE)
MOUSEX()
MOUSEY()
ISMOUSEBUTTONPRESSED(BUTTON_LEFT)
```

### Drawing
```basic
DRAWCIRCLE(x, y, radius, r, g, b)
DRAWRECTANGLE(x, y, width, height, r, g, b)
DRAWTEXT(text, x, y, size, r, g, b)
```

### Objects
```basic
VAR pos = Vector2(x, y)
VAR color = Color(r, g, b, a)
pos.x = 100
color.r = 255
```

## Operators

### Arithmetic
```basic
+  -  *  /  MOD  ^
```

### Relational
```basic
=  !=  <>  <  <=  >  >=
```

### Boolean
```basic
AND  OR  NOT  XOR
```

### Bitwise
```basic
<<  >>  &&  ||  ~~  !
```

## Common Functions

```basic
ABS(value)
TIMER()
STR(number)
VAL(string)
LEN(array)
SQRT(value)
RND()
```

---

**See `docs/GAME_DEVELOPMENT_GUIDE.md` for complete guide**

