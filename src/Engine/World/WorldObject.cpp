#include "Engine/World/WorldObject.h"

namespace Engine {
    WorldObject::WorldObject(const Mesh& mesh, Shader& shader)
        : m_transform(),
          m_mesh(mesh),
          m_shader(shader)
    {
    }

    void WorldObject::draw() const {
        m_shader.setMat4("model", m_transform.getMatrix());
        m_mesh.draw();
    }
}