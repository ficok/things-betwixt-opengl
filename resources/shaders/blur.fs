#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;
uniform float gaussianWeights[5] = float[] (.2270270270, .1945945946, .1216216216, .0540540541, .0162162162);

void main()
{
    vec2 texOffset = 1. / textureSize(image, 0);
    vec3 result = texture(image, TexCoords).rgb * gaussianWeights[0];

    if (horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(image, TexCoords + vec2(texOffset.x * i, .0)).rgb * gaussianWeights[i];
            result += texture(image, TexCoords - vec2(texOffset.x * i, .0)).rgb * gaussianWeights[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(image, TexCoords + vec2(.0, texOffset.y * i)).rgb * gaussianWeights[i];
            result += texture(image, TexCoords - vec2(.0, texOffset.y * i)).rgb * gaussianWeights[i];
        }
    }

    FragColor = vec4(result, 1.);
}