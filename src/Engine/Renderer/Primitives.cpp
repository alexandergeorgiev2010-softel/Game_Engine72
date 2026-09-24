#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Primitives.h"


namespace Engine {

Mesh Primitives::createCube() {
    const float vertices[] = {
        //Front face
        //0 = bottom-left-front
        -1.0f, -1.0f, 1.0f,

        //1 = bottom-right-front
        1.0f, -1.0f, 1.0f,

        //2 = top-right-front
        1.0f, 1.0f, 1.0f,

        //3 = top-left-front
        -1.0f, 1.0f, 1.0f,

        //Back face
        //4 = bottom-left-back
        -1.0f, -1.0f, -1.0f,

        //5 = bottom-right-back
        1.0f, -1.0f, -1.0f,

        //6 = top-right-back
        1.0f, 1.0f, -1.0f,

        //7 = top-left-back
        -1.0f, 1.0f, -1.0f
    };


    const unsigned int indices[] = {
        //front face
        0, 1, 2,
        0, 2, 3,

        //back face
        4, 6, 5,
        4, 7, 6,

        //right face
        1, 5, 6,
        1, 6, 2,

        //left face
        0, 7, 4,
        0, 3, 7,

        //bottom face
        0, 5, 1,
        0, 4, 5,

        //upper face
        2, 6, 3,
        3, 6, 7 
    };

    return Mesh(vertices, 8, indices, 36);
}

}