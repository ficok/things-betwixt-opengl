#ifndef THINGSBETWIXT_SIMPLE_SHAPES_H
#define THINGSBETWIXT_SIMPLE_SHAPES_H

#include <glad/glad.h>
#include <stb_image.h>

#include <shader.h>
#include <utils.h>

enum ShapeType
{
    CUBE, PLANE
};

class SimpleShapes
{
public:
    SimpleShapes(ShapeType type): shapeType(type)
    {
        // generating array and buffer
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // binding array and buffer to array buffer
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        switch (shapeType)
        {
            case CUBE: loadCube(); break;
            case PLANE: loadPlane(); break;
            default:
                assert(false, "ERROR [SimpleShapes]: invalid shape type");
        }
        // unbinding buffer and vertex array
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    SimpleShapes(ShapeType type, std::string pathToTexture, float alpha = 1.f): shapeType(type)
    {
        this->alpha = alpha;
        loadTexture(pathToTexture);
        // generating array and buffer
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // binding array and buffer to array buffer
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        switch (shapeType)
        {
            case CUBE: loadCube(); break;
            case PLANE: loadPlane(); break;
            default:
                assert(false, "ERROR [SimpleShapes]: invalid shape type");
        }
        // unbinding buffer and vertex array
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    SimpleShapes(SimpleShapes& shape)
    {
        this->shapeType = shape.shapeType;
        this->VAO = shape.VAO;
        this->VBO = shape.VBO;
        this->texture = shape.texture;
        this->alpha = shape.alpha;
    }

    // SimpleShapes draw function with matrices and shader
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, Shader& shader)
    {
        shader.activate();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("color", color);

        if (this->usingTexture())
        {
            // activate texture
            glActiveTexture(GL_TEXTURE0);

            // tell shader that is should draw the texture
            shader.setBool("usingTexture", true);
            // set the texture number to draw
            shader.setInt("tex", 0);
            // set the alpha
            shader.setFloat("alpha", alpha);

            // bind texture
            glBindTexture(GL_TEXTURE_2D, this->texture);
        }
        else
        {
            shader.setBool("usingTexture", false);
            shader.setFloat("alpha", alpha);
        }

        glBindVertexArray(VAO);
        switch (shapeType)
        {
            case CUBE:
                glDrawArrays(GL_TRIANGLES, 0, 36);
                break;
            case PLANE:
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
        }
        glBindVertexArray(0);
        if (usingTexture())
            glActiveTexture(GL_TEXTURE0);
    }
    // SimpleShape draw function that just draws from VAO
    void draw()
    {
        glBindVertexArray(VAO);
        switch (shapeType)
        {
            case CUBE:
                glDrawArrays(GL_TRIANGLES, 0, 36);
                break;
            case PLANE:
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
        }
        glBindVertexArray(0);
    }

    void setColor(const glm::vec3& newColor)
    {
        this->color = newColor;
    }

    // deletes VAO and VBO
    void del()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    ShapeType shapeType;
    unsigned VBO, VAO;
    unsigned int texture = -1;
    float alpha;
    glm::vec3 color;

    // loads the cube's attributes
    void loadCube()
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(data::cubeVerticesWNormalsTextures),
                     &data::cubeVerticesWNormalsTextures, GL_STATIC_DRAW);
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        // normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // textures
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

    // loads the plane's attributes
    void loadPlane()
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(data::rectangleVertices),
                     &data::rectangleVertices, GL_STATIC_DRAW);
        // position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
        // textures
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    void loadTexture(std::string& pathToTexture)
    {
        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        std::cout << "INFO [SimpleShapes]: loading texture at path " << pathToTexture << ".\n";
        int width, height, nrChannels;
        unsigned char* data = stbi_load(pathToTexture.c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            GLenum format;
            switch (nrChannels)
            {
                case 1: format = GL_RED;  break;
                case 3: format = GL_RGB;  break;
                case 4: format = GL_RGBA; break;
                default: assert(false, "ERROR [SimpleShapes]: invalid color format");
            }

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "INFO [SimpleShapes]: loading succeeded.\n";
        }
        else
            std::cerr << "ERROR [SimpleShapes]: failed to load texture at path: " << pathToTexture << ".\n";

        stbi_image_free(data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    bool usingTexture()
    {
        return texture != -1;
    }
};

#endif //THINGSBETWIXT_SIMPLE_SHAPES_H
