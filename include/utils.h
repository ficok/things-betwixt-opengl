#ifndef THINGSBETWIXT_UTILS_H
#define THINGSBETWIXT_UTILS_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// ignore warnings about unused parameters
#define UNUSED(x)(void)x

namespace utils {
    // declarations
    std::string readFile(const std::string& path);
    std::string shadersDir(const std::string& path);
    std::string objectsDir(const std::string& path);

    // definitions
    std::string readFile(const std::string& path) {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }

    std::string shadersDir(const std::string& path)
    {
        return "resources/shaders/" + path;
    }

    std::string objectsDir(const std::string& path)
    {
        return "resources/objects/" + path;
    }

    // useful data
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
}

#endif //THINGSBETWIXT_UTILS_H
