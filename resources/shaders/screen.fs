#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screen;

void main()
{
    vec3 color = texture(screen, TexCoords).rgb;
    // invert colors
    FragColor = vec4(1. - color, 1.);
}