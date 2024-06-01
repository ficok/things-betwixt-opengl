#ifndef THINGSBETWIXT_MODEL_H
#define THINGSBETWIXT_MODEL_H

#include <vector>
#include <string>
#include <unordered_map>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <shader.h>
#include <mesh.h>

class Model
{
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    std::unordered_map<std::string, Texture> loadedTextures;
    // needed for loading textures from file
    std::string directory;

    void loadModel(std::string path)
    {
        // creating the assimp importer
        Assimp::Importer importer;
        // setting process flags
        auto flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
        // generating the scene (importer will import all the data needed from the model)
        const aiScene* scene = importer.ReadFile(path, flags);
        // checking for errors
        assert((!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode), "model import failed");

        // recursively process nodes
        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        {
            // scene->mMeshes is an array of meshes in the model.
            // node->mMeshes is an array of indices that correspond to the meshes that this
            // current node consists of. so scene->mMeshes[node->mMeshes[i]] is a mesh for the current node.
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(convertMesh(mesh, scene));
        }

        // process the child nodes
        for (unsigned int i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i], scene);
    }

    Mesh convertMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // converting vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            // position
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
            /* texture coordinates
             * a mesh can have multiple texture coorindates. i only use one so i will only
             * process one. otherwise, i would need a loop (and the vertex struct would have, instead of
             * glm::vec2 texCoords, a std::vector<glm::vec2> texCoords
             */
            if (mesh->mTextureCoords[0])
            {
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
                // tangents
                vertex.tangent.x = mesh->mTangents[i].x;
                vertex.tangent.y = mesh->mTangents[i].y;
                vertex.tangent.z = mesh->mTangents[i].z;
                // bitangents
                vertex.bitangent.x = mesh->mBitangents[i].x;
                vertex.bitangent.y = mesh->mBitangents[i].y;
                vertex.bitangent.z = mesh->mBitangents[i].z;
            } else
                vertex.texCoords = glm::vec2(.0f);

            vertices.push_back(vertex);
        }

        // converting indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
               indices.push_back(face.mIndices[j]);
        }

        // converting textures
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadTextureFromMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);
        loadTextureFromMaterial(material, aiTextureType_SPECULAR, "texture_specular", textures);
        loadTextureFromMaterial(material, aiTextureType_NORMALS, "texture_normal", textures);
        loadTextureFromMaterial(material, aiTextureType_HEIGHT, "texture_height", textures);

        return Mesh(vertices, textures, indices);
    }

    void loadTextureFromMaterial(aiMaterial* material, aiTextureType type, std::string typeName, std::vector<Texture>& textures)
    {
        for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
            aiString filename;
            // store the texture file name into filename
            material->GetTexture(type, i, &filename);

            // load textures
            auto it = loadedTextures.find(filename.C_Str());
            if (it == loadedTextures.end())
            {
                // create a new texture
                Texture texture;
                texture.id = loadTextureFromFile(filename.C_Str());
                texture.type = typeName;
                texture.path = filename.C_Str();

                loadedTextures[filename.C_Str()] = texture;
                textures.push_back(texture);
            }
            else
                // load an already created texture
                textures.push_back(it->second);
        }
    }

    unsigned int loadTextureFromFile(const char* filename)
    {
        // construct the complete path to the texture
        std::string completePath(directory + "/" + filename);

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(completePath.c_str(), &width, &height, &nrComponents, 0);
        assert(data, "loading image data failed");

        GLenum format;
        switch (nrComponents)
        {
            case 1: format = GL_RED;  break;
            case 3: format = GL_RGB;  break;
            case 4: format = GL_RGBA; break;
            default:
                assert(false, "invalid image format");
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

public:
    Model(std::string path)
    {
        // save the directory of this model
        directory = path.substr(0, path.find_last_of('/'));
        loadModel(path);
    }

    void draw(Shader& shader)
    {
        for (Mesh& mesh: meshes)
            mesh.draw(shader);
    }

};

#endif //THINGSBETWIXT_MODEL_H
