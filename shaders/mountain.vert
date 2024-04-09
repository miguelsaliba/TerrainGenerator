#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vertColor;
out vec3 normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform float heights[5];
uniform vec3 colors[5];

void main()
{
    if (aPos.y < heights[0])
        vertColor = colors[0];
    else if (aPos.y < heights[1])
        vertColor = colors[1];
    else if (aPos.y < heights[2]) {
        float blend = (aPos.y - heights[1]) / (heights[2] - heights[1]);
        vertColor = mix(colors[2], colors[3], blend);
    } else if (aPos.y < heights[3]) {
        vertColor = colors[3];
    } else
        vertColor = colors[4];

    if (aPos.y < heights[0])
        gl_Position = projection * view * vec4(aPos.x, heights[0], aPos.z, 1.0);
    else
        gl_Position = projection * view * vec4(aPos, 1.0);

    normal = aNormal;
    FragPos = aPos;
}
