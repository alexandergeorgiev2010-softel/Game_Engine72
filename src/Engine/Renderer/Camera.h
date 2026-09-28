#pragma once

#include <Engine/Math/Vec3.h>
#include <Engine/Math/Mat4.h>

namespace Engine {
    class Camera {
        public:
            explicit Camera(const Vec3& position);

            Mat4 getViewMatrix() const;

            void Rotate(float yawOffset, float pitchOffset);
            void Move(const Vec3& other);

            Vec3 getfront() const;
            Vec3 getright() const;

            void MoveForward(float amount);
            void MoveBackward(float amont);
            void MoveRight(float amount);
            void MoveLeft(float amount);
        
        private:
            void UpdateDirection();

            Vec3 m_position;

            float m_yaw = 0.0f;
            float m_pitch = 0.0f;

            Vec3 m_front{0.0f, 0.0f, -1.0f};
            Vec3 m_right{1.0f, 0.0f, 0.0f};
            Vec3 m_up{0.0f, 1.0f, 0.0f};
    };
}