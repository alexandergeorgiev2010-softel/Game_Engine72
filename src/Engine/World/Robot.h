#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Physics/Rigidbody.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/World/WorldObject.h"
#include "Engine/Physics/AABB.h"
#include "Engine/World/World.h"
#include "Engine/Sensors/RaySensor.h"

#include <array>

namespace Engine {

class Robot {
public:
    Robot(const Mesh& cube, const Mesh& sphere, Shader& shader, const Vec3& position);

    void update(float deltaTime, PhysicsWorld& physicsWorld, const World& world);
    void reset(const Vec3& position, float rotation, const World& world);
    void draw() const;

    void moveForward(float amount, const World& world);
    void moveBackward(float amount, const World& world);
    void rotate(float amount);

    Vec3 getPosition() const;
    float getRotation() const;
    std::array<float, 9> getObservations(const World& world) const;

    const RaySensor& getRaySensor() const {
        return m_raySensors[2];
    }

    const std::array<RaySensor, 5>& getRaySensors() const {
        return m_raySensors;
    }

private:
    RigidBody m_rigidBody;
    float m_rotation = 0.0f;
    Shader& m_shader;

    std::array<RaySensor, 5> m_raySensors{
        RaySensor(20.0f),
        RaySensor(20.0f),
        RaySensor(20.0f),
        RaySensor(20.0f),
        RaySensor(20.0f)
    };

    WorldObject m_torso;
    WorldObject m_head;

    WorldObject m_leftEye;
    WorldObject m_rightEye;

    WorldObject m_leftPupil;
    WorldObject m_rightPupil;

    WorldObject m_leftShoulder;
    WorldObject m_rightShoulder;

    WorldObject m_leftArm;
    WorldObject m_rightArm;

    WorldObject m_leftElbow;
    WorldObject m_rightElbow;

    WorldObject m_leftLeg;
    WorldObject m_rightLeg;

    AABB getAABBAt(const Vec3& position) const;
    bool canOccupy(const Vec3& position, const World& world) const;

    void updatePartTransforms();
    void updateSensors(const World& world);
};

}
