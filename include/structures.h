#ifndef THINGSBETWIXT_STRUCTURES_H
#define THINGSBETWIXT_STRUCTURES_H

#include <glm/glm.hpp>

struct Mouse
{
    double lastX;
    double lastY;
    bool firstMovement;
};

struct Frame
{
    double delta;
    double last;
    double current;
};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight
{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Spotlight
{
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;

    bool on;
};

#endif //THINGSBETWIXT_STRUCTURES_H
