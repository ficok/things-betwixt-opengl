#ifndef THINGSBETWIXT_UTILS_H
#define THINGSBETWIXT_UTILS_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace utils {
    // declarations
    std::string readFile(std::string path);

    // definitions
    std::string readFile(std::string path) {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
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
}

#endif //THINGSBETWIXT_UTILS_H
