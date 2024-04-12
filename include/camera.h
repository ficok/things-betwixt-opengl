#ifndef THINGSBETWIXT_CAMERA_H
#define THINGSBETWIXT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    float _zoom = 45.f;
    float _speed = 10.f;
    float _yaw;
    float _pitch;
    float _mouseSensitivity = .1f;

    glm::vec3 _position;
    glm::vec3 _worldUp;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _front;

    Camera(glm::vec3 position = glm::vec3(2.f, 2.f, 2.f), glm::vec3 worldUp = glm::vec3(.0f, 1.f, .0f),
           float zoom = 45.f, float speed = 10.f, float yaw = .0f, float pitch = .0f, float mouseSensitivity = .1f)
    {
        _zoom = zoom;
        _speed = speed;
        _yaw = yaw;
        _pitch = pitch;
        _mouseSensitivity = mouseSensitivity;

        _position = position;
        _worldUp = worldUp;

        updateCameraVectors();
        std::cout << "INFO [camera]: initiailization complete:\n"
        << "- zoom: " << _zoom << std::endl
        << "- speed: " << _speed << std::endl
        << "- yaw: " << _yaw << std::endl
        << "- pitch: " << _pitch << std::endl
        << "- mouse sensitivity: " << _mouseSensitivity << std::endl
        << std::endl
        << "- position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")\n"
        << "- world up: (" << _worldUp.x << ", " << _worldUp.y << ", " << _worldUp.z << ")\n"
        << "- up: (" << _up.x << ", " << _up.y << ", " << _up.z << ")\n"
        << "- right: (" << _right.x << ", " << _right.y << ", " << _right.z << ")\n"
        << "- front: (" << _front.x << ", " << _front.y << ", " << _front.z << ")\n"
        << "----------\n\n";
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
