#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexCol;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexCol = vec4(0.5f / aPos.x, 0.1f / aPos.y, 0.2f / (aPos.x * aPos.y), 1.0);
}