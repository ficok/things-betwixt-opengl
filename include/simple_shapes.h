#ifndef THINGSBETWIXT_SIMPLE_SHAPES_H
#define THINGSBETWIXT_SIMPLE_SHAPES_H

#include <glad/glad.h>

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

    // SimpleSHape draw function with matrices and shader
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, Shader& shader)
    {
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

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

    // deletes VAO and VBO
    void del()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    ShapeType shapeType;
    unsigned VBO, VAO;

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
};

#endif //THINGSBETWIXT_SIMPLE_SHAPES_H
