#include "RaySensor.h"
#include "Engine/Physics/AABB.h"
#include "Engine/World/World.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace Engine {

    static bool RayIntersectsAABB(const Ray& ray, const AABB& bounds, float maxDistance, float& hitDistance)
    {
        if (!std::isfinite(maxDistance) || maxDistance < 0.0f) {
            return false;
        }
        
        if (!std::isfinite(ray.m_origin.x) ||
            !std::isfinite(ray.m_origin.y) ||
            !std::isfinite(ray.m_origin.z) ||
            !std::isfinite(ray.m_direction.x) ||
            !std::isfinite(ray.m_direction.y) ||
            !std::isfinite(ray.m_direction.z)) {
            return false;
        }

        if (ray.m_direction.length() <= 0.000001f) {
            return false;
        }

        if (!std::isfinite(bounds.minimum.x) ||
            !std::isfinite(bounds.minimum.y) ||
            !std::isfinite(bounds.minimum.z) ||
            !std::isfinite(bounds.maximum.x) ||
            !std::isfinite(bounds.maximum.y) ||
            !std::isfinite(bounds.maximum.z)) {
            return false;
        }

        if (bounds.minimum.x > bounds.maximum.x ||
            bounds.minimum.y > bounds.maximum.y ||
            bounds.minimum.z > bounds.maximum.z) {
            return false;
        }

        float tEnter = -std::numeric_limits<float>::infinity();
        float tExit = std::numeric_limits<float>::infinity();

        auto checkAxis = [&tEnter, &tExit](float origin, float direction, float min, float max) {
            if (std::abs(direction) < 0.000001f) {
                return origin >= min && origin <= max;
            }

            float t1 = (min - origin) / direction;
            float t2 = (max - origin) / direction;

            if (t1 > t2) {
                std::swap(t1, t2);
            }

            tEnter = std::max(tEnter, t1);
            tExit = std::min(tExit, t2);

            return tEnter <= tExit;
        };

        if (!checkAxis(ray.m_origin.x, ray.m_direction.x, bounds.minimum.x, bounds.maximum.x)) {
            return false;
        }

        if (!checkAxis(ray.m_origin.y, ray.m_direction.y, bounds.minimum.y, bounds.maximum.y)) {
            return false;
        }

        if (!checkAxis(ray.m_origin.z, ray.m_direction.z, bounds.minimum.z, bounds.maximum.z)) {
            return false;
        }

        if (tExit < 0.0f) {
            return false;
        }

        hitDistance = std::max(tEnter, 0.0f);

        return hitDistance <= maxDistance;
    }

    RaySensor::RaySensor(float maxDistance)
        : m_ray(Vec3(), Vec3()),
          m_maxDistance(std::isfinite(maxDistance) ? std::max(maxDistance, 0.0f) : 0.0f),
          m_hitDistance(m_maxDistance),
          m_hasHit(false)
    {
    }

    void RaySensor::UpdateRay(const Vec3& origin, const Vec3& direction)
    {
        m_ray = Ray(origin, direction);

        m_hitDistance = m_maxDistance;
        m_hasHit = false;
    }

    void RaySensor::Update(const World& world) {
        m_hitDistance = m_maxDistance;
        m_hasHit = false;

        for (const Obstacle& obstacle: world.getObstacles()) {
            float distance = m_maxDistance;

            if (RayIntersectsAABB(m_ray, obstacle.bounds, m_maxDistance, distance)) {
                if (distance < m_hitDistance) {
                    m_hitDistance = distance;
                    m_hasHit = true;
                }
            }
        }
    }

}
