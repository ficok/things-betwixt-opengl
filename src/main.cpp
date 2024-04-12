#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define S_WIDTH  2000
#define S_HEIGHT 1500
#define TITLE "Things betwixt"

// function declarations
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

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // processing input from previous frame
        processInput(window);

        // everything that will be rendered goes below

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanly clearing glfw resources
    glfwTerminate();
}