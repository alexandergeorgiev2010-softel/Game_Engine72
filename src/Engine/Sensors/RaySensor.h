#pragma once

#include "Engine/Sensors/Ray.h"

namespace Engine {
    class RaySensor {
        public:
            RaySensor(float maxDistance);

            void UpdateRay(const Vec3& origin, const Vec3& direction);

            float GetDistance() const {
                return m_hitDistance;
            }

            bool HasHit() const {
                return m_hasHit;
            }

        private:
            Ray m_ray;
            float m_maxDistance;
            float m_hitDistance;
            bool m_hasHit;
    };
}