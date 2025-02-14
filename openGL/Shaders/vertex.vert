#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 modelPos;


out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;
out float disToModel;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0f);
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;

    TexCoord = aTexCoord;
    disToModel = aPos.y;
    
    gl_Position = projection * view * worldPos;

}