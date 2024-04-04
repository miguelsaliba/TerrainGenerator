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
    vertColor = colors[4];
    for (int i = 0; i < 5; i++)
    {
        if (aPos.y < heights[i])
        {
            vertColor = colors[i];
            break;
        }
    }

    if (aPos.y < heights[0])
        gl_Position = projection * view * vec4(aPos.x, heights[0], aPos.z, 1.0);
    else
        gl_Position = projection * view * vec4(aPos, 1.0);

    normal = aNormal;
    FragPos = aPos;
}
