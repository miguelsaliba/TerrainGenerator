#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec3 normal;
in vec3 FragPos;

uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float specularStrength;
uniform float ambientStrength;

void main() {
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 cameraDir = normalize(cameraPos - FragPos);

    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = vertColor * (specular + diffuse + ambient);

    FragColor = vec4(min(result, 1.0), 1.0);
}
