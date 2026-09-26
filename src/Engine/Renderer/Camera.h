#pragma once

#include <Engine/Math/Vec3.h>
#include <Engine/Math/Mat4.h>

namespace Engine {
    class Camera {
        public:
            explicit Camera(const Vec3& position);

            Mat4 getViewMatrix() const;
        
        private:
            Vec3 m_position;
    };
}