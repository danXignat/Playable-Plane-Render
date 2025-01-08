#pragma once
#include "Includes.h"
#include "Shader.h"
#include "Model.h"
#include "settings.h"
#include "Camera.h"

class Shadow
{
public:
    const unsigned int SHADOW_WIDTH;
    const unsigned int SHADOW_HEIGHT;
    unsigned int depthMap;
    unsigned int depthMapFBO;

    Shader shadowShader;

	Shadow(const char* vertexPath, const char* fragmentPath);

    Shader& getShader();
    void RenderShadowMap(Shader& mainShader, const glm::mat4& lightSpaceMatrix, std::vector<Model>& sceneObjects, std::vector<glm::mat4> sceneModels, const glm::vec3& lightPosition, const Camera& pCamera);

};

