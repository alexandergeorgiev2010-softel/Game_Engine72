#include "Engine/Physics/PhysicsWorld.h"

#include <cmath>

namespace Engine {

    void PhysicsWorld::update(
        float deltaTime,
        RigidBody& rigidBody,
        float groundHeight
    ) {
        if (deltaTime <= 0.0f) {
            return;
        }

        Vec3& position = rigidBody.get_m_position();
        Vec3& velocity = rigidBody.get_m_velocity();
        bool& isResting = rigidBody.get_m_isResting();

        
        if (isResting) {
            position.y = groundHeight;
            velocity.y = 0.0f;
            return;
        }

        
        velocity += m_gravity * deltaTime;
        position += velocity * deltaTime;

        if (position.y <= groundHeight) {
            position.y = groundHeight;

            if (std::abs(velocity.y) < rigidBody.get_m_bouncing_threshold()) {
                velocity.y = 0.0f;
                isResting = true;
            }
            else {
                velocity.y = -velocity.y * rigidBody.get_m_restitution();
            }
        }
    }

}

