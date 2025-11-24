# Dot Notation & Modern Language Upgrade Plan

## Executive Summary

Transform CyberBasic into a modern, expressive language with dot notation support, full raylib integration, and object-oriented features - similar to modern game development languages but without mentioning specific products.

## Core Objectives

1. **Dot Notation**: Support `object.property.method()` syntax
2. **Object System**: Native objects for raylib types (Camera, Vector3, Color, etc.)
3. **Full Raylib Integration**: All 527 functions accessible via dot notation
4. **Modern Syntax**: Clean, intuitive, developer-friendly
5. **Modular Architecture**: Maintain existing modular structure
6. **Backward Compatible**: Existing code continues to work

---

## Phase 1: Dot Notation Foundation (Week 1-2)

### 1.1 Lexer Updates

**Tasks:**
- [ ] Verify dot token exists (likely `Tok::Dot` or `Tok::Period`)
- [ ] Ensure lexer properly tokenizes dots in expressions
- [ ] Test: `object.property.method()`

**Files:**
- `src/core/lexer.cpp`
- `include/bas/token.hpp`

**Deliverable:** Lexer correctly tokenizes dot notation

### 1.2 AST Extensions

**Tasks:**
- [ ] Add `MemberAccess` expression node:
  ```cpp
  struct MemberAccess : Expr {
    std::unique_ptr<Expr> object;
    std::string member;
    MemberAccess(std::unique_ptr<Expr> obj, std::string mem)
      : object(std::move(obj)), member(std::move(mem)) {}
  };
  ```
- [ ] Add `MethodCall` expression node:
  ```cpp
  struct MethodCall : Expr {
    std::unique_ptr<Expr> object;
    std::string method;
    std::vector<std::unique_ptr<Expr>> args;
    MethodCall(std::unique_ptr<Expr> obj, std::string m, std::vector<std::unique_ptr<Expr>> a)
      : object(std::move(obj)), method(std::move(m)), args(std::move(a)) {}
  };
  ```
- [ ] Update `include/bas/ast.hpp`

**Deliverable:** AST supports member access and method calls

### 1.3 Parser Updates

**Tasks:**
- [ ] Add `member_access()` parsing function
- [ ] Update `primary()` to handle dots after identifiers
- [ ] Support chained dot notation: `object.property.subproperty.method()`
- [ ] Handle method calls: `object.method(args)`
- [ ] Update `src/core/parser.cpp`

**Grammar:**
```
member_access := primary ('.' IDENTIFIER)*
method_call := member_access ('(' args? ')')?
```

**Deliverable:** Parser correctly parses dot notation

### 1.4 Interpreter Updates

**Tasks:**
- [ ] Add `eval_member_access()` function
- [ ] Add `eval_method_call()` function
- [ ] Support object property access
- [ ] Support method invocation
- [ ] Update `src/core/interpreter.cpp`

**Deliverable:** Interpreter executes dot notation

---

## Phase 2: Object System (Week 3-4)

### 2.1 Value Extensions

**Tasks:**
- [ ] Add `Object` type to `Value` variant:
  ```cpp
  using Object = std::map<std::string, Value>;
  using V = std::variant<..., Object>;
  ```
- [ ] Add object factory methods:
  ```cpp
  static Value from_object(Object obj);
  static Value create_object();
  ```
- [ ] Add object access methods:
  ```cpp
  bool is_object() const;
  Object& as_object();
  const Object& as_object() const;
  Value get_property(const std::string& name) const;
  void set_property(const std::string& name, Value value);
  ```
- [ ] Update `include/bas/value.hpp`

**Deliverable:** Value system supports objects

### 2.2 Built-in Object Types

**Tasks:**
- [ ] Create object factories for raylib types:
  - `Camera3D()` - Returns camera object
  - `Vector3(x, y, z)` - Returns vector object
  - `Vector2(x, y)` - Returns vector2 object
  - `Color(r, g, b, a)` - Returns color object
  - `Rectangle(x, y, w, h)` - Returns rectangle object
- [ ] Add property accessors:
  - `camera.position`, `camera.target`, `camera.up`
  - `vector.x`, `vector.y`, `vector.z`
  - `color.r`, `color.g`, `color.b`, `color.a`
- [ ] Create `src/core/builtins_objects.cpp`

**Deliverable:** Built-in object types available

### 2.3 Object Method Support

**Tasks:**
- [ ] Implement method resolution:
  - Check object methods first
  - Fall back to global functions
  - Support chaining
- [ ] Add object methods:
  - `vector.length()`, `vector.normalize()`
  - `camera.update()`
  - etc.

**Deliverable:** Objects support methods

---

## Phase 3: Raylib Dot Notation Integration (Week 5-6)

### 3.1 Raylib Object Wrappers

**Tasks:**
- [ ] Create wrapper classes for raylib types:
  - `Camera3DWrapper`
  - `Vector3Wrapper`
  - `Vector2Wrapper`
  - `ColorWrapper`
  - `RectangleWrapper`
  - `Texture2DWrapper`
  - `ModelWrapper`
  - etc.
- [ ] Each wrapper provides:
  - Property access
  - Method calls
  - Type conversion
