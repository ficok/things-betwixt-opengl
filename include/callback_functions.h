#ifndef THINGSBETWIXT_CALLBACK_FUNCTIONS_H
#define THINGSBETWIXT_CALLBACK_FUNCTIONS_H

#include <iostream>
#include <GLFW/glfw3.h>

#include <camera.h>
#include <structures.h>
#include <utils.h>

extern Camera camera;
extern Mouse mouse;

namespace callback
{
    // declarations
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    // definitions
    void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        UNUSED(window);
        glViewport(0, 0, width, height);
    }

    void mouseCallback(GLFWwindow* window, double xpos, double ypos)
    {
        UNUSED(window);
        if (mouse.firstMovement)
        {
            mouse.lastX = xpos;
            mouse.lastY = ypos;
            mouse.firstMovement = false;
        }

        auto xoffset = (float)(xpos - mouse.lastX);
        auto yoffset = (float)(mouse.lastY - ypos);

        mouse.lastX = xpos;
        mouse.lastY = ypos;

        camera.updateCameraView(xoffset, yoffset);
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        UNUSED(window); UNUSED(xoffset);
        camera.updateFOV(yoffset);
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        UNUSED(window); UNUSED(scancode); UNUSED(mods);

        // exit program
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // toggle flashlight
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            settings::flashlightOn = !settings::flashlightOn;
            std::cout << "INFO: flashlight turned " << (settings::flashlightOn ? "on\n" : "off\n");
        }

        // movement speed
        if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
            camera.updateMovementSpeed(2.f);
        if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
            camera.updateMovementSpeed(-2.f);

        // mouse sensitivity
        if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS)
            camera.updateMouseSensitivity(-.1f);
        if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
            camera.updateMouseSensitivity(.1f);
        }

        // invert axis
        if (key == GLFW_KEY_X && action == GLFW_PRESS)
            camera.invertAxis('x');
        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
            camera.invertAxis('y');

        // toggle face culling
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            settings::cull = !settings::cull;
            std::cout << "INFO: " << (settings::cull ? "culling faces.\n" : "not culling faces.\n");
        }

        // toggle transparency (blending)
        if (key == GLFW_KEY_B && action == GLFW_PRESS)
        {
            settings::blend = !settings::blend;
            std::cout << "INFO: transparency " << (settings::blend ? "on.\n" : "off.\n");
        }

        // toggle blinn-phong and phong
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            settings::blinn = !settings::blinn;
            std::cout << "INFO: using " << (settings::blinn ? "blinn-phong's model.\n" : "phong's model.\n");
        }

        // toggle custom framebuffer and post-processing
        if (key == GLFW_KEY_I && action == GLFW_PRESS)
        {
            settings::bloom = !settings::bloom;
            std::cout << "INFO: bloom is " << (settings::bloom ? "activated.\n" : "deactivated.\n");
        }

        // exposure
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            if (settings::exposure - .005f < .005f)
                settings::exposure = .005f;
            else
                settings::exposure -= .005f;
            std::cout << "INFO: exposure: " << settings::exposure << "\n";
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            if (settings::exposure + .005f > 1.f)
                settings::exposure = 1.f;
            else
                settings::exposure += .005f;
            std::cout << "INFO: exposure: " << settings::exposure << "\n";
        }
    }
}

#endif //THINGSBETWIXT_CALLBACK_FUNCTIONS_H
