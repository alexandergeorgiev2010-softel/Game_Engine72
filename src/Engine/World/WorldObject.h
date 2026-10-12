#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {
    class WorldObject {
        public:
            WorldObject(const Mesh& mesh, Shader& shader);

            void draw() const;

            Transform& getTransform() {
                return m_transform;
            }
            
        private:
            Transform m_transform;
            const Mesh& m_mesh;
            Shader& m_shader;
    };
}