#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec3 normal;
in vec3 FragPos;

uniform vec3 lightPos;

void main() {
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 result = vertColor * (vec3(.5, .5, .5) + diff);
    FragColor = vec4(result, 1.0);
}
