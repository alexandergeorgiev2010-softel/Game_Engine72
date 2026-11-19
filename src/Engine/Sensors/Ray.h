#pragma once

#include "Math/Vec3.h"

namespace Engine {

    struct Ray
    {
        Vec3 m_origin;
        Vec3 m_direction;

        Ray(const Vec3& origin, const Vec3& direction);
    };

}