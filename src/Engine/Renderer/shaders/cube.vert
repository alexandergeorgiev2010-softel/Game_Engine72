#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;
out vec4 vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 WorldPosition = model * vec4(aPos, 1.0);

    gl_Position = projection * view * WorldPosition;

    vNormal = transpose(inverse(mat3(model))) * aNormal;

    vPos = WorldPosition;
}