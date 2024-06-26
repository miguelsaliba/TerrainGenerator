#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec3 normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float specularStrength;
uniform float ambientStrength;

uniform bool texture_enabled;
uniform float heights[5];
uniform vec3 colors[5];
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;

vec4 calculateColor(vec3 color) {
    vec3 norm = normalize(normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 cameraDir = normalize(cameraPos - FragPos);

    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = ambientStrength * lightColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = color * (specular + diffuse + ambient);
    return vec4(min(result, 1.0), 1.0);
}

float getPercentage(float up, float down, float percent) {
    return down + (up - down) * percent;
}

// TODO: move the texture calls outside the if statements
vec3 calculateTextureColor(vec3 color) {
    float interp1down = getPercentage(heights[1], heights[0], 0.75);
    float interp1up   = getPercentage(heights[2], heights[1], 0.25);
    float interp2down = getPercentage(heights[2], heights[1], 0.75);
    float interp2up   = getPercentage(heights[3], heights[2], 0.25);
    float interp3down = getPercentage(heights[3], heights[2], 0.75);
    float interp3up   = getPercentage(heights[4], heights[3], 0.25);
    vec3 fragColor1 = texture(texture1, TexCoords).rgb;
    vec3 fragColor2 = texture(texture2, TexCoords).rgb;
    vec3 fragColor3 = texture(texture3, TexCoords).rgb;
    vec3 fragColor4 = texture(texture4, TexCoords).rgb;
    vec3 fragColor5 = texture(texture5, TexCoords).rgb;
    if (FragPos.y < heights[0]) {
        color = colors[0];
    }
    else if (FragPos.y < interp1down) {
        color = texture(texture2, TexCoords).rgb;
    }
    else if (FragPos.y < interp1up) {
        float blend = (FragPos.y - interp1down) / (interp1up - interp1down);
        color = mix(fragColor2, fragColor3, blend);
    }
    else if (FragPos.y < interp2down) {
        color = fragColor3;
    }
    else if (FragPos.y < interp2up) {
        float blend = (FragPos.y - interp2down) / (interp2up - interp2down);
        color = mix(fragColor3, fragColor4, blend);
    }
    else if (FragPos.y < interp3down) {
        color = fragColor4;
    }
    else if (FragPos.y < interp3up) {
        float blend = (FragPos.y - interp3down) / (interp3up - interp3down);
        color = mix(fragColor4, fragColor5, blend);
    }
    else {
        color = fragColor5;
    }
    return color;
}

void main() {
    vec3 color = vertColor;
    if (texture_enabled) {
        color = calculateTextureColor(vertColor);
    }
    FragColor = calculateColor(color);
}
