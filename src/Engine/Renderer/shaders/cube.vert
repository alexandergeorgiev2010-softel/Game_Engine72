#version 410 core

layout (location = 0) in vec3 aPos;

void main() {
    float angle = 0.7;
    float c = cos(angle);
    float s = sin(angle);

    vec3 rotated;

    rotated.x = c * aPos.x + s * aPos.z;
    rotated.y = aPos.y;
    rotated.z = -s * aPos.x + c * aPos.z;

    float z = rotated.z + 4.0;

    gl_Position = vec4(
        rotated.x / z,
        rotated.y / z,
        rotated.z / z,
        1.0
    );


}