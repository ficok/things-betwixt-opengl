#ifndef THINGSBETWIXT_SKYBOX_H
#define THINGSBETWIXT_SKYBOX_H

#include <glad/glad.h>
#include <shader.h>
#include <utils.h>
#include <stb_image.h>

extern Camera camera;
/* supports multiple skyboxes and cycling between them */
class Skybox
{
private:
    std::vector<unsigned int> cubemapTextures;
    unsigned int skyboxVAO, skyboxVBO;
    int nrOfTextures = 0;
    int currentTexture = 0;

    // create a cubemap texture out of the given image files
    unsigned int loadCubemap(std::vector<std::string> cubemap)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        int width, height, nrChannels;
        for (unsigned int i = 0; i < cubemap.size(); i++)
        {
            std::cout << "INFO [skybox: loading cubemaps]: now loading " << cubemap[i] << ".\n";
            unsigned char *data = stbi_load(cubemap[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else
                std::cout << "ERROR [skybox]: cubemap texture failed to load at path: " << cubemap[i] << ".\n";

            stbi_image_free(data);
        }

        std::cout << "----------\n\n";

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }
public:

    Skybox(std::vector<std::vector<std::string>> cubemaps)
    {
        // loading cubemap textures into texture vector
        for (auto cubemap: cubemaps)
        {
            cubemapTextures.push_back(loadCubemap(cubemap));
            nrOfTextures++;
        }

        // generating the cube on which the textures will be mapped
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data::cubeVertices), &data::cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    }

    // draw the current cubemap texture
    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, Shader& skyboxShader)
    {
        // change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc(GL_LEQUAL);
        skyboxShader.activate();
        // remove translation from the view matrix
        view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        model = glm::mat4(1.0);
        // rotate the model
        model = glm::rotate(model, glm::radians(195.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        skyboxShader.setMat4("model", model);
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // drawing the skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextures[currentTexture]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // set depth function back to default
        glDepthFunc(GL_LESS);
    }

    // set a specific cubemap
    void changeCubemap(int x)
    {
        if (x >= nrOfTextures)
            std::cerr << "WARNING [skybox: change cubemap]: position " << x << " is out of bounds. nothing will change.\n";
        else
            currentTexture = x;
    }

    // cycle to the next cubemap
    void cycleCubemap()
    {
        if (currentTexture + 1 >= nrOfTextures)
            currentTexture = 0;
        else
            currentTexture++;
    }
};

#endif //THINGSBETWIXT_SKYBOX_H
