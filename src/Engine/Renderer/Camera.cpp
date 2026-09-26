#include "Engine/Renderer/Camera.h"

namespace Engine {
    Camera::Camera(const Vec3& position)
        :m_position(position)
    {
    }


    Mat4 Camera::getViewMatrix() const {
        return Mat4::Translation(
            -m_position.x,
            -m_position.y,
            -m_position.z
        );
    }
}
