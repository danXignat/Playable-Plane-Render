#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "Shader.h"
#include "settings.h"

class Skybox {
public:
    unsigned int VAO, VBO, textureID;

    Skybox(const std::string& path);
    void Draw(const Shader& shader);

private:
    unsigned int loadCubemap(const std::vector<std::string>& faces);
};