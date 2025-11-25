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
    R.add_with_policy("VECTOR2ZERO", NativeFn{"VECTOR2ZERO", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector2_to_Map(Vector2Zero()));
    }}, true);
    R.add_with_policy("VECTOR2ONE", NativeFn{"VECTOR2ONE", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector2_to_Map(Vector2One()));
    }}, true);
    R.add_with_policy("VECTOR2ADD", NativeFn{"VECTOR2ADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Add(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR2ADDVALUE", NativeFn{"VECTOR2ADDVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2AddValue(Value_to_Vector2(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2SUBTRACT", NativeFn{"VECTOR2SUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Subtract(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR2SUBTRACTVALUE", NativeFn{"VECTOR2SUBTRACTVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2SubtractValue(Value_to_Vector2(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2LENGTH", NativeFn{"VECTOR2LENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Length(Value_to_Vector2(a[0])));
    }}, true);
    R.add_with_policy("VECTOR2DOTPRODUCT", NativeFn{"VECTOR2DOTPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2DotProduct(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }}, true);
    R.add_with_policy("VECTOR2DISTANCE", NativeFn{"VECTOR2DISTANCE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Distance(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }}, true);
    R.add_with_policy("VECTOR2ANGLE", NativeFn{"VECTOR2ANGLE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2Angle(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }}, true);
    R.add_with_policy("VECTOR2SCALE", NativeFn{"VECTOR2SCALE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Scale(Value_to_Vector2(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2MULTIPLY", NativeFn{"VECTOR2MULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Multiply(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR2NEGATE", NativeFn{"VECTOR2NEGATE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Negate(Value_to_Vector2(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR2DIVIDE", NativeFn{"VECTOR2DIVIDE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Divide(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR2NORMALIZE", NativeFn{"VECTOR2NORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Normalize(Value_to_Vector2(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR2LERP", NativeFn{"VECTOR2LERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Lerp(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2LENGTHSQR", NativeFn{"VECTOR2LENGTHSQR", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2LengthSqr(Value_to_Vector2(a[0])));
    }}, true);
    R.add_with_policy("VECTOR2DISTANCESQR", NativeFn{"VECTOR2DISTANCESQR", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector2DistanceSqr(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }}, true);
    R.add_with_policy("VECTOR2REFLECT", NativeFn{"VECTOR2REFLECT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Reflect(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR2ROTATE", NativeFn{"VECTOR2ROTATE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Rotate(Value_to_Vector2(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2MOVETOWARDS", NativeFn{"VECTOR2MOVETOWARDS", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2MoveTowards(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2INVERT", NativeFn{"VECTOR2INVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Invert(Value_to_Vector2(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR2CLAMP", NativeFn{"VECTOR2CLAMP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2Clamp(Value_to_Vector2(a[0]), Value_to_Vector2(a[1]), Value_to_Vector2(a[2]))));
    }}, true);
    R.add_with_policy("VECTOR2CLAMPVALUE", NativeFn{"VECTOR2CLAMPVALUE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector2_to_Map(Vector2ClampValue(Value_to_Vector2(a[0]), static_cast<float>(a[1].as_number()), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR2EQUALS", NativeFn{"VECTOR2EQUALS", 2, [](const std::vector<Value>& a) {
        return Value::from_bool(Vector2Equals(Value_to_Vector2(a[0]), Value_to_Vector2(a[1])));
    }}, true);

    // --- Vector3 ---
    R.add_with_policy("VECTOR3ZERO", NativeFn{"VECTOR3ZERO", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector3_to_Map(Vector3Zero()));
    }}, true);
    R.add_with_policy("VECTOR3ONE", NativeFn{"VECTOR3ONE", 0, [](const std::vector<Value>&) {
        return Value::from_map(Vector3_to_Map(Vector3One()));
    }}, true);
    R.add_with_policy("VECTOR3ADD", NativeFn{"VECTOR3ADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Add(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3ADDVALUE", NativeFn{"VECTOR3ADDVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3AddValue(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR3SUBTRACT", NativeFn{"VECTOR3SUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Subtract(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3SUBTRACTVALUE", NativeFn{"VECTOR3SUBTRACTVALUE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3SubtractValue(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR3SCALE", NativeFn{"VECTOR3SCALE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Scale(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR3MULTIPLY", NativeFn{"VECTOR3MULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Multiply(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3CROSSPRODUCT", NativeFn{"VECTOR3CROSSPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3CrossProduct(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3PERPENDICULAR", NativeFn{"VECTOR3PERPENDICULAR", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Perpendicular(Value_to_Vector3(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR3LENGTH", NativeFn{"VECTOR3LENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3Length(Value_to_Vector3(a[0])));
    }}, true);
    R.add_with_policy("VECTOR3LENGTHSQR", NativeFn{"VECTOR3LENGTHSQR", 1, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3LengthSqr(Value_to_Vector3(a[0])));
    }}, true);
    R.add_with_policy("VECTOR3DOTPRODUCT", NativeFn{"VECTOR3DOTPRODUCT", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3DotProduct(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }}, true);
    R.add_with_policy("VECTOR3DISTANCE", NativeFn{"VECTOR3DISTANCE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3Distance(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }}, true);
    R.add_with_policy("VECTOR3DISTANCESQR", NativeFn{"VECTOR3DISTANCESQR", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3DistanceSqr(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }}, true);
    R.add_with_policy("VECTOR3ANGLE", NativeFn{"VECTOR3ANGLE", 2, [](const std::vector<Value>& a) {
        return Value::from_number(Vector3Angle(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }}, true);
    R.add_with_policy("VECTOR3NEGATE", NativeFn{"VECTOR3NEGATE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Negate(Value_to_Vector3(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR3DIVIDE", NativeFn{"VECTOR3DIVIDE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Divide(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3NORMALIZE", NativeFn{"VECTOR3NORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Normalize(Value_to_Vector3(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR3LERP", NativeFn{"VECTOR3LERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Lerp(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR3REFLECT", NativeFn{"VECTOR3REFLECT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Reflect(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3MIN", NativeFn{"VECTOR3MIN", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Min(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3MAX", NativeFn{"VECTOR3MAX", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Max(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("VECTOR3BARYCENTER", NativeFn{"VECTOR3BARYCENTER", 4, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Barycenter(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), Value_to_Vector3(a[2]), Value_to_Vector3(a[3]))));
    }}, true);
    R.add_with_policy("VECTOR3INVERT", NativeFn{"VECTOR3INVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Invert(Value_to_Vector3(a[0]))));
    }}, true);
    R.add_with_policy("VECTOR3CLAMP", NativeFn{"VECTOR3CLAMP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Clamp(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), Value_to_Vector3(a[2]))));
    }}, true);
    R.add_with_policy("VECTOR3CLAMPVALUE", NativeFn{"VECTOR3CLAMPVALUE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3ClampValue(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("VECTOR3EQUALS", NativeFn{"VECTOR3EQUALS", 2, [](const std::vector<Value>& a) {
        return Value::from_bool(Vector3Equals(Value_to_Vector3(a[0]), Value_to_Vector3(a[1])));
    }}, true);
    R.add_with_policy("VECTOR3REFRACT", NativeFn{"VECTOR3REFRACT", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(Vector3Refract(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);

    // --- Matrix ---
    R.add_with_policy("MATRIXIDENTITY", NativeFn{"MATRIXIDENTITY", 0, [](const std::vector<Value>&) {
        return Value::from_map(Matrix_to_Map(MatrixIdentity()));
    }}, true);
    R.add_with_policy("MATRIXADD", NativeFn{"MATRIXADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixAdd(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }}, true);
    R.add_with_policy("MATRIXSUBTRACT", NativeFn{"MATRIXSUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixSubtract(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }}, true);
    R.add_with_policy("MATRIXMULTIPLY", NativeFn{"MATRIXMULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixMultiply(Value_to_Matrix(a[0]), Value_to_Matrix(a[1]))));
    }}, true);
    R.add_with_policy("MATRIXTRANSLATE", NativeFn{"MATRIXTRANSLATE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixTranslate(static_cast<float>(a[0].as_number()), static_cast<float>(a[1].as_number()), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXROTATE", NativeFn{"MATRIXROTATE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotate(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXROTATEX", NativeFn{"MATRIXROTATEX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateX(static_cast<float>(a[0].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXROTATEY", NativeFn{"MATRIXROTATEY", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateY(static_cast<float>(a[0].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXROTATEZ", NativeFn{"MATRIXROTATEZ", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixRotateZ(static_cast<float>(a[0].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXSCALE", NativeFn{"MATRIXSCALE", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixScale(static_cast<float>(a[0].as_number()), static_cast<float>(a[1].as_number()), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("MATRIXINVERT", NativeFn{"MATRIXINVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixInvert(Value_to_Matrix(a[0]))));
    }}, true);
    R.add_with_policy("MATRIXTRANSPOSE", NativeFn{"MATRIXTRANSPOSE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(MatrixTranspose(Value_to_Matrix(a[0]))));
    }}, true);
    R.add_with_policy("MATRIXDETERMINANT", NativeFn{"MATRIXDETERMINANT", 1, [](const std::vector<Value>& a) {
        return Value::from_number(MatrixDeterminant(Value_to_Matrix(a[0])));
    }}, true);
    R.add_with_policy("MATRIXTRACE", NativeFn{"MATRIXTRACE", 1, [](const std::vector<Value>& a) {
        return Value::from_number(MatrixTrace(Value_to_Matrix(a[0])));
    }}, true);

    // --- Quaternion ---
    R.add_with_policy("QUATERNIONIDENTITY", NativeFn{"QUATERNIONIDENTITY", 0, [](const std::vector<Value>&) {
        return Value::from_map(Quaternion_to_Map(QuaternionIdentity()));
    }}, true);
    R.add_with_policy("QUATERNIONADD", NativeFn{"QUATERNIONADD", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionAdd(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }}, true);
    R.add_with_policy("QUATERNIONSUBTRACT", NativeFn{"QUATERNIONSUBTRACT", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionSubtract(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }}, true);
    R.add_with_policy("QUATERNIONLENGTH", NativeFn{"QUATERNIONLENGTH", 1, [](const std::vector<Value>& a) {
        return Value::from_number(QuaternionLength(Value_to_Quaternion(a[0])));
    }}, true);
    R.add_with_policy("QUATERNIONNORMALIZE", NativeFn{"QUATERNIONNORMALIZE", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionNormalize(Value_to_Quaternion(a[0]))));
    }}, true);
    R.add_with_policy("QUATERNIONINVERT", NativeFn{"QUATERNIONINVERT", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionInvert(Value_to_Quaternion(a[0]))));
    }}, true);
    R.add_with_policy("QUATERNIONMULTIPLY", NativeFn{"QUATERNIONMULTIPLY", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionMultiply(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]))));
    }}, true);
    R.add_with_policy("QUATERNIONLERP", NativeFn{"QUATERNIONLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionLerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("QUATERNIONNLERP", NativeFn{"QUATERNIONNLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionNlerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("QUATERNIONSLERP", NativeFn{"QUATERNIONSLERP", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionSlerp(Value_to_Quaternion(a[0]), Value_to_Quaternion(a[1]), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("QUATERNIONFROMVECTOR3TOVECTOR3", NativeFn{"QUATERNIONFROMVECTOR3TOVECTOR3", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromVector3ToVector3(Value_to_Vector3(a[0]), Value_to_Vector3(a[1]))));
    }}, true);
    R.add_with_policy("QUATERNIONFROMMATRIX", NativeFn{"QUATERNIONFROMMATRIX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromMatrix(Value_to_Matrix(a[0]))));
    }}, true);
    R.add_with_policy("QUATERNIONTOMATRIX", NativeFn{"QUATERNIONTOMATRIX", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Matrix_to_Map(QuaternionToMatrix(Value_to_Quaternion(a[0]))));
    }}, true);
    R.add_with_policy("QUATERNIONFROMAXISANGLE", NativeFn{"QUATERNIONFROMAXISANGLE", 2, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromAxisAngle(Value_to_Vector3(a[0]), static_cast<float>(a[1].as_number()))));
    }}, true);
    R.add_with_policy("QUATERNIONFROMEULER", NativeFn{"QUATERNIONFROMEULER", 3, [](const std::vector<Value>& a) {
        return Value::from_map(Quaternion_to_Map(QuaternionFromEuler(static_cast<float>(a[0].as_number()), static_cast<float>(a[1].as_number()), static_cast<float>(a[2].as_number()))));
    }}, true);
    R.add_with_policy("QUATERNIONTOEULER", NativeFn{"QUATERNIONTOEULER", 1, [](const std::vector<Value>& a) {
        return Value::from_map(Vector3_to_Map(QuaternionToEuler(Value_to_Quaternion(a[0]))));
    }}, true);
}

} // namespace bas

