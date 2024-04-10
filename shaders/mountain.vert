#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vertColor;
out vec3 normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

uniform bool texture_enabled;
uniform float heights[5];
uniform vec3 colors[5];

float getPercentage(float up, float down, float percent) {
    return down + (up - down) * percent;
}

void main()
{
    float interp1down = getPercentage(heights[1], heights[0], 0.75);
    float interp1up   = getPercentage(heights[2], heights[1], 0.25);
    float interp2down = getPercentage(heights[2], heights[1], 0.75);
    float interp2up   = getPercentage(heights[3], heights[2], 0.25);
    float interp3down = getPercentage(heights[3], heights[2], 0.75);
    float interp3up   = getPercentage(heights[4], heights[3], 0.25);

    if (aPos.y < heights[0]) {
        vertColor = colors[0];
    }
    else if (aPos.y < interp1down) {
        vertColor = colors[1];
    }
    else if (aPos.y < interp1up) {
        float blend = (aPos.y - interp1down) / (interp1up - interp1down);
        vertColor = mix(colors[1], colors[2], blend);
    }
    else if (aPos.y < interp2down) {
        vertColor = colors[2];
    }
    else if (aPos.y < interp2up) {
        float blend = (aPos.y - interp2down) / (interp2up - interp2down);
        vertColor = mix(colors[2], colors[3], blend);
    }
    else if (aPos.y < interp3down) {
        vertColor = colors[3];
    }
    else if (aPos.y < interp3up) {
        float blend = (aPos.y - interp3down) / (interp3up - interp3down);
        vertColor = mix(colors[3], colors[4], blend);
    }
    else {
        vertColor = colors[4];
    }

    if (aPos.y < heights[0])
        gl_Position = projection * view * vec4(aPos.x, heights[0], aPos.z, 1.0);
    else
        gl_Position = projection * view * vec4(aPos, 1.0);

    normal = aNormal;
    FragPos = aPos;
    TexCoords = aTexCoords;
}
