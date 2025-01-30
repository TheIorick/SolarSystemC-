#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

out vec3 fragNormal;
out vec3 fragPosition; // Передаем позицию в мировых координатах

void main() {
    fragPosition = vec3(modelViewMatrix * vec4(aPos, 1.0)); // Мировые координаты
    fragNormal = mat3(transpose(inverse(modelViewMatrix))) * aNormal;
    gl_Position = projectionMatrix * vec4(fragPosition, 1.0);
}
