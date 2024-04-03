#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vertColor;
out vec3 normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    if (aPos.y < -2.0) {
        vertColor = vec3(0.3, 0.4, 1.0);
    }
    else if (aPos.y < 2.0) {
        vertColor = vec3(0.2, 0.25, 0.3);
    }
    else {
        vertColor = vec3(0.8, 0.85, 0.9);
    }

    if (aPos.y < -2.0)
        gl_Position = projection * view * vec4(aPos.x, -2.0, aPos.z, 1.0);
    else
        gl_Position = projection * view * vec4(aPos, 1.0);

    normal = aNormal;
    FragPos = aPos;
}
