#version 330 core
// in variables
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// out variables
out vec3 Normal;
out vec3 FragmentPosition;

// uniform variables
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = aNormal;
    FragmentPosition = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * vec4(FragmentPosition, 1.0f);
}