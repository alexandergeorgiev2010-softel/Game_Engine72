#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Physics/AABB.h"

namespace Engine {

struct Obstacle {
    Vec3 position;
    Vec3 scale;
    AABB bounds;

    Obstacle(const Vec3& position, const Vec3& scale)
        : position(position),
          scale(scale),
          bounds(position - scale, position + scale)
    {
    }
};

}
