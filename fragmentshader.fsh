#version 330 core

in vec3 fragNormal;
in vec3 fragPosition;
out vec4 FragColor;
uniform vec3 color;
uniform vec3 lightDirection;
uniform float ambientStrength;
uniform bool shadowed;

void main() {
    float ambient = ambientStrength;
    float diff = max(dot(normalize(fragNormal), normalize(lightDirection)), 0.0);
    float light = ambient + diff;
    if(shadowed)
    {
        light = ambient;
    }
    FragColor = vec4(color * light, 1.0);
}
