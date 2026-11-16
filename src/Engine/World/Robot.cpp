
#include "Engine/World/Robot.h"
#include <cmath>

namespace Engine {

namespace {

Vec3 rotateOffset(const Vec3& offset, float angle)
{
    const float c = std::cos(angle);
    const float s = std::sin(angle);

    return Vec3(
        offset.x * c + offset.z * s,
        offset.y,
        -offset.x * s + offset.z * c
    );
}

}

Robot::Robot(
    const Mesh& cube,
    const Mesh& sphere,
    Shader& shader,
    const Vec3& position
)
    : m_rigidBody(position, Vec3(0, 0, 0), 1.0f, true),
      m_shader(shader),
      m_torso(cube, shader),
      m_head(cube, shader),
      m_leftEye(sphere, shader),
      m_rightEye(sphere, shader),
      m_leftPupil(sphere, shader),
      m_rightPupil(sphere, shader),
      m_leftShoulder(sphere, shader),
      m_rightShoulder(sphere, shader),
      m_leftArm(cube, shader),
      m_rightArm(cube, shader),
      m_leftElbow(sphere, shader),
      m_rightElbow(sphere, shader),
      m_leftLeg(cube, shader),
      m_rightLeg(cube, shader)
{
    updatePartTransforms();
}

void Robot::update(float deltaTime, PhysicsWorld& physicsWorld)
{
    physicsWorld.update(deltaTime, m_rigidBody);
    updatePartTransforms();
}

void Robot::moveForward(float amount)
{
    Vec3& position = m_rigidBody.get_m_position();

    position.x += std::sin(m_rotation) * amount;
    position.z += std::cos(m_rotation) * amount;

    updatePartTransforms();
}

void Robot::moveBackward(float amount)
{
    moveForward(-amount);
}

void Robot::rotate(float amount)
{
    m_rotation += amount;
    updatePartTransforms();
}

Vec3 Robot::getPosition() const
{
    return m_rigidBody.get_m_position();
}

float Robot::getRotation() const
{
    return m_rotation;
}

AABB Robot::getAABB() const
{
    const Vec3& position = m_rigidBody.get_m_position();

    const float c = std::abs(std::cos(m_rotation));
    const float s = std::abs(std::sin(m_rotation));

    const float halfWidth = c * 3.2f + s * 1.8f;
    const float halfDepth = s * 3.2f + c * 1.8f;

    return AABB(
        position + Vec3(-halfWidth, 0.0f, -halfDepth),
        position + Vec3( halfWidth, 9.0f,  halfDepth)
    );
}

void Robot::updatePartTransforms()
{
    const Vec3 position = m_rigidBody.get_m_position();

    auto setPart = [&](WorldObject& part,
                       const Vec3& offset,
                       const Vec3& scale)
    {
        part.getTransform().position =
            position + rotateOffset(offset, m_rotation);

        part.getTransform().rotation = Vec3(0.0f, m_rotation, 0.0f);
        part.getTransform().scale = scale;
    };

    setPart(m_torso,
        Vec3(0.0f, 4.2f, 0.0f),
        Vec3(2.1f, 2.7f, 1.2f));

    setPart(m_head,
        Vec3(0.0f, 7.5f, 0.0f),
        Vec3(1.35f, 1.35f, 1.35f));

    setPart(m_leftEye,
        Vec3(-0.4f, 7.7f, 1.4f),
        Vec3(0.25f, 0.25f, 0.25f));

    setPart(m_rightEye,
        Vec3(0.4f, 7.7f, 1.4f),
        Vec3(0.25f, 0.25f, 0.25f));

    setPart(m_leftPupil,
        Vec3(-0.4f, 7.7f, 1.61f),
        Vec3(0.15f, 0.15f, 0.09f));

    setPart(m_rightPupil,
        Vec3(0.4f, 7.7f, 1.61f),
        Vec3(0.15f, 0.15f, 0.09f));

    setPart(m_leftShoulder,
        Vec3(-2.15f, 5.65f, 0.0f),
        Vec3(0.65f, 0.65f, 0.65f));

    setPart(m_rightShoulder,
        Vec3(2.15f, 5.65f, 0.0f),
        Vec3(0.65f, 0.65f, 0.65f));

    setPart(m_leftArm,
        Vec3(-2.55f, 4.25f, 0.0f),
        Vec3(0.55f, 1.15f, 0.55f));

    setPart(m_rightArm,
        Vec3(2.55f, 4.25f, 0.0f),
        Vec3(0.55f, 1.15f, 0.55f));

    setPart(m_leftElbow,
        Vec3(-2.55f, 3.65f, 0.25f),
        Vec3(0.43f, 0.43f, 0.43f));

    setPart(m_rightElbow,
        Vec3(2.55f, 3.65f, 0.25f),
        Vec3(0.43f, 0.43f, 0.43f));

    setPart(m_leftLeg,
        Vec3(-0.9f, 1.5f, 0.0f),
        Vec3(0.75f, 1.5f, 0.9f));

    setPart(m_rightLeg,
        Vec3(0.9f, 1.5f, 0.0f),
        Vec3(0.75f, 1.5f, 0.9f));
}

void Robot::draw() const
{
    m_shader.setVec3("color", Vec3(0.2f, 0.4f, 0.9f));
    m_torso.draw();

    m_shader.setVec3("color", Vec3(0.8f, 0.8f, 0.8f));
    m_head.draw();

    m_shader.setVec3("color", Vec3(0.1f, 0.9f, 1.0f));
    m_leftEye.draw();
    m_rightEye.draw();

    m_shader.setVec3("color", Vec3(0.005f, 0.005f, 0.01f));
    m_leftPupil.draw();
    m_rightPupil.draw();

    m_shader.setVec3("color", Vec3(0.25f, 0.27f, 0.32f));
    m_leftShoulder.draw();
    m_rightShoulder.draw();

    m_shader.setVec3("color", Vec3(0.8f, 0.16f, 0.18f));
    m_leftArm.draw();
    m_rightArm.draw();

    m_shader.setVec3("color", Vec3(0.45f, 0.48f, 0.53f));
    m_leftElbow.draw();
    m_rightElbow.draw();

    m_shader.setVec3("color", Vec3(0.2f, 0.2f, 0.25f));
    m_leftLeg.draw();
    m_rightLeg.draw();
}

}
