// 3D Array Demo
// Demonstrates creating and using 3D arrays

PRINT "=== 3D Array Demo ==="
PRINT ""

// Create a 3D array: Array(width, height, depth)
VAR arr3d = Array(3, 4, 5)
PRINT "Created 3D array: Array(3, 4, 5)"
PRINT ""

// Get dimensions
VAR dims = Array.getDimensions(arr3d)
PRINT "Dimensions: [" + STR(dims[0]) + ", " + STR(dims[1]) + ", " + STR(dims[2]) + "]"
PRINT ""

// Access and set values in 3D array
arr3d[0][0][0] = 100
arr3d[1][2][3] = 200
arr3d[2][3][4] = 300

PRINT "Set values:"
PRINT "  arr3d[0][0][0] = " + STR(arr3d[0][0][0])
PRINT "  arr3d[1][2][3] = " + STR(arr3d[1][2][3])
PRINT "  arr3d[2][3][4] = " + STR(arr3d[2][3][4])
PRINT ""

// Iterate through 3D array
PRINT "Iterating through 3D array:"
FOR x = 0 TO 2
    FOR y = 0 TO 3
        FOR z = 0 TO 4
            IF arr3d[x][y][z] <> 0 THEN
                PRINT "  arr3d[" + STR(x) + "][" + STR(y) + "][" + STR(z) + "] = " + STR(arr3d[x][y][z])
            END IF
        NEXT
    NEXT
NEXT
PRINT ""

// Create 2D array for comparison
VAR arr2d = Array(5, 10)
PRINT "Created 2D array: Array(5, 10)"
arr2d[2][5] = 42
PRINT "  arr2d[2][5] = " + STR(arr2d[2][5])
PRINT ""

// Create 1D array
VAR arr1d = Array(10)
PRINT "Created 1D array: Array(10)"
arr1d[5] = 99
PRINT "  arr1d[5] = " + STR(arr1d[5])
PRINT ""

PRINT "3D arrays fully supported!"

