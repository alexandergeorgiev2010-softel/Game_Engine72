#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Math/Mat4.h"

namespace Engine {
    struct Transform {
        Vec3 position{0.0f, 0.0f, 0.0f};
        Vec3 rotation{0.0f, 0.0f, 0.0f};
        Vec3 scale{1.0f, 1.0f, 1.0f};

        Mat4 getMatrix() const {
            Mat4 translation = Mat4::Translation(position.x, position.y, position.z);

            Mat4 rotationX = Mat4::RotationX(rotation.x);
            Mat4 rotationY = Mat4::RotationY(rotation.y);
            Mat4 rotationZ = Mat4::RotationZ(rotation.z);

            Mat4 scaling = Mat4::Scale(scale.x, scale.y, scale.z);

            return translation * rotationZ * rotationY * rotationX * scaling;
        }
    };
}