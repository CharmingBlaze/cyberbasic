# CONST Implementation - Safe and Future-Proof

## Overview

The `CONST` keyword (case-insensitive: `const` or `CONST`) allows declaring immutable constants in CyberBasic. The implementation is safe, future-proof, and handles all value types.

## Features

### ✅ Supported Syntax

```basic
REM Both uppercase and lowercase work (case-insensitive)
const ANSWER = 42
CONST THE_NAME = "Charly"
const PI = 3.14159
const IS_ACTIVE = TRUE
```

### ✅ Supported Value Types

- **Numbers**: `const ANSWER = 42`
- **Strings**: `const THE_NAME = "Charly"`
- **Booleans**: `const IS_ACTIVE = TRUE`
- **Any expression**: `const SUM = 10 + 20`

### ✅ Safety Features

1. **Immutable**: Constants cannot be reassigned
   ```basic
   const ANSWER = 42
   ANSWER = 50  REM ERROR: Assignment to constant 'ANSWER'
   ```

2. **Protected from indexed assignment**
   ```basic
   const ARR = [1, 2, 3]
   ARR[0] = 99  REM ERROR: Indexed assignment to constant 'ARR'
   ```

3. **Protected from REDIM**
   ```basic
   const ARR = [1, 2, 3]
   REDIM ARR(10)  REM ERROR: REDIM on constant 'ARR'
   ```

4. **Prevents redefinition in same scope**
   ```basic
   const ANSWER = 42
   const ANSWER = 50  REM ERROR: Constant 'ANSWER' is already defined in this scope
   ```

5. **Allows shadowing in child scopes**
   ```basic
   const ANSWER = 42
   IF TRUE THEN
       const ANSWER = 100  REM OK: Shadows parent constant
       PRINT ANSWER  REM Prints 100
   ENDIF
   PRINT ANSWER  REM Prints 42 (parent scope)
   ```

## Implementation Details

### Location
- **Parser**: `src/core/parser.cpp` - `parse_const()`
- **Interpreter**: `src/core/interpreter.cpp` - `ConstDecl` handling
- **Environment**: `src/core/interpreter.cpp` - `Env::define_const()`, `Env::is_const()`

### Protection Points

1. **Direct Assignment** (`Env::set()`)
   - Checks `is_const()` before assignment
   - Throws: `"Assignment to constant 'NAME'"`

2. **Indexed Assignment** (`AssignIndex`)
   - Checks `is_const()` before array modification
   - Throws: `"Indexed assignment to constant 'NAME'"`

3. **REDIM** (`Redim`)
   - Checks `is_const()` before resizing
   - Throws: `"REDIM on constant 'NAME'"`

4. **Redefinition** (`ConstDecl`)
   - Checks `is_const_here()` to prevent same-scope redefinition
   - Throws: `"Constant 'NAME' is already defined in this scope"`

### Storage

- Constants are stored in `Env::vars` (same as variables)
- Constant names are tracked in `Env::consts` (set for fast lookup)
- Case-insensitive: all names are converted to uppercase

### Scope Rules

- Constants follow the same scoping rules as variables
- Child scopes can shadow parent constants
- Constants are checked up the scope chain: `is_const()` checks current scope and all parents
- Global constants are accessible from any scope

## Future-Proof Design

1. **Type-agnostic**: Works with any `Value` type (number, string, bool, array, map, nil)
2. **Expression support**: Can use any expression as the value
3. **Scope-aware**: Properly handles nested scopes and shadowing
4. **Performance**: Uses `std::unordered_set` for O(1) constant lookup
5. **Extensible**: Easy to add new protection points if needed

## Usage Examples

```basic
REM Game constants
const SCREEN_WIDTH = 1280
const SCREEN_HEIGHT = 720
const FPS = 60

REM String constants
const GAME_TITLE = "My Game"
const PLAYER_NAME = "Charly"

REM Mathematical constants
const PI = 3.14159
const E = 2.71828

REM Boolean flags
const DEBUG_MODE = TRUE
const ENABLE_SOUND = FALSE

REM Use in expressions
VAR area = PI * radius * radius
VAR message = "Welcome, " + PLAYER_NAME
INITWINDOW(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE)
```

## Testing

See `test_const_comprehensive.bas` for comprehensive tests covering:
- Numeric constants
- String constants
- Boolean constants
- Expression usage
- Scope shadowing
- Protection from reassignment

