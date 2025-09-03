' Raymath bindings test

PRINT "--- Raymath Test ---"

' --- Vector2 Test ---
PRINT "--- Vector2 ---"
v1 = {}
v1.x = 10
v1.y = 20

v2 = {}
v2.x = 5
v2.y = 3

v_add = VECTOR2ADD(v1, v2)
PRINT "Vector2Add: " + v_add.x + ", " + v_add.y

v_len = VECTOR2LENGTH(v1)
PRINT "Vector2Length: " + v_len

' --- Vector3 Test ---
PRINT "--- Vector3 ---"
a = {} : a.x = 1 : a.y = 2 : a.z = 3
b = {} : b.x = 4 : b.y = 5 : b.z = 6
c = VECTOR3CROSSPRODUCT(a, b)
PRINT "Vector3CrossProduct: " + c.x + ", " + c.y + ", " + c.z

' --- Matrix Test ---
PRINT "--- Matrix ---"
ident = MATRIXIDENTITY()
PRINT "MatrixIdentity.m0: " + ident.m0
PRINT "MatrixIdentity.m5: " + ident.m5
PRINT "MatrixIdentity.m1: " + ident.m1

' --- Quaternion Test ---
PRINT "--- Quaternion ---"
q_ident = QUATERNIONIDENTITY()
PRINT "QuaternionIdentity.x: " + q_ident.x
PRINT "QuaternionIdentity.y: " + q_ident.y
PRINT "QuaternionIdentity.z: " + q_ident.z
PRINT "QuaternionIdentity.w: " + q_ident.w

PRINT "--- Test Complete ---"
