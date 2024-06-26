#ifndef THINGSBETWIXT_MESH_H
#define THINGSBETWIXT_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <shader.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
private:
    // data that the mesh consists of
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;

public:
    std::string shaderIdentifierPrefix;
    // copying only once. can be reduced to zero with move constructors. research
    Mesh(std::vector<Vertex>& vs, std::vector<Texture>& tex, std::vector<unsigned int>& ind)
        : vertices(vs), textures(tex), indices(ind)
    {
        // sending vertex data to the GPU
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoords)));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, tangent)));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, bitangent)));

        glBindVertexArray(0);
    }

    void draw(Shader& shader)
    {
        // the number of each of the texture types differs across objects. this way
        // we find their exact numbers.
        unsigned int nrDiffuse = 1;
        unsigned int nrSpecular = 1;
        unsigned int nrNormal = 1;
        unsigned int nrHeight = 1;

        for (unsigned int i = 0; i < textures.size(); ++i)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            // we need to construct the name that will be in the shader
            std::string name = textures[i].type;
            std::string number;

            if (name == "texture_diffuse")
                number = std::to_string(nrDiffuse++);
            else if (name == "texture_specular")
                number = std::to_string(nrSpecular++);
            else if (name == "texture_normal")
                number = std::to_string(nrNormal++);
            else if (name == "texture_height")
                number = std::to_string(nrHeight++);
            else
                assert(false, "unknown texture type");

            name.insert(0, shaderIdentifierPrefix);
            name.append(number);
            shader.setInt(name, i);
            // setting the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // drawing from EBO
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

    void del()
    const {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        for (Texture tex: textures)
            glDeleteTextures(1, &tex.id);
    }
};

#endif //THINGSBETWIXT_MESH_H
