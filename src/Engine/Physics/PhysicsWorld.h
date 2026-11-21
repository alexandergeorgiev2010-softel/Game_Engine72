#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Physics/Rigidbody.h"

namespace Engine {

    class PhysicsWorld {
    public:
        PhysicsWorld() = default;

        void update(float deltaTime, RigidBody& rigidBody, float groundHeight);

        Vec3 get_m_gravity() const {
            return m_gravity;
        }

    private:
        Vec3 m_gravity = {0.0f, -9.81f, 0.0f};
    };

}