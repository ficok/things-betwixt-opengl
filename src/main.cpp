#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <callback_functions.h>
#include <utils.h>
#include <shader.h>
#include <camera.h>
#include <structures.h>
#include <errors.h>
#include <framebuffer.h>
#include <skybox.h>
#include <model.h>
#include <simple_shapes.h>

// function declarations
void processMovement(GLFWwindow* window);

// global variables
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
    GLFWwindow* window = glfwCreateWindow(S_WIDTH, S_HEIGHT, TITLE, nullptr, nullptr);
    assert(window, "ERROR [main]: window creation failed.");
    glfwMakeContextCurrent(window);

    // GLAD: loading all opengl function pointers
    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "ERROR [main]: failed to initialize GLAD.");

    // configuring callback functions
    glfwSetFramebufferSizeCallback(window, callback::framebufferSizeCallback);
    glfwSetCursorPosCallback(window, callback::mouseCallback);
    glfwSetScrollCallback(window, callback::scrollCallback);
    glfwSetKeyCallback(window, callback::keyCallback);

    // capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // enabling gl tests
    glEnable(GL_DEPTH_TEST);

    // configuring tests
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);



    // cube positions
    std::vector<glm::vec3> lanternPositions =
    {
        glm::vec3(12.635745, 1.897386, 43.799279),
        glm::vec3(8.716487, 1.055845, 22.247738),
        glm::vec3(-7.843780, .644258, 5.800420),
        glm::vec3(-20.519054, 19.660023, -32.730717),
        glm::vec3(38.972025, 2.050676, 37.930135),
        glm::vec3(-34.578686, 4.421967, 24.077633),
    };
    // lantern light color
    glm::vec3 lanternLightColor = glm::vec3(5.f, 5.f, 1.f);



    // creating a shader program
    Shader modelShader("vertex.vs", "fragment.fs", "model");
    Shader lightCubeShader("lightCube.vs", "lightCube.fs", "lightCube");
    Shader blurShader("blur.vs", "blur.fs", "blur");
    Shader bloomShader("bloom.vs", "bloom.fs", "bloom");
    Shader skyboxShader("skybox.vs", "skybox.fs", "skybox");
    Shader waterShader("water.vs", "water.fs", "water");

    // creating a custom framebuffer
    Framebuffer hdrFB(RGBA, 2, true, false);
    Framebuffer pingpongFB1(RGBA, 1, false, false);
    Framebuffer pingpongFB2(RGBA, 1, false, false);

    // importing models
    Model ThingsBetwixt("resources/objects/ThingsBetwixt/ThingsBetwixtV2.1.obj");
    ThingsBetwixt.setShaderTextureNamePrefix("material.");
    SimpleShapes lightCube(CUBE);
    SimpleShapes water(PLANE, "resources/textures/water_texture_02.png", .15f);
    // creating a rectangle that represents the screen (for the framebuffer texture)
    SimpleShapes screenRectangle(PLANE);

    // initializing the light
    DirectionalLight directionalLight =
    {
        glm::vec3(.0f, -1.f, .0f),
        glm::vec3(.007f),
        glm::vec3(.007f),
        glm::vec3(0.f)
    };

    std::vector<PointLight> pointLights;
    for (unsigned int i = 0; i < settings::nrPointLights; ++i) {
        PointLight pl = {
            lanternPositions[i],

            glm::vec3(.0f),
             glm::vec3(.6f, .6f, .3f),
            glm::vec3(.5f, .5f, .1f),

            1.f, .09f, .032f
        };

        pointLights.push_back(pl);
    }

    Spotlight flashlight =
    {
        camera.position(),
        camera.front(),

        glm::vec3(.0f),
        glm::vec3(1.f),
        glm::vec3(.7f),

        1.f, .09f, .032f,
        glm::cos(glm::radians(5.f)),
        glm::cos(glm::radians(20.f)),
        settings::flashlightOn
    };

    Spotlight ErdtreeSpotlight =
    {
        glm::vec3(-6.9f, 27.0f, 21.33f),
        glm::vec3(-0.39f, 0.755f, -0.92f),

        glm::vec3(0.2f),
        glm::vec3(0.8f),
        glm::vec3(0.2f),

        1.f, .0f, .0f,
        glm::cos(glm::radians(35.f)),
        glm::cos(glm::radians(45.f)),
        true
    };

    // sending render independent info to shaders
    modelShader.activate();
    modelShader.setInt("nrPointLights", settings::nrPointLights);

    blurShader.activate();
    blurShader.setInt("image", 0);

    bloomShader.activate();
    bloomShader.setInt("scene", 0);
    bloomShader.setInt("blurred", 1);

    // skybox textures
    std::vector<std::string> milkyway
    {
        "resources/textures/milkyway/right.jpg",
        "resources/textures/milkyway/left.jpg",
        "resources/textures/milkyway/front.jpg",
        "resources/textures/milkyway/back.jpg",
        "resources/textures/milkyway/top.jpg",
        "resources/textures/milkyway/bottom.jpg",
    };

    // create a skybox
    Skybox skybox({milkyway});

    // print some default settings
    utils::printSettings();

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // calculating time passed since the last frame
        frame.current = glfwGetTime();
        frame.delta = frame.current - frame.last;
        frame.last = frame.current;

        // toggling tests
        settings::cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        settings::blend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        // processing input from previous frame
        processMovement(window);

        // drawing into the hdr framebuffer
        if (settings::bloom)
            hdrFB.activate();

        // clearing buffers for the current frame
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // everything that will be rendered goes below
        // activating the shader
        modelShader.activate();
        // sending camera information to shader
        modelShader.setVec3("viewPosition", camera._position);
        // sending lighting information to shader
        modelShader.setDirectionalLight("directionalLight", directionalLight);
        for (unsigned int i = 0; i < settings::nrPointLights; ++i)
            modelShader.setPointLight("pointLight[" + std::to_string(i) + "]", pointLights[i]);
        flashlight.position = camera.position();
        flashlight.direction = camera.front();
        flashlight.on = settings::flashlightOn;
        modelShader.setSpotlight("flashlight", flashlight);
        modelShader.setSpotlight("ErdtreeSpotlight", ErdtreeSpotlight);

        // indicate if we're using blinn-phong or phong
        modelShader.setBool("blinn", settings::blinn);

        // updating the view and projection matrices
        glm::mat4 view = glm::mat4(1.f);
        view = camera.getViewMatrix();
        glm::mat4 projection =
                glm::perspective(glm::radians(camera.fov()), (float)S_WIDTH/(float)S_HEIGHT, .1f, 10000.f);
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);
        // drawing the model
        glm::mat4 model;
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(.0f));
        model = glm::scale(model, glm::vec3(4.f));
        modelShader.setMat4("model", model);
        modelShader.setFloat("alpha", 1.f);
        modelShader.setFloat("material.shininess", 4.f);
        ThingsBetwixt.draw(modelShader);

        // drawing lantern lights
        lightCubeShader.activate();
        lightCubeShader.setVec3("color", lanternLightColor);
        for (unsigned int i = 0; i < settings::nrPointLights; ++i)
        {
            model = glm::mat4(1.f);
            model = glm::translate(model, lanternPositions[i]);
            model = glm::scale(model, glm::vec3(.25f));
            lightCube.draw(model, view, projection, lightCubeShader);
        }

        // fireflies

        // drawing water
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(18.f, -.3f, 5.f));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        model = glm::scale(model, glm::vec3(20.f));
        water.draw(model, view, projection, waterShader);

        // draw the skybox
        skybox.draw(model, view, projection, skyboxShader);

        if (settings::bloom)
        {
            // turn off hdr framebuffer
            hdrFB.deactivate();

            // blur bright fragments with a two-pass gaussian blur
            blurShader.activate();
            bool horizontal = true;
            bool firstIteration = true;
            unsigned nrPasses = 10; // double the nr of weights in blur shader
            for (unsigned i = 0; i < nrPasses; ++i) {
                horizontal ? pingpongFB1.activate() : pingpongFB2.activate();

                glActiveTexture(GL_TEXTURE0);
                blurShader.setBool("horizontal", horizontal);

                glBindTexture(GL_TEXTURE_2D,
                              firstIteration ? hdrFB.colorBuffers[1] :
                              (horizontal ? pingpongFB2.colorBuffers[0] : pingpongFB1.colorBuffers[0]));

                screenRectangle.draw();

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
            bloomShader.setBool("bloom", settings::bloom);
            bloomShader.setFloat("exposure", settings::exposure);
            screenRectangle.draw();

        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // clearing resources
    lightCube.del();
    screenRectangle.del();
    modelShader.del();
    lightCubeShader.del();
    blurShader.del();
    bloomShader.del();
    skyboxShader.del();
    // cleanly clearing glfw resources
    glfwTerminate();
}

void processMovement(GLFWwindow* window)
{
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
