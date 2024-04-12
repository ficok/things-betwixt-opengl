#ifndef THINGSBETWIXT_CAMERA_H
#define THINGSBETWIXT_CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    float _zoom = 45.f;
    float _speed = 10.f;
    float _yaw;
    float _pitch;
    float _mouseSensitivity = .1f;

    glm::vec3 _position;
    glm::vec3 _worldUp = glm::vec3(.0f, 1.f, .0f);
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _front;

    Camera(float zoom = 45.f, float speed = 10.f, float yaw = .0f, float pitch = .0f, float mouseSensitivity = .1f,
           glm::vec3 position = glm::vec3(2.f, 2.f, 2.f))
    {
        _zoom = zoom;
        _speed = speed;
        _yaw = yaw;
        _pitch = pitch;
        _mouseSensitivity = mouseSensitivity;

        _position = position;

        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        _front.x = cos(glm::radians(_yaw) * cos(glm::radians(_pitch)));
        _front.y = sin(glm::radians(_pitch));
        _front.z = cos(glm::radians(_yaw) * sin(glm::radians(_pitch)));
        _front = glm::normalize(_front);

        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }

    glm::mat4 getViewMatrix()
    const {
        return glm::lookAt(_position, _position + _front, _up);
    }

    float fov()
    {
        return _zoom;
    }
};

#endif //THINGSBETWIXT_CAMERA_H
