#pragma once

#include <vector>
#include <string>
//#include <glad/glad.h>
#include "Shader.h"

class Skybox {
public:
    unsigned int VAO, VBO, textureID;

    Skybox(const std::vector<std::string>& faces);
    void Draw(const Shader& shader);

private:
    unsigned int loadCubemap(const std::vector<std::string>& faces);
};