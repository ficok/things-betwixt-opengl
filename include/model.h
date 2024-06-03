#ifndef THINGSBETWIXT_MODEL_H
#define THINGSBETWIXT_MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

class Model
{
public:
    // constructor, expects a filepath to a 3D model.
    Model(const std::string& path)
    {
        // save the model's directory
        directory = path.substr(0, path.find_last_of('/'));
        std::cout << "INFO [model]: model's directory is " << directory << ".\n";
        loadModel(path);
    }

    // draws the model - draws all its meshes
    void draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); ++i)
            meshes[i].draw(shader);
    }

    void setShaderTextureNamePrefix(std::string prefix)
    {
        for (Mesh& mesh: meshes)
            mesh.shaderIdentifierPrefix = prefix;
    }

    void del()
    const {
        // textures don't need to be deleted, because that is done inside mesh.del()
        for (Mesh mesh: meshes)
            mesh.del();
    }

private:
    // model data
    /* stores already loaded textures with efficient searching.
     * this way we don't need to check the load status of every texture. */
    std::unordered_map<std::string, Texture> loadedTextures;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(const std::string& path)
    {
        // creating the assimp importer
        Assimp::Importer importer;
        // importing model data
        auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
        const aiScene* scene = importer.ReadFile(path, flags);
        // check for errors
        assert(!(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode),
               "failed to import model: " + std::string(importer.GetErrorString()));

        // recursively process all nodes
        processNode(scene->mRootNode, scene);
        std::cout << "----------\n\n";
    }

    void processNode(aiNode *node, const aiScene *scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(convertMesh(mesh, scene));
        }

        // recursively process this node's child nodes
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i], scene);
    }

    Mesh convertMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            // vertex
            Vertex vertex;
            // positions
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // normals
            if (mesh->HasNormals())
            {
                vertex.normal.x = mesh->mNormals[i].x;
                vertex.normal.y = mesh->mNormals[i].y;
                vertex.normal.z = mesh->mNormals[i].z;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0])
            {
                // a vertex can contain multiple texture coordinates. for my purpose, i use only one, so
                // i will only access the first (0)
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
                // tangent
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;
                // bitangent
                vertex.bitangent.x = mesh->mBitangents[i].x;
                vertex.bitangent.y = mesh->mBitangents[i].y;
                vertex.bitangent.z = mesh->mBitangents[i].z;
            }
            else
                vertex.texCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        // indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        // scene contains all of the materials; this mesh contains the index which corresponds to
        // it's material in scene's material array
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
        loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", textures);
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", textures);

        return Mesh(vertices, textures, indices);
    }

    std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, std::vector<Texture>& textures)
    {
        for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
        {
            aiString str;
            material->GetTexture(type, i, &str);
            // if the texture has already been loaded, just take it and continue
            auto it = loadedTextures.find(str.C_Str());
            if (it != loadedTextures.end())
                textures.push_back(it->second);
            else
            {   // if texture hasn't been loaded already, create it and load it
                Texture texture;
                texture.id = loadTextureFromFile(str.C_Str());
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                loadedTextures[str.C_Str()] = texture;  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                std::cout << "INFO [model]: creating texture from " << texture.path << ".\n";
            }
        }

        return textures;
    }

    unsigned int loadTextureFromFile(const char *path)
    {
        std::string filename = std::string(path);
        filename = this->directory + '/' + filename;
        std::cout << "INFO [model]: now loading file: " << filename << ".\n";

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            switch (nrComponents)
            {
                case 1: format = GL_RED; break;
                case 3: format = GL_RGB; break;
                case 4: format = GL_RGBA; break;
                default: assert(false, "invalid color format");
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
            std::cout << "WARNING [model]: texture failed to load at path: " << path << std::endl;

        stbi_image_free(data);

        return textureID;
    }
};

#endif
