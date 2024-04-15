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
// function declarations
vec3 calculateDirectional(DirectionalLight directionalLight, vec3 normal, vec3 fragmentPosition);
// in variables
in vec3 Normal;
in vec3 FragmentPosition;
// uniform variables
uniform vec3 viewPosition;
uniform vec3 color;
uniform DirectionalLight directionalLight;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    result += calculateDirectional(directionalLight, normal, FragmentPosition);

    FragColor = vec4(result, 1.0f);
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
    // if it's only color, or the right combination of .xxx, .yyy, or .zzz and the direction of
    // light, the specular component behaves weirdly
    vec3 specular = light.specular * specularFactor * color.yyy;

    return ambient + diffuse + specular;
}