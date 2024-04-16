#ifndef THINGSBETWIXT_CAMERA_H
#define THINGSBETWIXT_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    // control values
    float _zoom;
    float _speed;
    float _mouseSensitivity;
    // euler angles
    float _yaw;
    float _pitch;
    // position vectors
    glm::vec3 _position;
    glm::vec3 _worldUp;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _front;
    // constructor
    Camera(glm::vec3 position = glm::vec3(.0f), glm::vec3 worldUp = glm::vec3(.0f, 1.f, .0f),
           float zoom = 45.f, float speed = 10.f, float yaw = -90.f, float pitch = .0f, float mouseSensitivity = .1f)
    {
        _zoom = zoom;
        _speed = speed;
        _yaw = yaw;
        _pitch = pitch;
        _mouseSensitivity = mouseSensitivity;
        _front = glm::vec3(.0f, .0f, -1.f);

        _position = position;
        _worldUp = worldUp;
        _up = worldUp;

        updateCameraVectors();
        printCameraInfo();
    }
    // TODO: comments
    void updateCameraVectors()
    {
        _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front.y = sin(glm::radians(_pitch));
        _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(_front);
        _right = glm::normalize(glm::cross(_front, _worldUp));
        _up = glm::normalize(glm::cross(_right, _front));
    }
    // TODO: comments
    void updatePosition(Movement direction, float deltaTime)
    {
        float movementSpeed = _speed * deltaTime;
        switch(direction)
        {
            case FORWARD:
                _position += _front * movementSpeed; break;
            case BACKWARD:
                _position -= _front * movementSpeed; break;
            case LEFT:
                _position -= _right * movementSpeed; break;
            case RIGHT:
                _position += _right * movementSpeed; break;
            case UP:
                _position += _worldUp * movementSpeed; break;
            case DOWN:
                _position -= _worldUp * movementSpeed; break;
        }
    }
    // TODO: comments
    void updateCameraView(double xoffset, double yoffset, bool invertAxis = false)
    {
        xoffset *= _mouseSensitivity;
        yoffset *= _mouseSensitivity;

        if (invertAxis)
        {
            _yaw -= xoffset;
            _pitch -= yoffset;
        }
        else
        {
            _yaw += xoffset;
            _pitch += yoffset;
        }

        if (_pitch > 89.f) _pitch = 89.f;
        if (_pitch < -89.f) _pitch = -89.f;

        updateCameraVectors();
    }
    // TODO: comments
    void updateFOV(double yoffset)
    {
        _zoom -= (float)yoffset;

        if (_zoom < 1.f) _zoom = 1.f;
        if (_zoom > 45.f) _zoom = 45.f;
    }

    glm::mat4 getViewMatrix()
    const {
        return glm::lookAt(_position, _position + _front, _up);
    }

    float fov()
    const {
        return _zoom;
    }

    void printCameraInfo()
    {

        std::cout << "INFO [camera]: initiailization complete:\n"
                  << "- zoom: " << _zoom << "\n"
                  << "- speed: " << _speed << "\n"
                  << "- yaw: " << _yaw << "\n"
                  << "- pitch: " << _pitch << "\n"
                  << "- mouse sensitivity: " << _mouseSensitivity << "\n\n"
                  << "- position: (" << _position.x << ", " << _position.y << ", " << _position.z << ")\n"
                  << "- world up: (" << _worldUp.x << ", " << _worldUp.y << ", " << _worldUp.z << ")\n"
                  << "- up: (" << _up.x << ", " << _up.y << ", " << _up.z << ")\n"
                  << "- right: (" << _right.x << ", " << _right.y << ", " << _right.z << ")\n"
                  << "- front: (" << _front.x << ", " << _front.y << ", " << _front.z << ")\n"
                  << "----------\n\n";
    }
};

#endif //THINGSBETWIXT_CAMERA_H
