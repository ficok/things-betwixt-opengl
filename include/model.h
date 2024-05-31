#ifndef THINGSBETWIXT_MODEL_H
#define THINGSBETWIXT_MODEL_H

#include <vector>
#include <string>

#include <shader.h>
#include <mesh.h>

class Model
{
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures;
    std::string directory;

    void loadModel(std::string path);

public:
    Model(std::string path)
    {
        loadModel(path);
    }

    draw(Shader& shader)
    {
        for (Mesh& mesh: meshes)
            mesh.draw(shader);
    }

};

#endif //THINGSBETWIXT_MODEL_H
