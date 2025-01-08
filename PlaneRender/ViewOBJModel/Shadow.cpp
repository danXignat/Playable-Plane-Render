#include "Shadow.h"



Shadow::Shadow(const char* vertexPath, const char* fragmentPath)
	: SHADOW_WIDTH{ 4096 }, SHADOW_HEIGHT{4096}, shadowShader{vertexPath, fragmentPath}
{
	glGenFramebuffers(1, &depthMapFBO);

	// Create the depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach the depth texture to the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shader& Shadow::getShader()
{
	return shadowShader;
}

void Shadow::RenderShadowMap(Shader& mainShader, const glm::mat4& lightSpaceMatrix, std::vector<Model>& sceneObjects, std::vector<glm::mat4> sceneModels, const glm::vec3& lightPosition, const Camera& pCamera ) {
	// Set viewport to the shadow map's resolution
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	// Bind the framebuffer for shadow mapping
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Use the shadow shader
	shadowShader.use();
	shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	// Render all objects in the scene from the light's perspective
	int l = sceneObjects.size();

	for (int i = 0; i < l; i++) {
		Model& object = sceneObjects[i];
		glm::mat4& modelMatrix = sceneModels[i];
		shadowShader.setMat4("model", modelMatrix);
		object.Draw(shadowShader); // Assuming the Model class has a Draw method
	}

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Set viewport to screen resolution

	mainShader.use();
	mainShader.setMat4("view", pCamera.GetViewMatrix());
	mainShader.setMat4("projection", pCamera.GetProjectionMatrix());
	mainShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	mainShader.SetVec3("lightPos", lightPosition);
	mainShader.SetVec3("viewPos", pCamera.GetPosition());

	// Bind the shadow map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	mainShader.setInt("shadowMap", 1);

	for (int i = 0; i < l; i++) {
		Model& object = sceneObjects[i];
		glm::mat4& modelMatrix = sceneModels[i];
		mainShader.setMat4("model", modelMatrix);
		object.Draw(mainShader); // Assuming the Model class has a Draw method
	}

	// Reset viewport to screen dimensions (assuming SCR_WIDTH and SCR_HEIGHT are defined)
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}
