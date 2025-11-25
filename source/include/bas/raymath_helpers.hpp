#pragma once
#include "bas/value.hpp"
#include "raylib.h"
#include <stdexcept>

namespace bas {

// --- Conversion helpers for raylib types ---

// Vector2 <-> Value::Map
inline Value::Map Vector2_to_Map(Vector2 v) {
    return {{ "x", Value::from_number(v.x) }, { "y", Value::from_number(v.y) }};
}

inline Vector2 Map_to_Vector2(const Value::Map& m) {
    Vector2 v = { 0.0f, 0.0f };
    if (auto it = m.find("x"); it != m.end()) v.x = (float)it->second.as_number();
    if (auto it = m.find("y"); it != m.end()) v.y = (float)it->second.as_number();
    return v;
}

inline Vector2 Value_to_Vector2(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Vector2");
    return Map_to_Vector2(val.as_map());
}

// Vector3 <-> Value::Map
inline Value::Map Vector3_to_Map(Vector3 v) {
    return {{ "x", Value::from_number(v.x) }, { "y", Value::from_number(v.y) }, { "z", Value::from_number(v.z) }};
}

inline Vector3 Map_to_Vector3(const Value::Map& m) {
    Vector3 v = { 0.0f, 0.0f, 0.0f };
    if (auto it = m.find("x"); it != m.end()) v.x = (float)it->second.as_number();
    if (auto it = m.find("y"); it != m.end()) v.y = (float)it->second.as_number();
    if (auto it = m.find("z"); it != m.end()) v.z = (float)it->second.as_number();
    return v;
}

inline Vector3 Value_to_Vector3(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Vector3");
    return Map_to_Vector3(val.as_map());
}

// Vector4 <-> Value::Map
inline Value::Map Vector4_to_Map(Vector4 v) {
    return {{ "x", Value::from_number(v.x) }, { "y", Value::from_number(v.y) }, { "z", Value::from_number(v.z) }, { "w", Value::from_number(v.w) }};
}

inline Vector4 Map_to_Vector4(const Value::Map& m) {
    Vector4 v = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (auto it = m.find("x"); it != m.end()) v.x = (float)it->second.as_number();
    if (auto it = m.find("y"); it != m.end()) v.y = (float)it->second.as_number();
    if (auto it = m.find("z"); it != m.end()) v.z = (float)it->second.as_number();
    if (auto it = m.find("w"); it != m.end()) v.w = (float)it->second.as_number();
    return v;
}

inline Vector4 Value_to_Vector4(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Vector4");
    return Map_to_Vector4(val.as_map());
}

// Quaternion is an alias for Vector4 in raylib
using ::Quaternion;
#define Quaternion_to_Map Vector4_to_Map
#define Map_to_Quaternion Map_to_Vector4
#define Value_to_Quaternion Value_to_Vector4

// Matrix <-> Value::Map
inline Value::Map Matrix_to_Map(const Matrix& mat) {
    return {
        {"m0", Value::from_number(mat.m0)}, {"m1", Value::from_number(mat.m1)}, {"m2", Value::from_number(mat.m2)}, {"m3", Value::from_number(mat.m3)},
        {"m4", Value::from_number(mat.m4)}, {"m5", Value::from_number(mat.m5)}, {"m6", Value::from_number(mat.m6)}, {"m7", Value::from_number(mat.m7)},
        {"m8", Value::from_number(mat.m8)}, {"m9", Value::from_number(mat.m9)}, {"m10", Value::from_number(mat.m10)}, {"m11", Value::from_number(mat.m11)},
        {"m12", Value::from_number(mat.m12)}, {"m13", Value::from_number(mat.m13)}, {"m14", Value::from_number(mat.m14)}, {"m15", Value::from_number(mat.m15)}
    };
}

inline Matrix Map_to_Matrix(const Value::Map& m) {
    Matrix mat = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    if (auto it = m.find("m0"); it != m.end()) mat.m0 = (float)it->second.as_number();
    if (auto it = m.find("m1"); it != m.end()) mat.m1 = (float)it->second.as_number();
    if (auto it = m.find("m2"); it != m.end()) mat.m2 = (float)it->second.as_number();
    if (auto it = m.find("m3"); it != m.end()) mat.m3 = (float)it->second.as_number();
    if (auto it = m.find("m4"); it != m.end()) mat.m4 = (float)it->second.as_number();
    if (auto it = m.find("m5"); it != m.end()) mat.m5 = (float)it->second.as_number();
    if (auto it = m.find("m6"); it != m.end()) mat.m6 = (float)it->second.as_number();
    if (auto it = m.find("m7"); it != m.end()) mat.m7 = (float)it->second.as_number();
    if (auto it = m.find("m8"); it != m.end()) mat.m8 = (float)it->second.as_number();
    if (auto it = m.find("m9"); it != m.end()) mat.m9 = (float)it->second.as_number();
    if (auto it = m.find("m10"); it != m.end()) mat.m10 = (float)it->second.as_number();
    if (auto it = m.find("m11"); it != m.end()) mat.m11 = (float)it->second.as_number();
    if (auto it = m.find("m12"); it != m.end()) mat.m12 = (float)it->second.as_number();
    if (auto it = m.find("m13"); it != m.end()) mat.m13 = (float)it->second.as_number();
    if (auto it = m.find("m14"); it != m.end()) mat.m14 = (float)it->second.as_number();
    if (auto it = m.find("m15"); it != m.end()) mat.m15 = (float)it->second.as_number();
    return mat;
}

inline Matrix Value_to_Matrix(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Matrix");
    return Map_to_Matrix(val.as_map());
}

} // namespace bas
