#include "Engine/Physics/PhysicsWorld.h"
#include <cmath>

namespace Engine {
    void PhysicsWorld::update(float deltaTime, RigidBody& m_RigidBody) {
        if (!m_RigidBody.get_m_isResting()) {
            m_RigidBody.get_m_velocity() += m_gravity * deltaTime;
            m_RigidBody.get_m_position() += m_RigidBody.get_m_velocity() * deltaTime;
        }

        if (m_RigidBody.get_m_position().y < m_groundHeight) {
            m_RigidBody.get_m_position().y = m_groundHeight;

            if (std::abs(m_RigidBody.get_m_velocity().y) < m_RigidBody.get_m_bouncing_threshold()) {
                m_RigidBody.get_m_velocity().y = 0.0f;
                m_RigidBody.get_m_isResting() = true;
            } else {
                m_RigidBody.get_m_velocity().y = -m_RigidBody.get_m_velocity().y * m_RigidBody.get_m_restitution();
            }
        }
    }
}