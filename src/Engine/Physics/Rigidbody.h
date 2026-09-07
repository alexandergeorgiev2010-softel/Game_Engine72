#pragma once
#include "Engine/Math/Vec3.h"


namespace Engine {
class RigidBody {
    public:
        RigidBody(Vec3 position, Vec3 velocity, float mass, bool isResting)
            : m_position(position), m_velocity(velocity), m_mass(mass), m_isResting(isResting) {}
        
        void update(float deltaTime);

        Vec3& get_m_position(){
            return m_position;
        }

        Vec3& get_m_velocity(){
            return m_velocity;
        }

        float& get_m_mass(){
            return m_mass;
        }

        bool& get_m_isResting(){
            return m_isResting;
        }

        float get_m_restitution() {
            return m_restitution;
        }

        float get_m_bouncing_threshold(){
            return m_bouncing_threshold;
        }


    private:
        Vec3 m_position;
        Vec3 m_velocity;
      

        float m_mass;
        float m_restitution = 0.7f;
        float m_bouncing_threshold = 0.05f;

        bool m_isResting;
        


};

}

