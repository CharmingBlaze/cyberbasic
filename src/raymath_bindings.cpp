#include "bas/raymath.hpp"
#include "bas/value.hpp"
#include "bas/builtins.hpp"
#include "bas/raymath_helpers.hpp"
#include "raylib.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#include "raymath.h"
#pragma GCC diagnostic pop

namespace bas {

void register_raymath_functions(FunctionRegistry& R) {
    // --- Vector2 ---
    R.add("VECTOR2ZERO", NativeFn{"VECTOR2ZERO", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector2_to_Map(Vector2Zero()));
    }});
    R.add("VECTOR2ONE", NativeFn{"VECTOR2ONE", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector2_to_Map(Vector2One()));
    }});
    R.add("VECTOR2ADD", NativeFn{"VECTOR2ADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Add(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }});
    R.add("VECTOR2ADDVALUE", NativeFn{"VECTOR2ADDVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2AddValue(Value_to_Vector2(a[0]), a[1].as_number())));
    }});
    R.add("VECTOR2SUBTRACT", NativeFn{"VECTOR2SUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Subtract(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }});
    R.add("VECTOR2SUBTRACTVALUE", NativeFn{"VECTOR2SUBTRACTVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2SubtractValue(Value_to_Vector2(a[0]), a[1].as_number())));
    }});
    R.add("VECTOR2LENGTH", NativeFn{"VECTOR2LENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Length(Value_to_Vector2(a[0])));
    }});
    R.add("VECTOR2DOTPRODUCT", NativeFn{"VECTOR2DOTPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2DotProduct(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }});
    R.add("VECTOR2DISTANCE", NativeFn{"VECTOR2DISTANCE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Distance(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }});
    R.add("VECTOR2ANGLE", NativeFn{"VECTOR2ANGLE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Angle(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }});
    R.add("VECTOR2SCALE", NativeFn{"VECTOR2SCALE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Scale(Value_to_Vector2(a[0]), a[1].as_number())));
    }});
    R.add("VECTOR2MULTIPLY", NativeFn{"VECTOR2MULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Multiply(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }});
    R.add("VECTOR2NEGATE", NativeFn{"VECTOR2NEGATE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Negate(Value_to_Vector2(a[0]))));
    }});
    R.add("VECTOR2DIVIDE", NativeFn{"VECTOR2DIVIDE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Divide(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }});
    R.add("VECTOR2NORMALIZE", NativeFn{"VECTOR2NORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Normalize(Value_to_Vector2(a[0]))));
    }});
    R.add("VECTOR2LERP", NativeFn{"VECTOR2LERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Lerp(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]), a[2].as_number())));
    }});

    // --- Vector3 ---
    R.add("VECTOR3ZERO", NativeFn{"VECTOR3ZERO", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector3_to_Map(Vector3Zero()));
    }});
    R.add("VECTOR3ONE", NativeFn{"VECTOR3ONE", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector3_to_Map(Vector3One()));
    }});
    R.add("VECTOR3ADD", NativeFn{"VECTOR3ADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Add(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3SUBTRACT", NativeFn{"VECTOR3SUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Subtract(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3SCALE", NativeFn{"VECTOR3SCALE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Scale(Value_to_Vector3(a[0]), a[1].as_number())));
    }});
    R.add("VECTOR3MULTIPLY", NativeFn{"VECTOR3MULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Multiply(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3CROSSPRODUCT", NativeFn{"VECTOR3CROSSPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3CrossProduct(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3PERPENDICULAR", NativeFn{"VECTOR3PERPENDICULAR", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Perpendicular(Value_to_Vector3(a[0]))));
    }});
    R.add("VECTOR3LENGTH", NativeFn{"VECTOR3LENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3Length(Value_to_Vector3(a[0])));
    }});
    R.add("VECTOR3DOTPRODUCT", NativeFn{"VECTOR3DOTPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3DotProduct(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }});
    R.add("VECTOR3DISTANCE", NativeFn{"VECTOR3DISTANCE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3Distance(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }});
    R.add("VECTOR3NEGATE", NativeFn{"VECTOR3NEGATE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Negate(Value_to_Vector3(a[0]))));
    }});
    R.add("VECTOR3DIVIDE", NativeFn{"VECTOR3DIVIDE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Divide(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3NORMALIZE", NativeFn{"VECTOR3NORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Normalize(Value_to_Vector3(a[0]))));
    }});
    R.add("VECTOR3LERP", NativeFn{"VECTOR3LERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Lerp(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), a[2].as_number())));
    }});
    R.add("VECTOR3REFLECT", NativeFn{"VECTOR3REFLECT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Reflect(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3MIN", NativeFn{"VECTOR3MIN", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Min(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3MAX", NativeFn{"VECTOR3MAX", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Max(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("VECTOR3BARYCENTER", NativeFn{"VECTOR3BARYCENTER", 4, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Barycenter(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), Value_to_Vector3(a[2]), Value_to_Vector3(a[3]))));
    }});

    // --- Matrix ---
    R.add("MATRIXIDENTITY", NativeFn{"MATRIXIDENTITY", 0, [](const std::vector<Value>&) {
        return Value::from_map(Matrix_to_Map(MatrixIdentity()));
    }});
    R.add("MATRIXADD", NativeFn{"MATRIXADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixAdd(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }});
    R.add("MATRIXSUBTRACT", NativeFn{"MATRIXSUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixSubtract(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }});
    R.add("MATRIXMULTIPLY", NativeFn{"MATRIXMULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixMultiply(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }});
    R.add("MATRIXTRANSLATE", NativeFn{"MATRIXTRANSLATE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixTranslate(a[0].as_number(), a[1].as_number(), a[2].as_number())));
    }});
    R.add("MATRIXROTATE", NativeFn{"MATRIXROTATE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotate(Value_to_Vector3(a[0]), a[1].as_number())));
    }});
    R.add("MATRIXROTATEX", NativeFn{"MATRIXROTATEX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateX(a[0].as_number())));
    }});
    R.add("MATRIXROTATEY", NativeFn{"MATRIXROTATEY", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateY(a[0].as_number())));
    }});
    R.add("MATRIXROTATEZ", NativeFn{"MATRIXROTATEZ", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateZ(a[0].as_number())));
    }});
    R.add("MATRIXSCALE", NativeFn{"MATRIXSCALE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixScale(a[0].as_number(), a[1].as_number(), a[2].as_number())));
    }});
    R.add("MATRIXINVERT", NativeFn{"MATRIXINVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixInvert(Value_to_Matrix(a[0]))));
    }});

    // --- Quaternion ---
    R.add("QUATERNIONIDENTITY", NativeFn{"QUATERNIONIDENTITY", 0, [](const std::vector<Value>&) {
        return Value::from_map(Quaternion_to_Map(QuaternionIdentity()));
    }});
    R.add("QUATERNIONADD", NativeFn{"QUATERNIONADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionAdd(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }});
    R.add("QUATERNIONSUBTRACT", NativeFn{"QUATERNIONSUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionSubtract(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }});
    R.add("QUATERNIONLENGTH", NativeFn{"QUATERNIONLENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(QuaternionLength(Value_to_Quaternion(a[0])));
    }});
    R.add("QUATERNIONNORMALIZE", NativeFn{"QUATERNIONNORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionNormalize(Value_to_Quaternion(a[0]))));
    }});
    R.add("QUATERNIONINVERT", NativeFn{"QUATERNIONINVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionInvert(Value_to_Quaternion(a[0]))));
    }});
    R.add("QUATERNIONMULTIPLY", NativeFn{"QUATERNIONMULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionMultiply(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }});
    R.add("QUATERNIONLERP", NativeFn{"QUATERNIONLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionLerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), a[2].as_number())));
    }});
    R.add("QUATERNIONNLERP", NativeFn{"QUATERNIONNLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionNlerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), a[2].as_number())));
    }});
    R.add("QUATERNIONSLERP", NativeFn{"QUATERNIONSLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionSlerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), a[2].as_number())));
    }});
    R.add("QUATERNIONFROMVECTOR3TOVECTOR3", NativeFn{"QUATERNIONFROMVECTOR3TOVECTOR3", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromVector3ToVector3(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }});
    R.add("QUATERNIONFROMMATRIX", NativeFn{"QUATERNIONFROMMATRIX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromMatrix(Value_to_Matrix(a[0]))));
    }});
    R.add("QUATERNIONTOMATRIX", NativeFn{"QUATERNIONTOMATRIX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(QuaternionToMatrix(Value_to_Quaternion(a[0]))));
    }});
    R.add("QUATERNIONFROMAXISANGLE", NativeFn{"QUATERNIONFROMAXISANGLE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromAxisAngle(Value_to_Vector3(a[0]), a[1].as_number())));
    }});
}

} // namespace bas

