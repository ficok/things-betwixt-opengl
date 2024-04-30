#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D blurred;
uniform float exposure;
uniform bool bloom;

void main()
{
    const float gamma = 2.2;

    vec3 hdrColor = texture(scene, TexCoords).rgb;
//     vec3 hdrColor = vec3(.0);
    vec3 blurredColor = texture(blurred, TexCoords).rgb;

        hdrColor += blurredColor;

    // tone mapping
    vec3 result = vec3(1.) - exp(-hdrColor * exposure);
    // gamma correction
    result += pow(result, vec3(1. / gamma));

    FragColor = vec4(result, 1.);
}