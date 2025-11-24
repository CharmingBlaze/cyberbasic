#pragma once
#include "bas/value.hpp"
#include "raylib.h"
#include <stdexcept>
#include <string>
#include <vector>

namespace bas {

// --- Conversion helpers for raygui types ---

// Rectangle <-> Value::Map
inline Value::Map Rectangle_to_Map(Rectangle r) {
    return {{ "x", Value::from_number(r.x) }, 
            { "y", Value::from_number(r.y) }, 
            { "width", Value::from_number(r.width) }, 
            { "height", Value::from_number(r.height) }};
}

inline Rectangle Map_to_Rectangle(const Value::Map& m) {
    Rectangle r = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (auto it = m.find("x"); it != m.end()) r.x = (float)it->second.as_number();
    if (auto it = m.find("y"); it != m.end()) r.y = (float)it->second.as_number();
    if (auto it = m.find("width"); it != m.end()) r.width = (float)it->second.as_number();
    if (auto it = m.find("height"); it != m.end()) r.height = (float)it->second.as_number();
    return r;
}

inline Rectangle Value_to_Rectangle(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Rectangle");
    return Map_to_Rectangle(val.as_map());
}

// Color <-> Value::Map
inline Value::Map Color_to_Map(Color c) {
    return {{ "r", Value::from_number(c.r) }, 
            { "g", Value::from_number(c.g) }, 
            { "b", Value::from_number(c.b) }, 
            { "a", Value::from_number(c.a) }};
}

inline Color Map_to_Color(const Value::Map& m) {
    Color c = { 0, 0, 0, 255 };
    if (auto it = m.find("r"); it != m.end()) c.r = (unsigned char)it->second.as_int();
    if (auto it = m.find("g"); it != m.end()) c.g = (unsigned char)it->second.as_int();
    if (auto it = m.find("b"); it != m.end()) c.b = (unsigned char)it->second.as_int();
    if (auto it = m.find("a"); it != m.end()) c.a = (unsigned char)it->second.as_int();
    return c;
}

inline Color Value_to_Color(const Value& val) {
    if (!val.is_map()) throw std::runtime_error("Expected a map for Color");
    return Map_to_Color(val.as_map());
}

// Helper to get Rectangle from 4 numbers
inline Rectangle make_rectangle(float x, float y, float width, float height) {
    return { x, y, width, height };
}

// Helper to get Color from 4 numbers (r, g, b, a)
inline Color make_color(int r, int g, int b, int a = 255) {
    return { (unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a };
}

} // namespace bas

