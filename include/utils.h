#ifndef THINGSBETWIXT_UTILS_H
#define THINGSBETWIXT_UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// ignore warnings about unused parameters
#define UNUSED(x)(void)x
#define S_WIDTH  2000
#define S_HEIGHT 1500
#define TITLE "Things betwixt"

// utility functions
namespace utils
{
    // declarations
    std::string readFile(const std::string &path);
    std::string shadersDir(const std::string &path);
    std::string objectsDir(const std::string &path);
    void printInfo(bool blend, bool cull, bool blinn, bool flashlightOn);

    // definitions
    std::string readFile(const std::string &path) {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    std::string shadersDir(const std::string &path) {
        return "resources/shaders/" + path;
    }

    std::string objectsDir(const std::string &path) {
        return "resources/objects/" + path;
    }

    void printInfo(const bool blend, const bool cull, const bool blinn, const bool flashlightOn)
    {
        std::cout <<
            "INFO: default settings:\n" <<
            "- flashlight is " << (flashlightOn ? "on;\n" : "off;\n") <<
            "- face culling is " << (cull ? "on;\n" : "off;\n") <<
            "- transparency is " << (blend ? "on;\n" : "off;\n") <<
            "- using " << (blinn ? "blinn-phong's model.\n" : "phong's model.\n") <<
            "----------\n\n";
    }
}

// variables that indicate the toggle state
namespace settings
{
    // values
    float exposure = .05f;

    // toggles
    bool flashlightOn = false;
    bool blinn = true;
    bool cull = false;
    bool blend = true;
    bool postprocessing = false;
    bool hdr = true;
    bool bloom = true;
}

// variables containing vertex information
namespace data
{
    float cubeVertices[] =
    {
        // positions
        -.5f, .5f, -.5f,
        -.5f, -.5f, -.5f,
        .5f, -.5f, -.5f,
        .5f, -.5f, -.5f,
        .5f, .5f, -.5f,
        -.5f, .5f, -.5f,

        -.5f, -.5f, .5f,
        -.5f, -.5f, -.5f,
        -.5f, .5f, -.5f,
        -.5f, .5f, -.5f,
        -.5f, .5f, .5f,
        -.5f, -.5f, .5f,

        .5f, -.5f, -.5f,
        .5f, -.5f, .5f,
        .5f, .5f, .5f,
        .5f, .5f, .5f,
        .5f, .5f, -.5f,
        .5f, -.5f, -.5f,

        -.5f, -.5f, .5f,
        -.5f, .5f, .5f,
        .5f, .5f, .5f,
        .5f, .5f, .5f,
        .5f, -.5f, .5f,
        -.5f, -.5f, .5f,

        -.5f, .5f, -.5f,
        .5f, .5f, -.5f,
        .5f, .5f, .5f,
        .5f, .5f, .5f,
        -.5f, .5f, .5f,
        -.5f, .5f, -.5f,

        -.5f, -.5f, -.5f,
        -.5f, -.5f, .5f,
        .5f, -.5f, -.5f,
        .5f, -.5f, -.5f,
        -.5f, -.5f, .5f,
        .5f, -.5f, .5f
    };
    float cubeVerticesWNormalsTextures[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float rectangleVertices[] =
    {
        // positions             // texCoords
        -1.0f,  1.0f, .0f,   0.0f, 1.0f,
        -1.0f, -1.0f, .0f,  0.0f, 0.0f,
         1.0f, -1.0f, .0f, 1.0f, 0.0f,

        -1.0f,  1.0f, .0f,  0.0f, 1.0f,
         1.0f, -1.0f, .0f, 1.0f, 0.0f,
         1.0f,  1.0f, .0f, 1.0f, 1.0f
    };
}

#endif //THINGSBETWIXT_UTILS_H
