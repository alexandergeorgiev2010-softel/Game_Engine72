#include "RaySensor.h"
#include "Engine/Physics/AABB.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace Engine {

    RaySensor::RaySensor(float maxDistance)
        : m_ray(Vec3(), Vec3()),
          m_maxDistance(maxDistance),
          m_hitDistance(maxDistance),
          m_hasHit(false)
    {
    }

    void RaySensor::UpdateRay(const Vec3& origin, const Vec3& direction)
    {
        m_ray = Ray(origin, direction);
    }

}
