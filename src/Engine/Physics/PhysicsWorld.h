#pragma once
#include "Engine/Math/Vec3.h"
#include "Engine/Physics/Rigidbody.h"

namespace Engine {
    class PhysicsWorld {
        public:
            PhysicsWorld() = default;

            void update(float deltaTime, RigidBody& m_RigidBody);
            
            float get_m_groundHeight() const {
                return m_groundHeight;
            }

            Vec3 get_m_gravity() const {
                return m_gravity;
            }
            

        private:

            Vec3 m_gravity = {0.0f, -9.81f, 0.0f};
            const float m_groundHeight = 0.0f;

    };
}