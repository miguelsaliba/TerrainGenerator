#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);
    if (aPos.y < -2.0)
        vertColor = vec4(0.0, 0.0, 0.5, 1.0);
    else if (aPos.y < 2.0)
        vertColor = vec4(0.2, 0.1, 0.0, 1.0);
    else 
        vertColor = vec4(0.7, 0.7, 0.7, 1.0);
}
