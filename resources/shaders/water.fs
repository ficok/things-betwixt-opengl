#version 330 core

out vec4 FragColor;

in vec2 texCoords;

uniform vec3 waterColor;
uniform bool usingTexture;
uniform sampler2D tex;
uniform float alpha;

void main()
{
    if (usingTexture)
        FragColor = vec4(texture(tex, texCoords).rgb, alpha);
    else
        FragColor = vec4(waterColor, alpha);
}