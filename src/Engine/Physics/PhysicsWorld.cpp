#include "Engine/Physics/PhysicsWorld.h"
#include <cmath>

namespace Engine {

    void PhysicsWorld::update(float deltaTime, RigidBody& rigidBody, float groundHeight) 
    {
        if (!rigidBody.get_m_isResting()) {
            rigidBody.get_m_velocity() += m_gravity * deltaTime;
            rigidBody.get_m_position() += rigidBody.get_m_velocity() * deltaTime;
        }

        if (rigidBody.get_m_position().y < groundHeight) {
            rigidBody.get_m_position().y = groundHeight;

            if (std::abs(rigidBody.get_m_velocity().y) <
                rigidBody.get_m_bouncing_threshold()) {

                rigidBody.get_m_velocity().y = 0.0f;
                rigidBody.get_m_isResting() = true;

            } else {
                rigidBody.get_m_velocity().y =
                    -rigidBody.get_m_velocity().y *
                    rigidBody.get_m_restitution();
            }
        }
    }

}