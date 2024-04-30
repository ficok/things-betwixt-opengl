#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 color;

void main()
{
    vec3 humanEyeSensitivity = vec3(.2126, .7152, .0722);
    FragColor = vec4(color, 1.0);
    float brightness = dot(FragColor.rgb, humanEyeSensitivity);
    if (brightness > 1.)
        BrightColor = vec4(FragColor.rgb, 1.);
    else
        BrightColor = vec4(.0, .0, .0, 1.);
}