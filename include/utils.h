#ifndef THINGSBETWIXT_UTILS_H
#define THINGSBETWIXT_UTILS_H

#include <string>
#include <fstream>
#include <sstream>

namespace utils
{
    // declarations
    std::string readFile(std::string path);

    // definitions
    std::string readFile(std::string path)
    {
        std::ifstream in(path);
        std::stringstream buffer;
        buffer << in.rdbuf();
        return buffer.str();
    }
}



#endif //THINGSBETWIXT_UTILS_H