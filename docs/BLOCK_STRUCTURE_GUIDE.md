# Block Structure Guide

CyberBASIC uses **explicit END keywords** for block structure. This guide documents what's supported in the parser and interpreter.

## How It Works

**Parser Level:** The parser validates END keywords during code parsing. If an END keyword is missing or incorrect, a parse error occurs.

**Interpreter Level:** The interpreter works with the parsed AST (Abstract Syntax Tree) and doesn't check END keywords at runtime. By the time code reaches the interpreter, all END keywords have already been validated by the parser.

## ✅ Supported END Keywords

CyberBASIC currently requires **single-word** END keywords for most blocks, with **two-word** support only for `END IF`:

### Single-Word END Keywords (Required)

```basic
FUNCTION Add(a, b)
    RETURN a + b
ENDFUNCTION

SUB PrintHeader(title$)
    PRINT "=========="
    PRINT title$
    PRINT "=========="
ENDSUB

IF x > 0 THEN
    PRINT "Positive"
ENDIF

WHILE x < 10
    x = x + 1
WEND
```

### Two-Word END Keywords (Supported for IF)

```basic
IF x > 0 THEN
    PRINT "Positive"
END IF
```

**Note:** 
- `END IF` (two words) is supported via `ENDIF` (single word)
- `WEND` is always single-word (no `END WHILE` variant)
- `ENDFUNCTION` and `ENDSUB` are single-word only (no `END FUNCTION` or `END SUB` variants currently)

## Supported END Keywords

### Functions and Subroutines
- ✅ `ENDFUNCTION` (single-word only)
- ✅ `ENDSUB` (single-word only)

### Control Flow
- ✅ `ENDIF` or `END IF` (both supported)
- ✅ `WEND` (always single-word)
- ✅ `NEXT` (for FOR loops, not an END keyword)

### Other Blocks
- ✅ `ENDTYPE` (single-word)
- ✅ `ENDMODULE` (single-word)
- ✅ `ENDSELECT` (single-word)
- ✅ `ENDSTATE` (single-word)
- ✅ `ENDMATCH` (single-word)
- ✅ `ENDENUM` (single-word)
- ✅ `ENDUNION` (single-word)
- ✅ `ENDLAMBDA` (single-word)
- ✅ `ENDOPERATOR` (single-word)
- ✅ `ENDUSING` (single-word)

## Examples

### Function with Both Styles

**Single-word style:**
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
```

**Mixed style (END IF supported, but ENDFUNCTION is single-word):**
```basic
FUNCTION calculate(x, y, operation$)
    IF operation$ = "add" THEN
        RETURN x + y
    ELSEIF operation$ = "multiply" THEN
        RETURN x * y
    ELSE
        RETURN 0
    END IF
ENDFUNCTION
```

### Nested Blocks

```basic
FUNCTION processData(data$)
    IF data$ <> "" THEN
        VAR result = PARSEJSON(data$)
        IF result <> NIL THEN
            RETURN result.value
        END IF
    END IF
    RETURN NIL
END FUNCTION
```

## Indentation

While CyberBASIC requires explicit END keywords (indentation alone is not sufficient), **proper indentation is strongly recommended** for readability:

```basic
FUNCTION wellFormatted(a, b)
    IF a > b THEN
        VAR result = a - b
        IF result > 0 THEN
            RETURN result
        END IF
    END IF
    RETURN 0
END FUNCTION
```

## Best Practices

1. **Be Consistent**: Choose one style (single-word or two-word) and use it consistently throughout your project
2. **Indent Properly**: Use consistent indentation (2 or 4 spaces) to show block structure
3. **Match Keywords**: Use the correct END keyword style:
   - `FUNCTION ... ENDFUNCTION` (single-word, required)
   - `IF ... ENDIF` or `IF ... END IF` (both supported)
4. **Readability First**: The goal is clean, readable code - choose the style that works best for your team

## Case Insensitivity

All keywords are **case-insensitive**:

```basic
function Add(a, b)
    return a + b
endfunction

FUNCTION Subtract(a, b)
    RETURN a - b
ENDFUNCTION

Function Multiply(a, b)
    Return a * b
EndFunction
```

All of the above are valid and equivalent.

