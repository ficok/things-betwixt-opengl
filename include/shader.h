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
    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& name)
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
        std::cout << "INFO [vertex shader: " << vertexPath <<"]: compiling..." << std::endl;
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

        std::cout << "INFO [vertex shader: " << vertexPath <<"]: done!" << std::endl;
        // creating the fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        std::cout << "INFO [fragment shader: " << fragmentPath <<"]: compiling..." << std::endl;
        // checking for errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "ERROR [fragment shader]: compilation failed!\n"
            << "- fragment shader path: " << fragmentPath << "\n- info log:\n" << infoLog << std::endl;
        }
        std::cout << "INFO [fragment shader: " << fragmentPath <<"]: done!" << std::endl;

        // linking shaders into shader program
        _programID = glCreateProgram();
        std::cout << "INFO [shader program: " << _programName << "]: attaching shaders..." << std::endl;
        glAttachShader(_programID, vertexShader);
        glAttachShader(_programID, fragmentShader);
        std::cout << "INFO [shader program: " << _programName << "]: linking program..." << std::endl;
        glLinkProgram(_programID);
        // checking for errors
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
            std::cerr << "ERROR [shader program]: linking failed!\n"
            << "- shader program name: " << _programName << "\n- info log: \n" << infoLog << std::endl;
        }
        std::cout << "INFO [shader program: " << _programName << "]: done!" << std::endl
        << "----------\n\n";
        // cleanup
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // setting uniforms
    void setMat4(const std::string& name, glm::mat4& matrix)
    const {
        glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }

    void setVec3(const std::string& name, const glm::vec3& vector)
    const {
        glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &vector[0]);
    }

    void setFloat(const std::string& name, const float value)
    const {
        glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
    }

    // methods
    void activate()
    const {
        glUseProgram(_programID);
    }

    void del()
    const {
        glDeleteProgram(_programID);
    }
};

#endif //THINGSBETWIXT_SHADER_H
