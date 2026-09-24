#pragma once
#include <cmath>

namespace Engine {

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3(float inputX, float inputY, float inputZ): x(inputX), y(inputY), z(inputZ) {}

    Vec3 operator+(const Vec3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vec3 operator*(float scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vec3& operator+=(const Vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3 operator-(const Vec3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vec3& operator-=(const Vec3& other) 
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3 operator/(float scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    float length() const 
    {
        return std::sqrt(x * x + y * y + z * z);
    }
};

} 
