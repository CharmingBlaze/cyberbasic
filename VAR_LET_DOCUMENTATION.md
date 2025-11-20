# VAR vs LET - Variable Declarations

## Overview

CyberBasic supports both `VAR` and `LET` for variable declarations. They work **identically** - use whichever you prefer!

## Syntax

```basic
// Both work the same way
LET x = 10
VAR y = 20

// Both can be used for any type
LET name = "CyberBasic"
VAR count = 100
LET vec = Vector3(1, 2, 3)
VAR color = Color(255, 0, 0, 255)
```

## When to Use Which?

### Use `LET` if:
- You prefer traditional BASIC style
- You're porting old BASIC code
- You want classic syntax

### Use `VAR` if:
- You prefer modern syntax (similar to JavaScript, C#, etc.)
- You want shorter, more concise code
- You're coming from modern languages

## Examples

### Basic Usage
```basic
// Traditional style
LET x = 10
LET y = 20
LET sum = x + y

// Modern style
VAR a = 10
VAR b = 20
VAR result = a + b

// Mix them - both work!
LET traditional = 1
VAR modern = 2
LET combined = traditional + modern
```

### With Objects
```basic
// Using LET
LET vec1 = Vector3(1, 2, 3)
LET len = vec1.length()

// Using VAR
VAR vec2 = Vector3(4, 5, 6)
VAR dist = vec1.distance(vec2)
```

### In Functions
```basic
FUNCTION addNumbers(a, b)
    LET result = a + b
    RETURN result
ENDFUNCTION

FUNCTION multiplyNumbers(a, b)
    VAR product = a * b
    RETURN product
ENDFUNCTION
```

## Technical Details

- Both `LET` and `VAR` are parsed identically
- Both create the same type of assignment statement
- No performance difference
- Both work with all data types (numbers, strings, objects, arrays)
- Both support indexed assignments: `LET arr[0] = 10` or `VAR arr[0] = 10`

## Backward Compatibility

- `LET` is the traditional BASIC keyword (always supported)
- `VAR` is a modern alternative (added for convenience)
- All existing code using `LET` continues to work
- You can mix `LET` and `VAR` in the same program

## Error Messages

Error messages mention both options:
```
Assignment: expected variable name
Make sure you have a valid variable name
LET x = 10 or VAR x = 10
```

## Recommendation

Use whichever style you prefer! Many developers use:
- `VAR` for new code (modern style)
- `LET` when porting old BASIC code (traditional style)

Both are equally valid and work identically! 🎉

