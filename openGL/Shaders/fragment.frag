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
uniform vec4 objColor;

uniform float discLyr;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // **环境光**
    vec3 ambient = 0.2 * lightColor;

    // **计算最终光照颜色**
    vec3 result = (ambient + diffuse) * objColor.xyz;
    FragColor = vec4(result.rgb, objColor.a / (1.0f + exp((disToModel - discLyr)) / 2));
}