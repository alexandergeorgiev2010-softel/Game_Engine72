#pragma once

#include "Engine/Sensors/Ray.h"

namespace Engine {

    class World;

    class RaySensor {
        public:
            RaySensor(float maxDistance);

            void UpdateRay(const Vec3& origin, const Vec3& direction);

            void Update(const World& world);
    
            float GetDistance() const {
                return m_hitDistance;
            }

            bool HasHit() const {
                return m_hasHit;
            }
            
            const Vec3& GetOrigin() const {
                return m_ray.m_origin;
            }

            const Vec3& GetDirection() const {
                return m_ray.m_direction;
            }

        private:
            Ray m_ray;
            float m_maxDistance;
            float m_hitDistance;
            bool m_hasHit;
    };
}