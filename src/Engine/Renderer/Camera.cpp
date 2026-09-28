#include "Engine/Renderer/Camera.h"
#include "Engine/Math/Vec3.h"
#include <cmath>

namespace Engine {
    Camera::Camera(const Vec3& position)
        : m_position(position),
          m_yaw(-3.14159265 / 2.0f),
          m_pitch(0.0f),
          m_front(0.0f, 0.0f, -1.0f)
{
    UpdateDirection();
}

    Mat4 Camera::getViewMatrix() const {
        Mat4 view;

        view.m[0][0] = m_right.x;
        view.m[0][1] = m_right.y;
        view.m[0][2] = m_right.z;
        view.m[0][3] = -m_right.dot(m_position);

        view.m[1][0] = m_up.x;
        view.m[1][1] = m_up.y;
        view.m[1][2] = m_up.z;
        view.m[1][3] = -m_up.dot(m_position);

        view.m[2][0] = -m_front.x;
        view.m[2][1] = -m_front.y;
        view.m[2][2] = -m_front.z;
        view.m[2][3] = m_front.dot(m_position);

        view.m[3][0] = 0.0f;
        view.m[3][1] = 0.0f;
        view.m[3][2] = 0.0f;
        view.m[3][3] = 1.0f;

        return view;
    }

    void Camera::UpdateDirection() {
        m_front.x = std::cos(m_yaw) * std::cos(m_pitch);
        m_front.y = std::sin(m_pitch);
        m_front.z = std::sin(m_yaw) * std::cos(m_pitch);

        m_front = m_front.normalized();

        Vec3 WorldUp(0.0f, 1.0f, 0.0f);

        m_right = WorldUp.cross(m_front).normalized();
        m_up = m_front.cross(m_right).normalized();

    }

    void Camera::Rotate(float yawOffset, float pitchOffset) {
        m_yaw += yawOffset;
        m_pitch += pitchOffset;

        UpdateDirection();

        if (m_pitch > 1.5f) {
            m_pitch = 1.5f;
        }

        if (m_pitch < -1.5f) {
            m_pitch = -1.5f;
        }
    }

    void Camera::Move(const Vec3& offset) {
        m_position += offset;
    }

    Vec3 Camera::getfront() const {
        return m_front;
    }

    Vec3 Camera::getright() const {
        return m_right;
    }

    void Camera::MoveForward(float amount) {
        m_position += m_front * amount;
    }

    void Camera::MoveBackward(float amount) {
        m_position -= m_front * amount;
    }

    void Camera::MoveRight(float amount) {
        m_position += m_right * amount;
    }

    void Camera::MoveLeft(float amount) {
        m_position -= m_right * amount;
    }
}
