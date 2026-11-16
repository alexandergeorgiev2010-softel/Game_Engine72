
#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Physics/Rigidbody.h"
#include "Engine/Physics/PhysicsWorld.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/World/WorldObject.h"
#include "Engine/Physics/AABB.h"

namespace Engine {

class Robot {
public:
    Robot(
        const Mesh& cube,
        const Mesh& sphere,
        Shader& shader,
        const Vec3& position
    );

    void update(float deltaTime, PhysicsWorld& physicsWorld);
    void draw() const;

    void moveForward(float amount);
    void moveBackward(float amount);
    void rotate(float amount);

    Vec3 getPosition() const;
    float getRotation() const;

private:
    RigidBody m_rigidBody;
    float m_rotation = 0.0f;
    Shader &m_shader;

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

    AABB getAABB() const;
    void updatePartTransforms();
};

}
