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

// global variables
// TODO: fix this
Camera camera(glm::vec3(2.f, 2.f, 2.f));

// function declarations
// TODO: define
void processInput(GLFWwindow* window);

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

    // creating a shader program
    Shader cubeShader("resources/shaders/vertex.vs", "resources/shaders/fragment.fs", "cube");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // processing input from previous frame
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // everything that will be rendered goes below
        // activating the shader
        cubeShader.activate();

        glm::mat4 model = glm::mat4(1.f);
        glm::mat4 view = glm::mat4(1.f);
        view = glm::lookAt(glm::vec3(2.f, .0f, 2.f), glm::vec3(.0f, .0f, .0f), glm::vec3(.0f, 1.f, .0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)S_WIDTH/(float)S_HEIGHT, .1f, 100.f);

        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);

        // binding the vertex array and drawing
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
    std::cout << "";
}