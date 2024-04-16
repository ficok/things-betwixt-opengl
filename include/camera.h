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
    bool _invertXaxis;
    bool _invertYaxis;
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
        _mouseSensitivity = mouseSensitivity;
        _invertXaxis = false;
        _invertYaxis = false;
        _yaw = yaw;
        _pitch = pitch;
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
    void updateCameraView(double xoffset, double yoffset)
    {
        xoffset *= _mouseSensitivity;
        yoffset *= _mouseSensitivity;

        _yaw += (float)(_invertXaxis ? -xoffset : xoffset);
        _pitch += (float)(_invertYaxis ? -yoffset : yoffset);

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

    void updateMovementSpeed(float value)
    {
        if (_speed + value < 2.f || _speed + value > 40.f)
            return;

        _speed += value;
        std::cout << "INFO [camera]: movement speed: " << _speed << "\n";
    }

    void updateMouseSensitivity(float value)
    {
        if (_mouseSensitivity + value < .1f || _mouseSensitivity + value > .6f)
            return;

        _mouseSensitivity += value;
        std::cout << "INFO [camera]: mouse sensitivity: " << _mouseSensitivity << "\n";
    }

    void invertAxis(char which)
    {
        if (which == 'x') {
            std::cout << "INFO [camera]: X axis " << (_invertXaxis ? "not inverted\n" : "inverted\n");
            _invertXaxis = !_invertXaxis;
        }
        else {
            std::cout << "INFO [camera]: Y axis " << (_invertYaxis ? "not inverted\n" : "inverted\n");
            _invertYaxis = !_invertYaxis;
        }
    }

    glm::mat4 getViewMatrix()
    const {
        return glm::lookAt(_position, _position + _front, _up);
    }

    // get functions
    float fov()
    const {
        return _zoom;
    }

    glm::vec3 position()
    const {
        return _position;
    }

    glm::vec3 front()
    const {
        return _front;
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
