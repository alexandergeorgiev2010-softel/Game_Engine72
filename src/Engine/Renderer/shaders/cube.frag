#version 410 core

uniform vec3 color;
out vec4 FragColor;

in vec3 vNormal;
in vec4 vPos;

uniform vec3 lightPosition;
vec3 lightDirection;



void main() {
    vec3 v_Pos = vec3(vPos);

    lightDirection = lightPosition - v_Pos;
    lightDirection = normalize(lightDirection);
    
    vec3 NormalizedNormal = normalize(vNormal);

    float diffuse = dot(NormalizedNormal, lightDirection);
    diffuse = max(diffuse, 0.0);

    vec3 LightenedColor = diffuse * color;

    FragColor = vec4(LightenedColor, 1.0);
}