#version 330 core
// out variables
out vec4 FragColor;
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
    vec3 direction;

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
// function declarations
vec3 calculateDirectional(DirectionalLight directionalLight, vec3 normal, vec3 viewDirection);
vec3 calculatePoint(PointLight pointLight, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec3 calculateSpotlight(Spotlight spotlight, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
// in variables
in vec3 Normal;
in vec3 FragmentPosition;
// uniform variables
uniform vec3 viewPosition;
uniform vec3 color;
uniform float alpha;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform Spotlight spotlight;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    result += calculateDirectional(directionalLight, normal, viewDirection);
    result += calculatePoint(pointLight, normal, viewDirection, FragmentPosition);
    if (spotlight.on)
        result += calculateSpotlight(spotlight, normal, viewDirection, FragmentPosition);

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
    // calculating the reflection vector
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 32.0f);
    // calculating the light components
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diffuseFactor * color;
    vec3 specular = light.specular * specularFactor * color;

    return (ambient + diffuse + specular);
}

vec3 calculatePoint(PointLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // direction of the light from the perspective of the fragment
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    // calculating the diffuse factor
    float diffuseFactor = max(dot(normal, lightDirection), .0f);
    // calculating the specular factor
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectionDirection), .0f), 32.f);
    // calculating attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // calculating light components
    vec3 ambient = light.ambient * color * attenuation;
    vec3 diffuse = light.diffuse * diffuseFactor * color * attenuation;
    vec3 specular = light.specular * specularFactor * color * attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition)
{
    // direction of the light from the perspective of the fragment
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    // calculating the diffuse factor
    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
    // calculating the specular factor
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32.f);
    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff)/epsilon, 0.0f, 1.0f);

    // calculating the light components
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diffuseFactor * color;
    vec3 specular = light.specular * specularFactor * color;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}