#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utils.h>
#include <shader.h>
#include <camera.h>
#include <structures.h>
#include <errors.h>

#define S_WIDTH  2000
#define S_HEIGHT 1500
#define TITLE "Things betwixt"

// function declarations
void processInput(GLFWwindow* window);

// callback functions
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// global variables
Camera camera(glm::vec3(.0f, .0f, .0f));
Frame frame = {.0f, .0f, .0f};
Mouse mouse = {(double)S_WIDTH/2, (double)S_HEIGHT/2, true};
bool flashlightOn = false;

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
    assert(window, "ERROR [main]: window creation failed.");
    glfwMakeContextCurrent(window);

    // GLAD: loading all opengl function pointers
    // TODO: error handling
    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "ERROR [main]: failed to initialize GLAD.");

    // configuring callback functions
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    // capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // depth testing
    glEnable(GL_DEPTH_TEST);

    // creating a cube
    // configuring VBO and VAO
    unsigned cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(utils::cubeVerticesWNormalsTextures),
                 &utils::cubeVerticesWNormalsTextures, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // normal attribte
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    // unbinding buffers
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
        glm::vec3(.0f, .0f, -3.f),

        glm::vec3(6.f, .0f, .0f)
    };
    // cube colors
    std::vector<glm::vec3> cubeColors =
    {
        glm::vec3(.0f, 1.f, .0f), // +x: green
        glm::vec3(.0f, .5f, .0f), // -x: dark green
        glm::vec3(1.f, .0f, .0f), // +y: red
        glm::vec3(.5f, .0f, .0f), // -y: dark red
        glm::vec3(.0f, .0f, 1.f), // +z: blue
        glm::vec3(.0f, .0f, .5f), // -z: darkblue

        glm::vec3(1.f)             // light cube
    };

    // creating a shader program
    Shader cubeShader("vertex.vs", "fragment.fs", "cube");

    // initializing the light
    DirectionalLight directionalLight =
    {
        glm::vec3(.0f, -1.f, .0f),
        glm::vec3(.05f),
        glm::vec3(.2f),
        glm::vec3(.2f)
    };

    PointLight pointLight =
    {
        glm::vec3(4.f, .0f, .0f),
        glm::vec3(.0f),

        glm::vec3(.1f),
        glm::vec3(1.f),
        glm::vec3(1.f),

        1.f, .09f, .032f
    };

    Spotlight spotlight =
    {
        camera._position,
        camera._front,

        glm::vec3(.0f),
        glm::vec3(1.f),
        glm::vec3(1.f),

        1.f, .09f, .032f,
        glm::cos(glm::radians(5.f)),
        glm::cos(glm::radians(20.f)),
        flashlightOn
    };

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculating time passed since the last frame
        frame.current = glfwGetTime();
        frame.delta = frame.current - frame.last;
        frame.last = frame.current;

        // processing input from previous frame
        processInput(window);
        // clearing buffers for the current frame
        glClearColor(.1f, .1f, .1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // everything that will be rendered goes below
        // activating the shader
        cubeShader.activate();
        // sending camera information to shader
        cubeShader.setVec3("viewPosition", camera._position);
        // TODO: create functions in shader that do this by passing the reference to the struct instance
        // sending light information to shader
        cubeShader.setDirectionalLight("directionalLight", directionalLight);
        cubeShader.setPointLight("pointLight", pointLight);
        spotlight.position = camera._position;
        spotlight.direction = camera._front;
        spotlight.on = flashlightOn;
        cubeShader.setSpotlight("spotlight", spotlight);

        // updating the view and projection matrices
        glm::mat4 view = glm::mat4(1.f);
        view = camera.getViewMatrix();
        glm::mat4 projection =
                glm::perspective(glm::radians(camera.fov()), (float)S_WIDTH/(float)S_HEIGHT, .1f, 100.f);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        // drawing
        glm::mat4 model;
        auto n = (int)cubePositions.size();
        for (int i = 0; i < n; ++i)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            if (i == n-1)
                model = glm::scale(model, glm::vec3(.3f));
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

void processInput(GLFWwindow* window)
{
    // closing the program
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.updatePosition(FORWARD, (float)frame.delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.updatePosition(LEFT, (float)frame.delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.updatePosition(BACKWARD, (float)frame.delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.updatePosition(RIGHT, (float)frame.delta);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.updatePosition(UP, (float)frame.delta);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.updatePosition(DOWN, (float)frame.delta);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}

// TODO: comments
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
    // toggle flashlight
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        flashlightOn = !flashlightOn;
        std::cout << "INFO: flashlight turned " << (flashlightOn ? "on\n" : "off\n");
    }
}