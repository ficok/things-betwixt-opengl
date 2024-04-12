#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "utils.h"

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
    // creating the vertex shader
    std::string vertexPath = "resources/shaders/vertex.vs";
    std::string vertexShaderString = utils::readFile(vertexPath);
    const char* vertexShaderSource = vertexShaderString.c_str();
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // checking for errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::" << vertexPath << "::COMPILATION FAILED\n"
            << infoLog << std::endl;
    }
    // creating the fragment shader
    std::string fragmentPath = "resources/shaders/fragment.fs";
    std::string fragmentShaderString = utils::readFile(fragmentPath);
    const char* fragmentShaderSource = fragmentShaderString.c_str();
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::" << fragmentPath << "::COMPILATION FAILED\n"
            << infoLog << std::endl;
    }
    // linking into shader program
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // processing input from previous frame
        processInput(window);

        // everything that will be rendered goes below
        // activating the shader
        glUseProgram(shaderProgram);
        // binding the vertex array
        glBindVertexArray(squareVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clearing resources
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
    glDeleteProgram(shaderProgram);
    // cleanly clearing glfw resources
    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    std::cout << "..";
}