#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "Shader.h"
#include "settings.h"
#include "Camera.h"

class Skybox {
public:
    unsigned int VAO, VBO, textureID;

    Skybox(const std::string& path, const Camera& pCamera,float time);
    void render();

private:
    const Camera& pCamera;
    Shader skyboxShader;
    unsigned int loadCubemap(const std::vector<std::string>& faces);
};