- [ ] Create `include/bas/raylib_objects.hpp`

**Deliverable:** Raylib types accessible as objects

### 3.2 Function-to-Method Conversion

**Tasks:**
- [ ] Convert raylib functions to object methods:
  - `INITWINDOW()` → `Window.init()`
  - `DRAWRECTANGLE()` → `Graphics.drawRectangle()`
  - `LOADTEXTURE()` → `Texture.load()`
  - etc.
- [ ] Maintain backward compatibility (both forms work)
- [ ] Update generator to create both function and method forms

**Deliverable:** Raylib accessible via dot notation

### 3.3 Namespace Organization

**Tasks:**
- [ ] Organize functions into logical namespaces:
  - `Window.*` - Window management
  - `Graphics.*` - 2D graphics
  - `Graphics3D.*` - 3D graphics
  - `Audio.*` - Audio functions
  - `Input.*` - Input handling
  - `Math.*` - Math functions
  - `File.*` - File operations
- [ ] Support both:
  - `Window.init(800, 600, "Game")` (dot notation)
  - `INITWINDOW(800, 600, "Game")` (classic)

**Deliverable:** Organized namespace system

---

## Phase 4: Modern Language Features (Week 7-8)

### 4.1 Enhanced Syntax

**Tasks:**
- [ ] Support method chaining:
  ```basic
  LET camera = Camera3D()
  camera.position = Vector3(0, 10, 10)
  camera.target = Vector3(0, 0, 0)
  camera.update()
  ```
- [ ] Support object literals:
  ```basic
  LET vec = Vector3(1, 2, 3)
  LET color = Color(255, 0, 0, 255)
  ```
- [ ] Support property assignment:
  ```basic
  camera.position.x = 10
  camera.position.y = 20
  ```

**Deliverable:** Modern, expressive syntax

### 4.2 Type System Enhancements

**Tasks:**
- [ ] Add type checking for object properties
- [ ] Support type inference
- [ ] Add type annotations (optional):
  ```basic
  LET camera AS Camera3D = Camera3D()
  ```

**Deliverable:** Enhanced type system

### 4.3 Standard Library Objects

**Tasks:**
- [ ] Create standard object types:
  - `Array` - Enhanced array with methods
  - `String` - String object with methods
  - `Map` - Dictionary/Map object
  - `File` - File object
  - `Timer` - Timer object
- [ ] Add methods:
  - `array.length()`, `array.push()`, `array.pop()`
  - `string.length()`, `string.upper()`, `string.lower()`
  - etc.

**Deliverable:** Rich standard library

---

## Phase 5: Integration & Testing (Week 9-10)

### 5.1 Backward Compatibility

**Tasks:**
- [ ] Ensure all existing code works
- [ ] Test all examples
- [ ] Verify function calls still work
- [ ] Document migration path

**Deliverable:** 100% backward compatible

### 5.2 Documentation

**Tasks:**
- [ ] Create dot notation guide
- [ ] Document object types
- [ ] Create migration examples
- [ ] Update all documentation

**Deliverable:** Comprehensive documentation

### 5.3 Examples

**Tasks:**
- [ ] Create examples using dot notation
- [ ] Show both old and new styles
- [ ] Demonstrate best practices

**Deliverable:** Example programs

---

## Implementation Details

### Dot Notation Grammar

```
expression := or_expression
or_expression := and_expression (OR and_expression)*
...
primary := 
  | literal
  | identifier
  | identifier '(' args ')'
  | identifier '.' identifier ('.' identifier)*
  | identifier '.' identifier '(' args ')'
  | '(' expression ')'
```

### Object Representation

```cpp
// Objects stored as maps in Value
Value::Object obj = {
  {"x", Value::from_number(10)},
  {"y", Value::from_number(20)},
  {"z", Value::from_number(30)}
};

// Access: obj["x"] or obj.get_property("x")
```

### Method Resolution

1. Check if object has method
2. Check if object has property that's callable
3. Check global function registry
4. Error if not found

---

## Example Syntax

### Before (Classic)
```basic
INITWINDOW(800, 600, "Game")
LET camera = CAMERA3D()
LET pos = VECTOR3(0, 10, 10)
SETCAMERAPOSITION(camera, pos)
DRAWRECTANGLE(100, 100, 200, 150, RED)
```

### After (Modern with Dot Notation)
```basic
Window.init(800, 600, "Game")
LET camera = Camera3D()
camera.position = Vector3(0, 10, 10)
camera.target = Vector3(0, 0, 0)
camera.update()

Graphics.drawRectangle(100, 100, 200, 150, Color.red)

REM Or mixed style (backward compatible)
INITWINDOW(800, 600, "Game")
LET camera = CAMERA3D()
camera.position = VECTOR3(0, 10, 10)
```

---

## Success Criteria

- [ ] Dot notation fully functional
- [ ] All raylib functions accessible via dot notation
- [ ] Object system complete
- [ ] Backward compatible
- [ ] Well documented
- [ ] Examples provided

---

**Status**: Ready for Implementation  
**Priority**: High  
**Estimated Time**: 10 weeks

