# Basic Statements Implementation

This document describes the implementation of common BASIC statements in CyberBasic.

## Implemented Statements

### 1. Assignment Statements ✅
- Simple assignment: `a = 42`
- Numeric assignment: `aNum = 99.9`
- String assignment: `aStr = "HELLO"`
- Array assignment: `arr[1] = 49`

All assignment types are fully supported through the `Assign` and `AssignIndex` AST nodes.

### 2. FOR NEXT Statements ✅
- Basic FOR loop: `FOR T = 1 TO 5 ... NEXT T`
- FOR with STEP: `FOR week = 1 TO 52 STEP 4 ... NEXT week`
- Nested FOR loops are supported
- STEP can be positive or negative
- Loop variable is automatically declared in the loop scope

**Example:**
```basic
FOR T = 1 TO 5
  PRINTC (T)
  PRINTC (" ")
NEXT T
```

### 3. IF THEN Statements ✅
- Single-line IF: `IF age >= 16 THEN PRINT "You can buy a lottery ticket"`
- Multi-line IF: `IF A = B THEN ... ENDIF`
- IF with ELSE: `IF A = B THEN ... ELSE ... ENDIF`
- IF with ELSEIF: `IF A = 1 THEN ... ELSEIF A = 2 THEN ... ENDIF`

**Example:**
```basic
IF A = B THEN
  PRINT "The values are the same!"
ELSE
  PRINT "The values are different!"
ENDIF
```

### 4. GOSUB/RETURN Statements ✅
- GOSUB: `GOSUB MySubroutine` - calls a subroutine at a label
- RETURN: `RETURN` - returns from GOSUB to the calling location
- Supports nested GOSUB calls (return stack)
- RETURN in GOSUB context pops the return stack
- RETURN in function/sub context works as before

**Example:**
```basic
PRINT "Hello"
GOSUB MySubroutine
PRINT "Back from subroutine"

MySubroutine:
  PRINT "World"
RETURN
```

### 5. GOTO Statement ✅
- GOTO: `GOTO label` - unconditional jump to a label
- Labels are defined with `label:` syntax
- Case-insensitive label matching
- Error if label not found

**Example:**
```basic
GOTO start

start:
  PRINT "Starting..."
  GOTO end

end:
  PRINT "End"
```

### 6. Labels ✅
- Label syntax: `label:` (identifier followed by colon)
- Labels are collected in first pass before execution
- Duplicate labels cause an error
- Labels are skipped during execution (they're just markers)

**Example:**
```basic
MyLabel:
  PRINT "At label"
```

### 7. PRINT Statement ✅
- PRINT: `PRINT (expression)` - prints value followed by newline
- Supports any expression type (numbers, strings, etc.)
- Automatic type conversion to string

**Example:**
```basic
PRINT ("Hello")
PRINT (42)
PRINT (a)
```

### 8. PRINTC Statement ✅
- PRINTC: `PRINTC (expression)` - prints value without newline
- Cursor stays at end of line after printing
- Useful for printing multiple values on same line

**Example:**
```basic
PRINTC ("Hello ")
PRINTC ("World")
PRINT ("")  REM Newline
```

### 9. END Statement ✅
- END: `END` - terminates program execution immediately
- Any statements after END are not executed
- Returns exit code 0 (success)

**Example:**
```basic
PRINT "This prints"
END
PRINT "This does not print"
```

### 10. EXIT Statement ✅ (Already implemented)
- EXIT FOR: `EXIT FOR` - exits FOR loop
- EXIT WHILE: `EXIT WHILE` - exits WHILE loop
- EXIT SUB: `EXIT SUB` - exits subroutine
- EXIT FUNCTION: `EXIT FUNCTION` - exits function

## Implementation Details

### Lexer Changes
- Added tokens: `Gosub`, `Goto`, `PrintC`, `End`
- Keywords are case-insensitive

### Parser Changes
- Added parsing functions: `parse_gosub()`, `parse_goto()`, `parse_end()`, `parse_printc()`
- Label parsing in `parse_ident_statement()` - checks for colon after identifier
- All statements properly integrated into statement dispatch

### AST Changes
- Added AST nodes: `Gosub`, `Goto`, `Label`, `End`, `PrintC`
- All nodes properly defined in `ast.hpp`

### Interpreter Changes
- Two-pass execution:
  1. First pass: collect labels and function/sub declarations
  2. Second pass: execute with GOTO/GOSUB support using program counter
- GOSUB return stack: `std::vector<size_t> g_gosub_stack`
- Label map: `std::unordered_map<std::string, size_t> g_labels`
- RETURN handling: checks if in GOSUB context (stack not empty) or function/sub context
- END statement: immediately returns from `interpret()` function

### Builtin Functions
- PRINTC registered in `register_builtins_console()`
- Prints without newline, cursor stays at end

## Safety Features

1. **Label Validation**: Labels are checked for existence before GOTO/GOSUB
2. **Duplicate Label Detection**: Duplicate labels cause runtime error
3. **Return Stack Safety**: GOSUB stack prevents stack overflow
4. **Case-Insensitive Matching**: Labels and keywords work in any case

## Future-Proof Design

- Modular implementation: each statement type is separate
- Extensible: easy to add new control flow statements
- Type-safe: uses proper AST nodes and type checking
- Error handling: proper error messages for missing labels, etc.

## Testing

See `test_basic_statements.bas` for comprehensive examples of all implemented statements.

