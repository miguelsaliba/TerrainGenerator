#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = view * projection * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos, 1.0);
    vertColor = vec4(0.5, 0.0, 0.0, 1.0);
}
