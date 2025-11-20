# Next Improvements Priority

## Overview
This document outlines the priority order for implementing remaining features and improvements to CyberBasic.

## Recently Completed

1. **3D Arrays** - Full support for multi-dimensional arrays (1D, 2D, 3D+)
2. **Animation System** - Frame-based animations with playback control
3. **Scene/Entity System** - ECS architecture for game objects
4. **Camera System** - 2D and 3D camera management
5. **Collision System** - 2D and 3D collision detection
6. **Game Loop** - Complete game loop management
7. **Game Helpers** - Math utilities and drawing functions

## High Priority (Game Development Focus)

### 1. Function Return Types & Better Function Syntax
**Priority:** High  
**Effort:** Medium (2-3 days)  
**Impact:** High

**What it enables:**
```basic
FUNCTION add(x AS INTEGER, y AS INTEGER) AS INTEGER
    RETURN x + y
END FUNCTION
```

**Implementation:**
- Enhance `FunctionDecl` AST node to store return type
- Update parser to parse `AS Type` for return types
- Add type checking in interpreter

---

### 2. Optional Parameters & Default Values
**Priority:** High  
**Effort:** Medium (2-3 days)  
**Impact:** High

**What it enables:**
```basic
FUNCTION draw(x, y, color = Color(255, 255, 255))
    // color defaults to white if not provided
END FUNCTION
```

**Implementation:**
- Already have `FunctionParam` with `defaultValue` support
- Need to implement default value evaluation in interpreter
- Handle missing arguments by using defaults

---

### 3. Named Parameters
**Priority:** Medium-High  
**Effort:** Medium (2-3 days)  
**Impact:** Medium-High

**What it enables:**
```basic
drawRectangle(x := 10, y := 20, width := 100, height := 50)
```

**Implementation:**
- Already have `NamedArg` AST node
- Need to implement parameter matching in interpreter
- Map named args to function parameters

---

## Medium Priority (Language Features)

### 4. Lambda Functions / Anonymous Functions
**Priority:** Medium  
**Effort:** Medium-High (3-4 days)  
**Impact:** Medium-High

**What it enables:**
```basic
VAR callback = LAMBDA(x, y) -> x + y
VAR result = callback(5, 10)
```

**Implementation:**
- Already have `LambdaExpr` AST node
- Need to implement lambda evaluation
- Store lambdas as callable values

---

### 5. String Interpolation
**Priority:** Medium  
**Effort:** Low-Medium (1-2 days)  
**Impact:** Medium

**What it enables:**
```basic
VAR name = "World"
VAR msg = "Hello {name}!"  // "Hello World!"
```

**Implementation:**
- Already have `InterpolatedString` AST node
- Need to implement interpolation evaluation
- Replace `{expr}` with evaluated values

---

### 6. Dictionary/Map Type
**Priority:** Medium  
**Effort:** Medium (2-3 days)  
**Impact:** Medium

**What it enables:**
```basic
VAR map = {"key1": "value1", "key2": 42}
VAR value = map["key1"]
```

**Implementation:**
- Already have `MapLiteral` AST node
- Need to enhance Value system for maps
- Implement map access and assignment

---

## Lower Priority (Advanced Features)

### 7. Generics / Templates (Basic)
**Priority:** Low-Medium  
**Effort:** High (4-5 days)  
**Impact:** Medium

### 8. Modules / Namespaces Enhancement
**Priority:** Low-Medium  
**Effort:** Medium (2-3 days)  
**Impact:** Medium

### 9. Coroutines / Async Functions
**Priority:** Low  
**Effort:** High (5-7 days)  
**Impact:** Medium

### 10. Reflection / Introspection
**Priority:** Low  
**Effort:** Medium (2-3 days)  
**Impact:** Low-Medium

### 11. Operator Overloading
**Priority:** Low  
**Effort:** Medium-High (3-4 days)  
**Impact:** Medium

### 12. Inheritance / Classes
**Priority:** Low  
**Effort:** High (5-7 days)  
**Impact:** Medium

### 13. Hot Reload / Live Coding
**Priority:** Low  
**Effort:** High (5-7 days)  
**Impact:** Medium

### 14. Package Manager / Module System
**Priority:** Low  
**Effort:** Very High (7-10 days)  
**Impact:** Medium

---

## Recommended Implementation Order

### Phase 1: Core Language Features (2-3 weeks)
1. Function Return Types (2-3 days)
2. Optional Parameters (2-3 days)
3. Named Parameters (2-3 days)
4. String Interpolation (1-2 days)
5. Dictionary/Map Type (2-3 days)

### Phase 2: Advanced Features (2-3 weeks)
6. Lambda Functions (3-4 days)
7. Modules Enhancement (2-3 days)
8. Reflection (2-3 days)

### Phase 3: Optional Enhancements (as needed)
9. Generics (if needed)
10. Coroutines (if needed)
11. Operator Overloading (if needed)
12. Inheritance (if needed)

---

## Focus: Game Development First

Since CyberBasic is optimized for game development, prioritize features that directly benefit game programming:

1. **Function Return Types** - Better code organization
2. **Optional Parameters** - More flexible APIs
3. **Named Parameters** - Clearer function calls
4. **String Interpolation** - Better text handling
5. **Dictionary/Map** - Useful for game data structures

These features will make the language more practical for game development while maintaining simplicity.

---

## Notes

- All AST nodes for these features are already defined
- Most work is in interpreter implementation
- Parser support may need minor enhancements
- Focus on game development use cases
