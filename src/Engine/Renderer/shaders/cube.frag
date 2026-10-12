#version 410 core

uniform vec3 color;
uniform vec3 lightPosition;

in vec3 vNormal;
in vec4 vPos;

out vec4 FragColor;

void main() {
    vec3 WorldPosition = vec3(vPos);

    vec3 lightDirection = normalize(lightPosition - WorldPosition);
    vec3 normal = normalize(vNormal);

    float diffuse = max(dot(normal, lightDirection), 0);

    float ambient = 0.25;

    vec3 lighting = vec3(ambient) + vec3(0.75 * diffuse);
    vec3 finalColor = color * lighting;

    FragColor = vec4(finalColor, 1.0);
}