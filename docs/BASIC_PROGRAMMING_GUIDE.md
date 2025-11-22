# CyberBasic Programming Guide

**Complete guide to the Modern BASIC programming language**

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
3. [Variables and Data Types](#variables-and-data-types)
4. [Constants](#constants)
5. [Arrays](#arrays)
6. [Operators](#operators)
7. [Control Flow](#control-flow)
8. [Functions and Subroutines](#functions-and-subroutines)
9. [User-Defined Types](#user-defined-types)
10. [String Operations](#string-operations)
11. [File I/O](#file-io)
12. [Error Handling](#error-handling)
13. [Modules and Includes](#modules-and-includes)
14. [Best Practices](#best-practices)
15. [Advanced Topics](#advanced-topics)

---

## Introduction

CyberBasic is a modern implementation of the BASIC programming language designed for clarity, simplicity, and power. It combines classic BASIC syntax with modern programming features, making it ideal for both beginners and experienced programmers.

### What is BASIC?

BASIC (Beginner's All-purpose Symbolic Instruction Code) was designed to be easy to learn and use. CyberBasic maintains this philosophy while adding modern features like:
- Type safety with optional explicit typing
- Advanced array operations
- Object-oriented concepts with dot notation
- JSON serialization
- Module system
- Comprehensive error handling

### Philosophy

CyberBasic follows these principles:
- **Simplicity**: Easy to read and write
- **Expressiveness**: Powerful features without complexity
- **Safety**: Clear error messages and type checking
- **Flexibility**: Multiple ways to accomplish tasks

---

## Getting Started

### Your First Program

```basic
REM This is a comment
PRINT "Hello, World!"
```

Save this as `hello.bas` and run it:
```bash
cyberbasic.exe hello.bas
```

### Program Structure

A CyberBasic program consists of:
- **Statements**: Instructions that perform actions
- **Comments**: Explanatory text (ignored by interpreter)
- **Labels**: Named locations for GOTO/GOSUB
- **Functions**: Reusable code blocks

```basic
REM Program: Simple Calculator
REM Author: Your Name
REM Date: 2024

REM Variable declarations
VAR a = 10
VAR b = 5

REM Calculations
VAR sum = a + b
VAR product = a * b

REM Output
PRINT "Sum: " + STR(sum)
PRINT "Product: " + STR(product)
```

### Comments

Comments help document your code:

```basic
REM This is a full-line comment
PRINT "Code here"  REM This is an inline comment

REM Multi-line comments require REM on each line
REM Line 1 of comment
REM Line 2 of comment
```

---

## Variables and Data Types

### Variable Declaration

CyberBasic supports multiple ways to declare variables:

```basic
REM Modern style (recommended)
VAR x = 10
VAR name$ = "John"
VAR price# = 99.99

REM Classic style
LET y = 20
LET city$ = "New York"
LET temperature# = 72.5

REM Direct assignment (variable created automatically)
z = 30
```

### Variable Naming Rules

- Must start with a letter
- Can contain letters, numbers, and underscores
- Case-insensitive (MyVar, myvar, MYVAR are the same)
- Cannot be a reserved word

```basic
VAR valid_name = 1
VAR name123 = 2
VAR my_variable = 3
VAR Invalid = 4  REM Also valid (case-insensitive)
```

### Type Suffixes

CyberBasic uses type suffixes to indicate data types:

```basic
VAR count = 10        REM Integer (default)
VAR count% = 10       REM Explicit integer
VAR price# = 99.99   REM Float/double
VAR name$ = "Text"    REM String
VAR flag = TRUE       REM Boolean
```

### Data Types

#### Integers

Whole numbers (positive, negative, or zero):

```basic
VAR count = 42
VAR negative = -10
VAR zero = 0
VAR large = 1000000
```

#### Floating Point Numbers

Decimal numbers:

```basic
VAR pi# = 3.14159
VAR temperature# = 98.6
VAR precise# = 0.000001
```

#### Strings

Text data enclosed in quotes:

```basic
VAR name$ = "Alice"
VAR message$ = "Hello, World!"
VAR empty$ = ""
VAR path$ = "C:\Users\Documents\file.txt"
```

#### Booleans

True or false values:

```basic
VAR isActive = TRUE
VAR isComplete = FALSE
VAR result = (5 > 3)  REM Evaluates to TRUE
```

### Type Conversion

CyberBasic automatically converts types when needed:

```basic
VAR num = 42
VAR text$ = "The answer is " + STR(num)  REM Convert number to string

VAR str$ = "123"
VAR value = VAL(str$)  REM Convert string to number

VAR float# = 3.14
VAR int = INT(float#)  REM Convert to integer (truncates)
```

### Scope

Variables can be declared anywhere, including inside loops and conditionals:

```basic
FOR i = 1 TO 10
    VAR temp = i * 2  REM Variable created in loop
    PRINT temp
NEXT

IF condition THEN
    VAR local = 100  REM Variable only exists in this block
    PRINT local
ENDIF
```

---

## Constants

Constants are immutable values that cannot be changed after declaration:

```basic
CONST PI = 3.14159
CONST MAX_PLAYERS = 4
CONST GAME_TITLE$ = "My Game"
CONST GRAVITY# = 9.8
```

### When to Use Constants

Use constants for:
- Magic numbers (values that appear in code)
- Configuration values
- Mathematical constants
- String literals used multiple times

```basic
CONST SCREEN_WIDTH = 1024
CONST SCREEN_HEIGHT = 768
CONST FPS = 60
CONST PLAYER_SPEED = 5.0

REM Use constants instead of hardcoding values
INITWINDOW(SCREEN_WIDTH, SCREEN_HEIGHT, "Game")
SETTARGETFPS(FPS)
```

### Constant Rules

- Must be initialized when declared
- Cannot be reassigned
- Follow same naming rules as variables
- Can use type suffixes

```basic
CONST MAX = 100
REM MAX = 200  REM ERROR: Cannot reassign constant
```

---

## Arrays

Arrays store multiple values of the same type.

### Array Declaration

```basic
REM Classic style
DIM scores[10]
DIM matrix[5, 10]  REM 2D array

REM Modern style
VAR items[20]
VAR grid[8, 8]

REM Initialize with values
VAR numbers = [1, 2, 3, 4, 5]
VAR names$ = ["Alice", "Bob", "Charlie"]
```

### Array Access

Arrays use zero-based indexing:

```basic
VAR arr = [10, 20, 30, 40, 50]
PRINT arr[0]  REM Prints 10
PRINT arr[2]  REM Prints 30
PRINT arr[4]  REM Prints 50

REM Assign values
arr[1] = 25
arr[3] = 45
```

### Multi-Dimensional Arrays

```basic
VAR matrix[3, 3]

REM Access elements
matrix[0, 0] = 1
matrix[0, 1] = 2
matrix[1, 0] = 3
matrix[1, 1] = 4

REM Initialize 2D array
VAR grid = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
PRINT grid[1, 2]  REM Prints 6
```

### Array Properties

#### Length

Get or set array size:

```basic
VAR arr = [1, 2, 3, 4, 5]
VAR len = arr.length  REM Get length (returns 5)

arr.length = 10  REM Resize array (new elements are zero/nil)
PRINT arr.length  REM Now returns 10
```

### Array Methods

#### Insert

Add element at specific position:

```basic
VAR arr = [1, 2, 3, 4]
arr.insert(2, 99)  REM Insert 99 at index 2
REM Result: [1, 2, 99, 3, 4]
```

#### Remove

Remove element at position:

```basic
VAR arr = [1, 2, 3, 4, 5]
arr.remove(2)  REM Remove element at index 2
REM Result: [1, 2, 4, 5]
```

#### Sort

Sort array in ascending order:

```basic
VAR arr = [5, 2, 8, 1, 9]
arr.sort()
REM Result: [1, 2, 5, 8, 9]
```

#### Find

Binary search for value (array must be sorted):

```basic
VAR arr = [1, 2, 3, 4, 5]
arr.sort()  REM Ensure sorted
VAR index = arr.find(3)  REM Returns 2
```

#### Reverse

Reverse array order:

```basic
VAR arr = [1, 2, 3, 4, 5]
arr.reverse()
REM Result: [5, 4, 3, 2, 1]
```

#### Swap

Swap two elements:

```basic
VAR arr = [1, 2, 3, 4, 5]
arr.swap(0, 4)  REM Swap first and last
REM Result: [5, 2, 3, 4, 1]
```

### Array Iteration

```basic
VAR arr = [10, 20, 30, 40, 50]

REM Using FOR loop
FOR i = 0 TO arr.length - 1
    PRINT arr[i]
NEXT

REM Using FOR EACH (if supported)
FOR EACH item IN arr
    PRINT item
NEXT
```

### Array Assignment

Arrays can be assigned from array literals:

```basic
VAR arr1 = [1, 2, 3]
VAR arr2 = arr1  REM Copy reference
arr2[0] = 99
PRINT arr1[0]  REM Also 99 (same array)

REM Create new array
VAR arr3 = [10, 20, 30]
```

### Dictionary Literals

Dictionaries (maps) can be created using curly braces with two syntax styles:

**JSON-style syntax** (using colons):
```basic
VAR dict1 = {"key": "value", "number": 42, "flag": TRUE}
VAR dict2 = {"name": "John", "age": 30, "active": TRUE}
VAR dict3 = {2: 3, "text": "hello", 10: 20}  REM Mixed key types
```

**BASIC-style syntax** (using equals):
```basic
VAR dict1 = {key = "value", number = 42, flag = TRUE}
VAR dict2 = {name = "John", age = 30, active = TRUE}
VAR dict3 = {my_key = 100, other_key = 200}
```

**Accessing dictionary values:**
```basic
VAR dict = {"name": "Alice", "age": 25}

REM Access with bracket notation
PRINT dict["name"]   REM "Alice"
PRINT dict["age"]    REM 25

REM Access with dot notation (if key is valid identifier)
REM Note: Dot notation may not work for all keys
```

**Dictionary operations:**
```basic
VAR dict = {"a": 1, "b": 2}

REM Add/update values
dict["c"] = 3
dict["a"] = 10

REM Check if key exists
IF dict.has("a") THEN PRINT "Key 'a' exists"

REM Get all keys
VAR keys = dict.keys()  REM Returns array of keys

REM Get all values
VAR values = dict.values()  REM Returns array of values
```

### REDIM and REDIM PRESERVE

Resize arrays while preserving data:

```basic
DIM arr[5]
FOR i = 0 TO 4
    arr[i] = i * 10
NEXT

REM Resize without preserving
REDIM arr[10]  REM All elements reset

REM Resize preserving existing data
REDIM PRESERVE arr[10]  REM First 5 elements preserved
```

---

## Operators

### Arithmetic Operators

```basic
VAR a = 10
VAR b = 3

VAR sum = a + b        REM 13 (addition)
VAR diff = a - b       REM 7 (subtraction)
VAR prod = a * b       REM 30 (multiplication)
VAR quot = a / b       REM 3.333... (division)
VAR mod = a MOD b      REM 1 (modulo/remainder)
VAR power = a ^ b      REM 1000 (exponentiation)
VAR intdiv = a \ b     REM 3 (integer division)
```

### Relational Operators

Compare values and return boolean:

```basic
VAR a = 10
VAR b = 5

VAR eq = (a = b)       REM FALSE (equality)
VAR ne = (a != b)      REM TRUE (inequality)
VAR ne2 = (a <> b)     REM TRUE (alternative inequality)
VAR lt = (a < b)       REM FALSE (less than)
VAR le = (a <= b)      REM FALSE (less than or equal)
VAR gt = (a > b)       REM TRUE (greater than)
VAR ge = (a >= b)      REM TRUE (greater than or equal)
```

### Boolean Operators

Logical operations:

```basic
VAR x = TRUE
VAR y = FALSE

VAR and_result = x AND y    REM FALSE
VAR or_result = x OR y      REM TRUE
VAR not_result = NOT x      REM FALSE
VAR xor_result = x XOR y    REM TRUE
```

### Bitwise Operators

Operate on binary representation:

```basic
VAR a = 5    REM Binary: 101
VAR b = 3    REM Binary: 011

VAR lshift = a << 1     REM 10 (left shift)
VAR rshift = a >> 1     REM 2 (right shift)
VAR band = a && b       REM 1 (bitwise AND)
VAR bor = a || b        REM 7 (bitwise OR)
VAR bxor = a ~~ b       REM 6 (bitwise XOR)
VAR bnot = !a           REM -6 (bitwise NOT)
```

### Operator Precedence

Operators are evaluated in this order:
1. Parentheses `()`
2. Exponentiation `^`
3. Unary operators (`-`, `+`, `NOT`, `!`)
4. Multiplication, Division, Modulo `*`, `/`, `MOD`, `\`
5. Addition, Subtraction `+`, `-`
6. Bitwise shifts `<<`, `>>`
7. Relational operators `<`, `<=`, `>`, `>=`
8. Equality operators `=`, `!=`, `<>`
9. Bitwise AND `&&`
10. Bitwise XOR `~~`
11. Bitwise OR `||`
12. Logical AND `AND`
13. Logical OR `XOR`
14. Logical OR `OR`

Use parentheses to clarify or override precedence:

```basic
VAR result = 2 + 3 * 4      REM 14 (multiplication first)
VAR result2 = (2 + 3) * 4   REM 20 (addition first)
```

---

## Control Flow

### IF Statements

Conditional execution:

```basic
REM Simple IF
IF x > 10 THEN PRINT "Large"

REM IF-THEN-ELSE
IF score >= 100 THEN
    PRINT "You win!"
ELSE
    PRINT "Try again"
ENDIF

REM IF-THEN-ELSEIF-ELSE
IF temperature# > 90 THEN
    PRINT "Hot"
ELSEIF temperature# > 70 THEN
    PRINT "Warm"
ELSEIF temperature# > 50 THEN
    PRINT "Cool"
ELSE
    PRINT "Cold"
ENDIF
```

### WHILE Loops

Execute while condition is true:

```basic
VAR count = 0
WHILE count < 10
    PRINT count
    count = count + 1
WEND

REM Infinite loop with break
WHILE TRUE
    VAR input$ = INPUT("Enter command: ")
    IF input$ = "quit" THEN BREAK
    PRINT "You entered: " + input$
WEND
```

### FOR Loops

Iterate a specific number of times:

```basic
REM Basic FOR loop
FOR i = 1 TO 10
    PRINT i
NEXT

REM FOR with STEP
FOR i = 0 TO 100 STEP 10
    PRINT i
NEXT

REM Countdown
FOR i = 10 TO 1 STEP -1
    PRINT i
NEXT
```

### SELECT CASE

Multi-way branching:

```basic
VAR choice = 2

SELECT CASE choice
    CASE 1
        PRINT "Option 1"
    CASE 2
        PRINT "Option 2"
    CASE 3
        PRINT "Option 3"
    CASE ELSE
        PRINT "Invalid choice"
ENDSELECT

REM Range cases
SELECT CASE score
    CASE 90 TO 100
        PRINT "A"
    CASE 80 TO 89
        PRINT "B"
    CASE 70 TO 79
        PRINT "C"
    CASE ELSE
        PRINT "F"
ENDSELECT
```

### BREAK and CONTINUE

Control loop execution:

```basic
REM BREAK exits loop
FOR i = 1 TO 100
    IF i > 50 THEN BREAK
    PRINT i
NEXT

REM CONTINUE skips to next iteration
FOR i = 1 TO 10
    IF i MOD 2 = 0 THEN CONTINUE
    PRINT i  REM Only prints odd numbers
NEXT
```

### GOTO and GOSUB

Jump to labels (use sparingly):

```basic
REM GOTO - unconditional jump
GOTO start

start:
    PRINT "Starting..."
    GOTO end

middle:
    PRINT "Middle"
    GOTO start

end:
    PRINT "End"

REM GOSUB - call subroutine
GOSUB subroutine
PRINT "Back from subroutine"
END

subroutine:
    PRINT "In subroutine"
    RETURN
```

### Labels

Labels mark locations for GOTO/GOSUB:

```basic
main:
    PRINT "Main program"
    GOSUB helper
    GOTO finish

helper:
    PRINT "Helper function"
    RETURN

finish:
    PRINT "Done"
```

---

## Functions and Subroutines

### Function Declaration

Functions are reusable code blocks that return values:

```basic
FUNCTION add(a, b)
    RETURN a + b
ENDFUNCTION

VAR result = add(5, 3)  REM Returns 8
PRINT result
```

### Function with Return Value

Alternative syntax for returning values:

```basic
FUNCTION multiply(x, y)
    VAR product = x * y
ENDFUNCTION product  REM Return value

VAR result = multiply(4, 5)  REM Returns 20
```

### Function Parameters

Functions can take multiple parameters:

```basic
FUNCTION calculate(x, y, operation$)
    IF operation$ = "add" THEN
        RETURN x + y
    ELSEIF operation$ = "multiply" THEN
        RETURN x * y
    ELSE
        RETURN 0
    ENDIF
ENDFUNCTION

VAR result1 = calculate(10, 5, "add")      REM 15
VAR result2 = calculate(10, 5, "multiply")  REM 50
```

### Function Scope

Functions have their own variable scope:

```basic
VAR global = 100

FUNCTION test()
    VAR local = 50
    PRINT global  REM Can access global
    PRINT local   REM Local variable
ENDFUNCTION

test()
PRINT global  REM Still 100
REM PRINT local  REM ERROR: local doesn't exist here
```

### Subroutines

Subroutines are functions that don't return values:

```basic
SUB printHeader(title$)
    PRINT "=========="
    PRINT title$
    PRINT "=========="
ENDSUB

printHeader("My Program")
```

### Recursion

Functions can call themselves:

```basic
FUNCTION factorial(n)
    IF n <= 1 THEN
        RETURN 1
    ELSE
        RETURN n * factorial(n - 1)
    ENDIF
ENDFUNCTION

VAR result = factorial(5)  REM Returns 120
```

### Built-in Functions

CyberBasic provides many built-in functions:

```basic
REM Math functions
VAR abs_val = ABS(-5)        REM 5
VAR sqrt_val = SQRT(16)      REM 4.0
VAR sin_val = SIN(3.14159)   REM ~0
VAR cos_val = COS(0)         REM 1.0

REM String functions
VAR len = LEN("Hello")       REM 5
VAR substr$ = SUBSTR("Hello", 1, 3)  REM "ell"

REM Type conversion
VAR str$ = STR(42)           REM "42"
VAR num = VAL("123")         REM 123

REM Random
VAR rand = RND()             REM 0.0 to 1.0
VAR rand_int = RANDOM(100)   REM 0 to 99
```

---

## User-Defined Types

Types allow you to create custom data structures:

### Type Declaration

```basic
TYPE Point
    x AS FLOAT
    y AS FLOAT
END TYPE

TYPE Player
    name AS STRING
    health AS INTEGER
    position AS Point
    inventory AS INTEGER[10]
END TYPE
```

### Creating Type Instances

```basic
VAR p = Point()
p.x = 10.5
p.y = 20.3

VAR player = Player()
player.name = "Hero"
player.health = 100
player.position = p
player.inventory[0] = 1
player.inventory[1] = 5
```

### Type Methods

Types can have methods (if supported):

```basic
TYPE Vector2
    x AS FLOAT
    y AS FLOAT
END TYPE

FUNCTION Vector2.length(v AS Vector2)
    RETURN SQRT(v.x * v.x + v.y * v.y)
ENDFUNCTION

VAR v = Vector2()
v.x = 3
v.y = 4
VAR len = Vector2.length(v)  REM Returns 5.0
```

### Nested Types

Types can contain other types:

```basic
TYPE Address
    street AS STRING
    city AS STRING
    zip AS STRING
END TYPE

TYPE Person
    name AS STRING
    age AS INTEGER
    address AS Address
END TYPE

VAR person = Person()
person.name = "John"
person.age = 30
person.address.street = "123 Main St"
person.address.city = "Anytown"
```

### Type Arrays

Arrays of types:

```basic
TYPE Point
    x AS FLOAT
    y AS FLOAT
END TYPE

VAR points[10]
FOR i = 0 TO 9
    points[i] = Point()
    points[i].x = i * 10
    points[i].y = i * 5
NEXT
```

---

## String Operations

### String Concatenation

Combine strings with `+`:

```basic
VAR first$ = "Hello"
VAR last$ = "World"
VAR combined$ = first$ + ", " + last$  REM "Hello, World"
```

### String Functions

```basic
VAR text$ = "Hello, World!"

REM Length
VAR len = LEN(text$)  REM 13

REM Substring
VAR substr$ = SUBSTR(text$, 0, 5)  REM "Hello"

REM Conversion
VAR num_str$ = STR(42)      REM "42"
VAR str_num = VAL("123")     REM 123

REM Case conversion (if available)
VAR upper$ = UPPER(text$)    REM "HELLO, WORLD!"
VAR lower$ = LOWER(text$)    REM "hello, world!"
```

### String Comparison

```basic
VAR str1$ = "apple"
VAR str2$ = "banana"

IF str1$ = str2$ THEN PRINT "Equal"
IF str1$ < str2$ THEN PRINT "Less than"  REM Alphabetical
IF str1$ > str2$ THEN PRINT "Greater than"
```

### String Searching

```basic
VAR text$ = "Hello, World!"
VAR search$ = "World"

REM Find position (if available)
VAR pos = INSTR(text$, search$)  REM Returns position or -1
```

---

## File I/O

### Reading Files

```basic
REM Read entire file
VAR content$ = READFILE("data.txt")
PRINT content$

REM Read line by line (if available)
VAR file = OPENFILE("data.txt", "r")
WHILE NOT EOF(file)
    VAR line$ = READLINE(file)
    PRINT line$
WEND
CLOSEFILE(file)
```

### Writing Files

```basic
REM Write entire file
VAR text$ = "Hello, World!"
WRITEFILE("output.txt", text$)

REM Append to file
APPENDFILE("log.txt", "New entry")
```

### JSON Operations

CyberBasic supports JSON for structured data:

```basic
TYPE Config
    width AS INTEGER
    height AS INTEGER
    title AS STRING
END TYPE

VAR config = Config()
config.width = 1024
config.height = 768
config.title = "My Game"

REM Serialize to JSON
VAR json$ = config.toJSON()
WRITEFILE("config.json", json$)

REM Deserialize from JSON
VAR json_data$ = READFILE("config.json")
VAR new_config = Config()
new_config.fromJSON(json_data$)
```

### Array JSON

Arrays also support JSON:

```basic
VAR arr = [1, 2, 3, 4, 5]
VAR json$ = arr.toJSON()  REM "[1,2,3,4,5]"

VAR new_arr = []
new_arr.fromJSON(json$)
```

---

## Error Handling

### TRY-CATCH

Handle errors gracefully:

```basic
TRY
    VAR result = 10 / 0  REM Division by zero
    PRINT result
CATCH error
    PRINT "Error occurred: " + error
ENDTRY

PRINT "Program continues"
```

### Common Error Types

```basic
TRY
    VAR file$ = READFILE("missing.txt")
CATCH error
    IF INSTR(error, "file not found") >= 0 THEN
        PRINT "File does not exist"
    ELSE
        PRINT "Unknown error: " + error
    ENDIF
ENDTRY
```

### Assertions

Check conditions and fail if false:

```basic
VAR value = 10
ASSERT value > 0, "Value must be positive"

VAR index = 5
ASSERT index < array.length, "Index out of bounds"
```

---

## Modules and Includes

### IMPORT

Import other BASIC files:

```basic
IMPORT "utils.bas"
IMPORT "graphics.bas"

REM Use functions from imported modules
VAR result = utils.calculate(10, 5)
```

### INCLUDE

Include file contents directly:

```basic
INCLUDE "constants.bas"
INCLUDE "functions.bas"

REM Code from included files is inserted here
```

### Module Organization

```basic
REM math_utils.bas
FUNCTION add(a, b)
    RETURN a + b
ENDFUNCTION

FUNCTION multiply(a, b)
    RETURN a * b
ENDFUNCTION

REM main.bas
IMPORT "math_utils.bas"

VAR sum = add(5, 3)
VAR product = multiply(4, 2)
```

---

## Best Practices

### Code Organization

```basic
REM Program: Calculator
REM Author: Your Name
REM Description: Simple calculator program

REM Constants
CONST VERSION$ = "1.0"
CONST MAX_INPUT = 1000

REM Global variables
VAR result = 0
VAR history[10]

REM Functions
FUNCTION add(a, b)
    RETURN a + b
ENDFUNCTION

FUNCTION subtract(a, b)
    RETURN a - b
ENDFUNCTION

REM Main program
PRINT "Calculator " + VERSION$
VAR a = VAL(INPUT("Enter first number: "))
VAR b = VAL(INPUT("Enter second number: "))
VAR op$ = INPUT("Enter operation (+, -, *, /): ")

SELECT CASE op$
    CASE "+"
        result = add(a, b)
    CASE "-"
        result = subtract(a, b)
    CASE ELSE
        PRINT "Invalid operation"
END SELECT

PRINT "Result: " + STR(result)
```

### Naming Conventions

```basic
REM Use descriptive names
VAR playerHealth = 100        REM Good
VAR ph = 100                 REM Bad

REM Use constants for magic numbers
CONST MAX_PLAYERS = 4         REM Good
IF count > 4 THEN ...         REM Bad

REM Use consistent naming
VAR userName$ = "John"       REM camelCase
VAR user_name$ = "John"      REM snake_case (also valid)
```

### Comments

```basic
REM Explain WHY, not WHAT
VAR x = 10  REM Bad: obvious
VAR x = 10  REM Good: Player starting position

REM Document complex logic
REM Calculate distance using Pythagorean theorem
VAR distance = SQRT((x2 - x1) ^ 2 + (y2 - y1) ^ 2)
```

### Error Handling

```basic
FUNCTION safeDivide(a, b)
    IF b = 0 THEN
        PRINT "Error: Division by zero"
        RETURN 0
    ENDIF
    RETURN a / b
ENDFUNCTION

REM Or use TRY-CATCH
FUNCTION safeDivide2(a, b)
    TRY
        RETURN a / b
    CATCH error
        PRINT "Error: " + error
        RETURN 0
    ENDTRY
ENDFUNCTION
```

### Performance Tips

```basic
REM Prefer local variables
FUNCTION calculate()
    VAR local = 10  REM Faster than global
    RETURN local * 2
ENDFUNCTION

REM Cache repeated calculations
VAR len = array.length
FOR i = 0 TO len - 1  REM Don't call length each iteration
    PRINT array[i]
NEXT

REM Use appropriate data types
VAR count = 10        REM Integer (smaller, faster)
VAR price# = 99.99    REM Float (when needed)
```

---

## Advanced Topics

### Dictionaries

Dictionaries provide key-value storage:

```basic
REM JSON-style syntax
VAR dict = {"name": "Player", "score": 100, 2: 3}

REM BASIC-style syntax
VAR dict2 = {name = "Player", score = 100}

REM Access values
VAR name = dict["name"]
VAR score = dict["score"]

REM Add/update
dict["level"] = 5
```

### Tuples

Tuples group multiple values:

```basic
VAR pos = (100, 200)
VAR (x, y) = pos  REM Destructure

FUNCTION getSize() AS tuple
    RETURN (800, 600)
END FUNCTION
```

### Enums

Enums define named constants:

```basic
ENUM Direction
    North, South, East, West
END ENUM

VAR dir = North
```

### Coroutines

Coroutines enable async execution:

```basic
FUNCTION animate() AS coroutine
    YIELD
END FUNCTION

WAIT(2.0)
TIMER(1000, repeat = true) -> PRINT "Tick"
```

### ECS System

Entity-Component-System architecture:

```basic
COMPONENT Position {x = 0, y = 0}
VAR player = ENTITY()
player.ADD(Position(100, 200))

SYSTEM Movement(Position, Velocity)
    Position.x += Velocity.dx
END SYSTEM

RUN Movement
```

## Advanced Topics

### Dot Notation

Access object properties and methods:

```basic
TYPE Vector2
    x AS FLOAT
    y AS FLOAT
END TYPE

VAR v = Vector2()
v.x = 10.5
v.y = 20.3

REM Access nested properties
TYPE Player
    position AS Vector2
END TYPE

VAR player = Player()
player.position.x = 100
player.position.y = 200
```

### Closures and Lambdas

(If supported in your version)

```basic
FUNCTION createMultiplier(factor)
    FUNCTION multiply(x)
        RETURN x * factor
    ENDFUNCTION
    RETURN multiply
ENDFUNCTION

VAR double = createMultiplier(2)
VAR triple = createMultiplier(3)

PRINT double(5)   REM 10
PRINT triple(5)   REM 15
```

### Metaprogramming

Dynamic code execution (if available):

```basic
REM Evaluate expressions at runtime
VAR expr$ = "10 + 20"
VAR result = EVAL(expr$)  REM 30
```

### Memory Management

CyberBasic handles memory automatically:

```basic
REM No manual memory management needed
VAR arr = [1, 2, 3, 4, 5]
arr = []  REM Old array automatically freed

VAR obj = MyType()
obj = MyType()  REM Old object automatically freed
```

---

## Complete Example Programs

### Example 1: Simple Calculator

```basic
REM Simple Calculator
PRINT "Simple Calculator"
PRINT "================="

WHILE TRUE
    VAR a = VAL(INPUT("Enter first number: "))
    VAR op$ = INPUT("Enter operation (+, -, *, /, q to quit): ")
    
    IF op$ = "q" THEN BREAK
    
    VAR b = VAL(INPUT("Enter second number: "))
    VAR result = 0
    
    SELECT CASE op$
        CASE "+"
            result = a + b
        CASE "-"
            result = a - b
        CASE "*"
            result = a * b
        CASE "/"
            IF b = 0 THEN
                PRINT "Error: Division by zero"
                CONTINUE
            ENDIF
            result = a / b
        CASE ELSE
            PRINT "Invalid operation"
            CONTINUE
    ENDSELECT
    
    PRINT STR(a) + " " + op$ + " " + STR(b) + " = " + STR(result)
    PRINT ""
WEND

PRINT "Goodbye!"
```

### Example 2: Array Operations

```basic
REM Array Operations Demo
VAR numbers = [5, 2, 8, 1, 9, 3, 7, 4, 6]

PRINT "Original: " + numbers.toJSON()

REM Sort
numbers.sort()
PRINT "Sorted: " + numbers.toJSON()

REM Find
VAR index = numbers.find(5)
PRINT "Index of 5: " + STR(index)

REM Reverse
numbers.reverse()
PRINT "Reversed: " + numbers.toJSON()

REM Insert
numbers.insert(3, 99)
PRINT "After insert: " + numbers.toJSON()

REM Remove
numbers.remove(3)
PRINT "After remove: " + numbers.toJSON()
```

### Example 3: Type System

```basic
REM Type System Demo
TYPE Person
    name AS STRING
    age AS INTEGER
    email AS STRING
END TYPE

VAR people[3]

people[0] = Person()
people[0].name = "Alice"
people[0].age = 30
people[0].email = "alice@example.com"

people[1] = Person()
people[1].name = "Bob"
people[1].age = 25
people[1].email = "bob@example.com"

people[2] = Person()
people[2].name = "Charlie"
people[2].age = 35
people[2].email = "charlie@example.com"

FOR i = 0 TO 2
    PRINT people[i].name + " (" + STR(people[i].age) + "): " + people[i].email
NEXT
```

---

## Conclusion

This guide covers the core features of CyberBasic. The language is designed to be:
- **Easy to learn**: Familiar BASIC syntax
- **Powerful**: Modern features for complex programs
- **Flexible**: Multiple ways to accomplish tasks
- **Safe**: Built-in error handling and type checking

Continue exploring by:
- Reading the Game Development Guide for game-specific features
- Examining example programs in the `examples/` directory
- Experimenting with the language features
- Building your own programs

Happy programming!

