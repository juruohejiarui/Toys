#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float disToModel;


// uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float breathVal;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // **环境光**
    vec3 ambient = 0.2 * lightColor;

    // **计算最终光照颜色**
    vec3 result = (ambient + diffuse) * vec3(1.0f, 0.0f, 0.0f);
    FragColor = vec4(result.rgb, 1.0f + breathVal);
}