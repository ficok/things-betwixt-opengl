#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utils.h>
#include <shader.h>
#include <camera.h>

#define S_WIDTH  2000
#define S_HEIGHT 1500
#define TITLE "Things betwixt"

// structure definitions
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

// function declarations
// TODO: define
void processInput(GLFWwindow* window);

// callback functions
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// global variables
// TODO: fix this
Camera camera(glm::vec3(.0f, .0f, .0f));
Frame frame = {.0f, .0f, .0f};
Mouse mouse = {(double)S_WIDTH/2, (double)S_HEIGHT/2, true};

int main()
{
    // glfw init and config
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfw window creation
    // TODO: error handling
    GLFWwindow* window = glfwCreateWindow(S_WIDTH, S_HEIGHT, TITLE, nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // GLAD: loading all opengl function pointers
    // TODO: error handling
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // configuring callback functions
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // depth testing
    glEnable(GL_DEPTH_TEST);

    // creating a cube
    // creating VBO and VAO
    unsigned cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(utils::cubeVertices), &utils::cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // cube positions
    std::vector<glm::vec3> cubePositions =
    {
        glm::vec3(3.f, .0f, .0f),
        glm::vec3(-3.f, .0f, .0f),
        glm::vec3(.0f, 3.f, .0f),
        glm::vec3(.0f, -3.f, .0f),
        glm::vec3(.0f, .0f, 3.f),
        glm::vec3(.0f, .0f, -3.f)
    };
    // cube colors
    std::vector<glm::vec3> cubeColors =
    {
        glm::vec3(.0f, 1.f, .0f), // +x: green
        glm::vec3(.0f, .5f, .0f), // -x: dark green
        glm::vec3(1.f, .0f, .0f), // +y: red
        glm::vec3(.5f, .0f, .0f), // -y: dark red
        glm::vec3(.0f, .0f, 1.f), // +z: blue
        glm::vec3(.0f, .0f, .5f)  // -z: darkblue
    };

    // creating a shader program
    Shader cubeShader("resources/shaders/vertex.vs", "resources/shaders/fragment.fs", "cube");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculating time passed since the last frame
        frame.current = glfwGetTime();
        frame.delta = frame.current - frame.last;
        frame.last = frame.current;

        // processing input from previous frame
        processInput(window);

        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // everything that will be rendered goes below
        // activating the shader
        cubeShader.activate();

        // updating the view and projection matrices
        glm::mat4 view = glm::mat4(1.f);
        view = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.fov()), (float)S_WIDTH/(float)S_HEIGHT, .1f, 100.f);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        // drawing
        glm::mat4 model;
        for (int i = 0; i < 6; ++i)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            cubeShader.setVec3("color", cubeColors[i]);
            cubeShader.setMat4("model", model);
            // binding the vertex array and drawing
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clearing resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    cubeShader.del();
    // cleanly clearing glfw resources
    glfwTerminate();
}

// TODO
void processInput(GLFWwindow* window)
{
    // closing the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.updatePosition(FORWARD, frame.delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.updatePosition(LEFT, frame.delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.updatePosition(BACKWARD, frame.delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.updatePosition(RIGHT, frame.delta);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.updatePosition(UP, frame.delta);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.updatePosition(DOWN, frame.delta);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouse.firstMovement)
    {
        mouse.lastX = xpos;
        mouse.lastY = ypos;
        mouse.firstMovement = false;
    }

    float xoffset = xpos - mouse.lastX;
    float yoffset = mouse.lastY - ypos;

    mouse.lastX = xpos;
    mouse.lastY = ypos;

    camera.updateCameraView(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.updateFOV(yoffset);
}