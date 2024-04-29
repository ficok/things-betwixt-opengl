#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    vec3 result = vec3(1.) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1. / gamma));

    FragColor = vec4(result, 1.);
}