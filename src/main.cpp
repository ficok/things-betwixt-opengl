#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include <utils.h>
#include <shader.h>
#include <camera.h>
#include <structures.h>
#include <errors.h>
#include <framebuffer.h>

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
float exposure = .05f;

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

    // enabling gl tests
    glEnable(GL_DEPTH_TEST);

    // configuring tests
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);

    // creating a cube
    // configuring VBO and VAO
    unsigned cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data::cubeVerticesWNormalsTextures),
                 &data::cubeVerticesWNormalsTextures, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // normal attribute
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
        glm::vec3(4.f, .0f, .0f),
        glm::vec3(-4.f, .0f, .0f),
        glm::vec3(.0f, 4.f, .0f),
        glm::vec3(.0f, -4.f, .0f),
        glm::vec3(.0f, .0f, 4.f),
        glm::vec3(.0f, .0f, -4.f),
        // light cube position
        glm::vec3(6.f)
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
        // light cube color
        glm::vec3(100.f)
    };

    std::vector<glm::vec3> transparentCubePositions =
    {
        glm::vec3(.0f),
        glm::vec3(2.f, 0.f, 2.0f),
        glm::vec3(-2.f, 0.f, 2.0f),
        glm::vec3(2.f, 0.f, -2.0f),
        glm::vec3(-2.f, 0.f, -2.0f),
        glm::vec3(2.f, 2.f, .0f),
        glm::vec3(-2.f, 2.f, .0f),
        glm::vec3(2.f, -2.f, .0f),
        glm::vec3(-2.f, -2.f, .0f)
    };

    glm::vec3 transparentCubeColor = glm::vec3(.9f, .9f, 1.f);

    // creating a rectangle that represents the screen (for the framebuffer texture)
    unsigned int rectangleVAO, rectangleVBO;
    glGenVertexArrays(1, &rectangleVAO);
    glGenBuffers(1, &rectangleVBO);
    glBindVertexArray(rectangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data::rectangleVertices), &data::rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // creating a shader program
    Shader cubeShader("vertex.vs", "fragment.fs", "cube");
    Shader lightCubeShader("lightCube.vs", "lightCube.fs", "lightCube");
    Shader blurShader("blur.vs", "blur.fs", "blur");
    Shader bloomShader("bloom.vs", "bloom.fs", "bloom");

    // creating a custom framebuffer
    Framebuffer hdrFB(RGBA, 2, true, false);
    Framebuffer pingpongFB1(RGBA, 1, false, false);
    Framebuffer pingpongFB2(RGBA, 1, false, false);

    // initializing the light
    DirectionalLight directionalLight =
    {
        glm::vec3(.0f, -1.f, .0f),
        glm::vec3(.05f),
        glm::vec3(.2f),
        glm::vec3(1.f)
    };

    PointLight pointLight =
    {
        cubePositions.back(),
        glm::vec3(.0f),

        glm::vec3(.1f),
        glm::vec3(1.f),
        glm::vec3(1.f),

        1.f, .09f, .032f
    };

    Spotlight spotlight =
    {
        camera.position(),
        camera.front(),

        glm::vec3(.0f),
        glm::vec3(1.f),
        glm::vec3(1.f),

        1.f, .09f, .032f,
        glm::cos(glm::radians(5.f)),
        glm::cos(glm::radians(20.f)),
        toggle::flashlightOn
    };

    // sending render independent info to shaders
    blurShader.activate();
    blurShader.setInt("image", 0);

    bloomShader.activate();
    bloomShader.setInt("scene", 0);
    bloomShader.setInt("blurred", 1);

    // print some default values
    utils::printInfo(toggle::blend, toggle::cull, toggle::blinn, toggle::flashlightOn);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculating time passed since the last frame
        frame.current = glfwGetTime();
        frame.delta = frame.current - frame.last;
        frame.last = frame.current;

        // toggling tests
        toggle::cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        toggle::blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        // processing input from previous frame
        processInput(window);

        // drawing into the hdr framebuffer
        hdrFB.activate();

        // clearing buffers for the current frame
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // everything that will be rendered goes below
        // activating the shader
        cubeShader.activate();
        // sending camera information to shader
        cubeShader.setVec3("viewPosition", camera._position);
        // sending light information to shader
        cubeShader.setDirectionalLight("directionalLight", directionalLight);
        cubeShader.setPointLight("pointLight", pointLight);
        spotlight.position = camera.position();
        spotlight.direction = camera.front();
        spotlight.on = toggle::flashlightOn;
        cubeShader.setSpotlight("spotlight", spotlight);

        // indicate if we're using blinn-phong or phong
        cubeShader.setBool("blinn", toggle::blinn);

        // sort transparent cube positions
        std::sort(transparentCubePositions.begin(), transparentCubePositions.end(),
            [cameraPosition = camera.position()](const glm::vec3 a, const glm::vec3 b)
            {
                float distanceA = glm::distance(a, cameraPosition);
                float distanceB = glm::distance(b, cameraPosition);
                return distanceA > distanceB;
            }
        );

        // updating the view and projection matrices
        glm::mat4 view = glm::mat4(1.f);
        view = camera.getViewMatrix();
        glm::mat4 projection =
                glm::perspective(glm::radians(camera.fov()), (float)S_WIDTH/(float)S_HEIGHT, .1f, 100.f);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        // drawing cubes
        glm::mat4 model;
        glBindVertexArray(cubeVAO);
        for (int i = 0; i < (int)cubePositions.size()-1; ++i)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, cubePositions[i]);
            cubeShader.setVec3("color", cubeColors[i]);
            cubeShader.setFloat("alpha", 1.f);
            cubeShader.setMat4("model", model);
            // binding the vertex array and drawing
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // drawing the light cube
        lightCubeShader.activate();
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setVec3("color", cubeColors.back());
        model = glm::mat4(1.f);
        model = glm::translate(model, cubePositions.back());
        model = glm::scale(model, glm::vec3(.35f));
        lightCubeShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // drawing transparent objects last
        cubeShader.activate();
        for (int i = 0; i < (int)transparentCubePositions.size(); ++i)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, transparentCubePositions[i]);
            model = glm::scale(model, glm::vec3(.5f));
            cubeShader.setVec3("color", transparentCubeColor);
            cubeShader.setFloat("alpha", .5f);
            cubeShader.setMat4("model", model);

            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // turn off hdr framebuffer
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        hdrFB.deactivate();

        // blur bright fragments with a two-pass gaussian blur
        blurShader.activate();
        bool horizontal = true;
        bool firstIteration = true;
        unsigned nrPasses = 10; // double the nr of weights in blur shader
        for (unsigned i = 0; i < nrPasses; ++i)
        {
            horizontal ? pingpongFB1.activate() : pingpongFB2.activate();

            glActiveTexture(GL_TEXTURE0);
            blurShader.setBool("horizontal", horizontal);

            glBindTexture(GL_TEXTURE_2D,
                          firstIteration ? hdrFB.colorBuffers[1] :
                                                 (horizontal ? pingpongFB2.colorBuffers[0] : pingpongFB1.colorBuffers[0]));

            glBindVertexArray(rectangleVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            horizontal = !horizontal;
            if (firstIteration)
                firstIteration = false;

            horizontal ? pingpongFB1.deactivate() : pingpongFB2.deactivate();
        }

        // render floating point color buffer to a 2D rectangle
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bloomShader.activate();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, hdrFB.colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, horizontal ? pingpongFB2.colorBuffers[0] : pingpongFB1.colorBuffers[0]);
        bloomShader.setBool("bloom", toggle::bloom);
        bloomShader.setFloat("exposure", exposure);
        glBindVertexArray(rectangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clearing resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    cubeShader.del();
    lightCubeShader.del();
    blurShader.del();
    bloomShader.del();
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
        toggle::flashlightOn = !toggle::flashlightOn;
        std::cout << "INFO: flashlight turned " << (toggle::flashlightOn ? "on\n" : "off\n");
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
        toggle::cull = !toggle::cull;
        std::cout << "INFO: " << (toggle::cull ? "culling faces.\n" : "not culling faces.\n");
    }

    // toggle transparency (blending)
    if (key == GLFW_KEY_B && action == GLFW_PRESS)
    {
        toggle::blend = !toggle::blend;
        std::cout << "INFO: transparency " << (toggle::blend ? "on.\n" : "off.\n");
    }

    // toggle blinn-phong and phong
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        toggle::blinn = !toggle::blinn;
        std::cout << "INFO: using " << (toggle::blinn ? "blinn-phong's model.\n" : "phong's model.\n");
    }

    // toggle custom framebuffer and post-processing
    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
        toggle::postprocessing = !toggle::postprocessing;
        std::cout << "INFO: post-processing " << (toggle::postprocessing ? "activated.\n" : "deactivated.\n");
    }

    // exposure
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (exposure - .005f < .005f)
            exposure = .005f;
        else
            exposure -= .005f;
        std::cout << "INFO: exposure: " << exposure << "\n";
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if (exposure + .005f > 1.f)
            exposure = 1.f;
        else
            exposure += .005f;
        std::cout << "INFO: exposure: " << exposure << "\n";
    }
}