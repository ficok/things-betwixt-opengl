#ifndef THINGSBETWIXT_SHADER_H
#define THINGSBETWIXT_SHADER_H

#include <iostream>
#include <string>
#include <glad/glad.h>

#include <utils.h>
#include <structures.h>

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

    // setting uniform structs
    void setDirectionalLight(const std::string& name, DirectionalLight& directionalLight)
    {
        this->setVec3(name + ".direction", directionalLight.direction);
        this->setVec3(name + ".ambient", directionalLight.ambient);
        this->setVec3(name + ".diffuse", directionalLight.diffuse);
        this->setVec3(name + ".specular", directionalLight.specular);
    }

    void setPointLight(const std::string& name, PointLight& pointLight)
    {
        this->setVec3(name + ".position", pointLight.position);
        this->setVec3(name + ".direction", pointLight.direction);
        this->setVec3(name + ".ambient", pointLight.ambient);
        this->setVec3(name + ".diffuse", pointLight.diffuse);
        this->setVec3(name + ".specular", pointLight.specular);
        this->setFloat(name + ".constant", pointLight.constant);
        this->setFloat(name + ".linear", pointLight.linear);
        this->setFloat(name + ".quadratic", pointLight.quadratic);
    }

    void setSpotlight(const std::string& name, Spotlight& spotlight)
    {
        this->setVec3(name + ".position", spotlight.position);
        this->setVec3(name + ".direction", spotlight.direction);
        this->setVec3(name + ".ambient", spotlight.ambient);
        this->setVec3(name + ".diffuse", spotlight.diffuse);
        this->setVec3(name + ".specular", spotlight.specular);
        this->setFloat(name + ".constant", spotlight.constant);
        this->setFloat(name + ".linear", spotlight.linear);
        this->setFloat(name + ".quadratic", spotlight.quadratic);
        this->setFloat(name + "cutoff", spotlight.cutoff);
        this->setFloat(name + "outerCutoff", spotlight.outerCutoff);
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
