#ifndef THINGSBETWIXT_SHADER_H
#define THINGSBETWIXT_SHADER_H

#include <iostream>
#include <string>
#include <glad/glad.h>

#include <utils.h>
#include <structures.h>
#include <errors.h>

class Shader
{
public:
    unsigned _programID;
    std::string _programName;
    // constructor
    Shader(const std::string& _vertexPath, const std::string& _fragmentPath, const std::string& name)
    {
        std::cout << "INFO [shader]\n";
        // appending source folder
        std::string vertexPath = utils::shadersDir(_vertexPath);
        std::string fragmentPath = utils::shadersDir(_fragmentPath);

        _programName = name;
        // getting shader source code
        std::string vertexShaderString = utils::readFile(vertexPath);
        const char* vertexShaderSource = vertexShaderString.c_str();
        std::string fragmentShaderString = utils::readFile(fragmentPath);
        const char* fragmentShaderSource = fragmentShaderString.c_str();
        // asserting we have the right files
        assert(!vertexShaderString.empty(), "ERROR [vertex shader: " + vertexPath + "] is empty!");
        assert(!fragmentShaderString.empty(), "ERROR [fragment shader: " + fragmentPath + "] is empty!");
        // creating the vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        std::cout << "INFO [vertex shader: " << vertexPath <<"]: compiling...\n";
        // checking for errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        assert(success, "ERROR [vertex shader]: compilation failed!\n- vertex shader path: "
            + vertexPath + "\n- info log:\n" + infoLog + "\n");

        std::cout << "INFO [vertex shader: " << vertexPath <<"]: done!\n";
        // creating the fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        std::cout << "INFO [fragment shader: " << fragmentPath <<"]: compiling...\n";
        // checking for errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        assert(success, "ERROR [fragment shader]: compilation failed!\n- fragment shader path: "
            + fragmentPath + "\n- info log:\n" + infoLog + "\n");
        std::cout << "INFO [fragment shader: " << fragmentPath <<"]: done!\n";

        // linking shaders into shader program
        _programID = glCreateProgram();
        std::cout << "INFO [shader program: " << _programName << "]: attaching shaders...\n";
        glAttachShader(_programID, vertexShader);
        glAttachShader(_programID, fragmentShader);
        std::cout << "INFO [shader program: " << _programName << "]: linking program...\n";
        glLinkProgram(_programID);
        // checking for errors
        glGetProgramiv(_programID, GL_LINK_STATUS, &success);
        glGetProgramInfoLog(_programID, 512, nullptr, infoLog);
        assert(success, "ERROR [shader program]: linking failed!\n- shader program name: "
            + _programName + "\n- info log: \n" + infoLog + "\n");
        std::cout << "INFO [shader program: " << _programName << "]: done!\n----------\n\n";
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
        this->setFloat(name + ".cutoff", spotlight.cutoff);
        this->setFloat(name + ".outerCutoff", spotlight.outerCutoff);
        this->setBool(name + ".on", spotlight.on);
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

    void setVec4(const std::string& name, const glm::vec3& vector)
    {
        glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &vector[0]);
    }

    void setFloat(const std::string& name, const float value)
    const {
        glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
    }

    void setBool(const std::string& name, const bool value)
    const {
        glUniform1i(glGetUniformLocation(_programID, name.c_str()), (int)value);
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
