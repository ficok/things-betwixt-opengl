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

// variables that toggle states and variables that are used for values
namespace settings
{
    // values
    float exposure = .05f;
    int nrPointLights = 6;

    // toggles
    bool flashlightOn = false;
    bool blinn = true;
    bool cull = false;
    bool blend = true;
    bool hdr = true;
    bool bloom = true;
}

// utility functions
namespace utils
{
    // declarations
    std::string readFile(const std::string &path);
    std::string shadersDir(const std::string &path);
    std::string objectsDir(const std::string &path);
    void printSettings();

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

    void printSettings()
    {
        std::cout <<
            "INFO: default settings:\n" <<
            "- flashlight is " << (settings::flashlightOn ? "on;\n" : "off;\n") <<
            "- face culling is " << (settings::cull ? "on;\n" : "off;\n") <<
            "- transparency is " << (settings::blend ? "on;\n" : "off;\n") <<
            "- using " << (settings::blinn ? "blinn-phong's model;\n" : "phong's model;\n") <<
            "- exposure is " << settings::exposure << ";\n" <<
            "- bloom is " << (settings::bloom ? "on.\n" : "off.\n") <<
            "----------\n\n";
    }
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
