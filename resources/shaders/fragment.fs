#version 330 core
// out variables
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
// structures
struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;

    bool on;
};

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

#define MAX_NR_POINT_LIGHTS 10

// function declarations
vec3 calculateDirectional(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 calculatePoint(PointLight pointLight, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec3 calculateSpotlight(Spotlight spotlight, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
// in variables
in vec3 Normal;
in vec3 FragmentPosition;
in vec2 texCoords;
// uniform variables
uniform vec3 viewPosition;
uniform vec3 color;
uniform float alpha;
uniform bool blinn;
uniform int nrPointLights;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_NR_POINT_LIGHTS];
uniform Spotlight flashlight;
uniform Spotlight ErdtreeSpotlight;
uniform Material material;

void main()
{
    vec3 humanEyeSensitivity = vec3(.2126, .7152, .0722);
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 light = vec3(0.0f, 0.0f, 0.0f);
    // directional light
    light += calculateDirectional(directionalLight, normal, viewDirection);
    // lantern lights
    for (int i = 0; i < nrPointLights; ++i)
        light += calculatePoint(pointLight[i], normal, viewDirection, FragmentPosition);
    // flashlight
    if (flashlight.on)
        light += calculateSpotlight(flashlight, normal, viewDirection, FragmentPosition);
    // erdtree spotlight
    light += calculateSpotlight(ErdtreeSpotlight, normal, viewDirection, FragmentPosition);

    vec3 result = light;
    // determine whether to send to bright or regular color buffer
    float brightness = dot(result, humanEyeSensitivity);
    if (brightness > 1.)
        BrightColor = vec4(result, 1.);
    else
        BrightColor = vec4(.0, .0, .0, 1.);

    FragColor = vec4(result, alpha);
}
// function definitions
vec3 calculateDirectional(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    // need the vector that originates in the fragment that is also normalized
    vec3 lightDirection = normalize(-light.direction);
    // calculating the diffuse factor
    float diffuseFactor = max(dot(normal, lightDirection), 0.0f);
    // calculating the specular factor
    float specularFactor = 0.0f;
    if (blinn)
    {
        vec3 halfway = normalize(lightDirection + viewDirection);
        specularFactor = pow(max(dot(halfway, normal), .0f), material.shininess*4.0f);
    }
    else
    {
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    }
    // calculating the light components
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * specularFactor * vec3(texture(material.texture_specular1, texCoords).xxx);

    return (ambient + diffuse + specular);
}

vec3 calculatePoint(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // direction of the light from the perspective of the fragment
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    // calculating the diffuse factor
    float diffuseFactor = max(dot(normal, lightDirection), .0f);
    // calculating the specular factor
    float specularFactor = 0.0f;
    if (blinn)
    {
        vec3 halfway = normalize(lightDirection + viewDirection);
        specularFactor = pow(max(dot(halfway, normal), .0f), material.shininess * 4.0f);
    }
    else
    {
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    }
    // calculating attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // calculating light components
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords)) * attenuation;
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.texture_diffuse1, texCoords)) * attenuation;
    vec3 specular = light.specular * specularFactor * vec3(texture(material.texture_specular1, texCoords).xxx) * attenuation;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // direction of the light from the perspective of the fragment
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    // calculating the diffuse factor
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    // calculating the specular factor
    float specularFactor = 0.0f;
    if (blinn)
    {
        vec3 halfway = normalize(lightDirection + viewDirection);
        specularFactor = pow(max(dot(halfway, normal), .0f), material.shininess * 4.0f);
    }
    else
    {
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff)/epsilon, 0.0f, 1.0f);

    // calculating the light components
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 diffuse = light.diffuse * diffuseFactor * vec3(texture(material.texture_diffuse1, texCoords));
    vec3 specular = light.specular * specularFactor * vec3(texture(material.texture_specular1, texCoords).xxx);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}