#ifndef THINGSBETWIXT_SHADER_H
#define THINGSBETWIXT_SHADER_H

#include <iostream>
#include <string>
#include <glad/glad.h>

#include <utils.h>

class Shader
{
public:
    unsigned _programID;
    std::string _programName;
    // constructor
    /*
     * TODO 1: print info
     * TODO 2: create an info printing abstraction
    */
    Shader(std::string vertexPath, std::string fragmentPath, std::string name)
    {
        _programName = name;
        // getting shader source code
        // TODO: check for empty source and exit program
        std::string vertexShaderString = utils::readFile(vertexPath);
        const char* vertexShaderSource = vertexShaderString.c_str();
        std::string fragmentShaderString = utils::readFile(fragmentPath);
        const char* fragmentShaderSource = fragmentShaderString.c_str();

        // creating the vertex shader
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
            std::cerr << "ERROR [vertex shader]: compilation failed!\n"
            << "- vertex shader path: " << vertexPath << "\n- info log:\n" << infoLog << std::endl;
        }

        // creating the fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        // checking for errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR [fragment shader]: compilation failed!\n"
            << "- fragment shader path: " << fragmentPath << "\n- info log:\n" << infoLog << std::endl;
        }

        // linking shaders into shader program
        _programID = glCreateProgram();
        glAttachShader(_programID, vertexShader);
        glAttachShader(_programID, fragmentShader);
        glLinkProgram(_programID);
        // checking for errors
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
            std::cerr << "ERROR [shader program]: linking failed!\n"
            << "- shader program name: " << _programName << "\n- info log: \n" << infoLog << std::endl;
        }

        // cleanup
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // methods
    void activate()
    {
        glUseProgram(_programID);
    }

    void del()
    {
        glDeleteProgram(_programID);
    }
};

#endif //THINGSBETWIXT_SHADER_H
