#pragma once
#include <cmath>

namespace bas {

struct Vector3D {
    float x, y, z;
    
    Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3D operator+(const Vector3D& other) const { 
        return Vector3D(x + other.x, y + other.y, z + other.z); 
    }
    Vector3D operator-(const Vector3D& other) const { 
        return Vector3D(x - other.x, y - other.y, z - other.z); 
    }
    Vector3D operator*(float scalar) const { 
        return Vector3D(x * scalar, y * scalar, z * scalar); 
    }
    Vector3D operator-() const { 
        return Vector3D(-x, -y, -z); 
    }
    
    float length() const {
        return static_cast<float>(std::sqrt(x*x + y*y + z*z));
    }
    
    Vector3D normalized() const {
        float len = length();
        if (len == 0) return Vector3D(0, 0, 0);
        return Vector3D(x/len, y/len, z/len);
    }
    
    float dot(const Vector3D& other) const {
        return x*other.x + y*other.y + z*other.z;
    }
    
    Vector3D cross(const Vector3D& other) const {
        return Vector3D(
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x
        );
    }
};

} // namespace bas
