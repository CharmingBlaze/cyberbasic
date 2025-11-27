REM Comments Demo - CyberBasic Example
REM This example demonstrates comments demo functionality
REM Use WASD or arrow keys to interact (where applicable)

// C++ Style Comments Demo
// This file demonstrates the new comment syntax

// Single-line comment using //
LET x = 10
LET y = 20

/*
 * Multi-line comment block
 * You can write multiple lines here
 * This is useful for longer explanations
 */

LET result = x + y

// Comments can be at end of line too
LET sum = x + y // Calculate sum

/*
 * You can also use multi-line comments
 * for code blocks that are temporarily disabled
 * 
 * LET oldCode = 999
 * PRINT oldCode
 */

PRINT "Result: " + STR(result)

// REM still works for backward compatibility
REM This is the old way, but // is preferred

