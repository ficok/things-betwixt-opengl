#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <utils.h>
#include <shader.h>

#define S_WIDTH  2000
#define S_HEIGHT 1500
#define TITLE "Things betwixt"

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

    // creating a square
    float vertices[]
    {
        -.5f, -.5f, .0f,
        -.5f,  .5f, .0f,
        .5f,  .5f, .0f,

        -.5f, -.5f, .0f,
        .5f, -.5f, .0f,
        .5f, .5f, .5f
    };
    // creating VBO and VAO
    unsigned squareVAO, squareVBO;
    glGenVertexArrays(1, &squareVAO);
    glBindVertexArray(squareVAO);
    glGenBuffers(1, &squareVBO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // creating a shader program
    Shader squareShader("resources/shaders/vertex.vs", "resources/shaders/fragment.fs", "square");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // processing input from previous frame
        processInput(window);

        // everything that will be rendered goes below
        // activating the shader
        squareShader.activate();
        // glUseProgram(shaderProgram);
        // binding the vertex array
        glBindVertexArray(squareVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clearing resources
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
    squareShader.del();
    // glDeleteProgram(shaderProgram);
    // cleanly clearing glfw resources
    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    std::cout << "";
}