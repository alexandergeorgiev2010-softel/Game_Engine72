#pragma once
#include "Engine/Renderer/Mesh.h"

namespace Engine {

class Primitives {
    public:
        static Mesh createCube();

        static Mesh createSphere(
            float radius = 1.0f,
            unsigned int sectors = 24,
            unsigned int stacks = 16
        );
};

}