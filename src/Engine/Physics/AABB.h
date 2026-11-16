#pragma once

#include "Engine/Math/Vec3.h"

namespace Engine {

struct AABB {
    Vec3 minimum;
    Vec3 maximum;

    AABB(const Vec3&min, const Vec3& max)
        :minimum(min), maximum(max) {}

    bool intersects(const AABB& other) const {
        return (minimum.x <= other.maximum.x && maximum.x >= other.minimum.x) &&
               (minimum.y <= other.maximum.y && maximum.y >= other.minimum.y) &&
               (minimum.z <= other.maximum.z && maximum.z >= other.minimum.z);
    }


};

}