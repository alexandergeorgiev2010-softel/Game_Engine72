#pragma once
#include <Engine/Math/Mat4.h>
#include <glad/gl.h>

namespace Engine {

class Shader {
    public:
        Shader(const char* vertexPath, const char* fragmentPath);

        void bind() const;
        
        void setMat4(const char* name, const Mat4& matrix) const;
    
    private:
        unsigned int m_program = 0;
};

}
