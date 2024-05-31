#ifndef THINGSBETWIXT_MESH_H
#define THINGSBETWIXT_MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <stddef.h>

#include <shader.h>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    glm::vec3 tangent;
    glm::vec3 bitangent;
};

class Mesh
{
private:
    std::vector<Vertex> vertices;
    std::vector<Textures> textures;
    std::vector<unsigned int> indices;
public:
    Mesh(std::vector<Vertex>& vs, std::vector<Texture>& tex, std::vector<unsigned int>& ind)
    {
        vertices = vs;
        textures = tex;
        indices = ind;

        unsigned int VBO, EBO, VAO;
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

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

    void draw(Shader& shader);

};

#endif //THINGSBETWIXT_MESH_H
