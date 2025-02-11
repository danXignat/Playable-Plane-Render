#pragma once
#include "Includes.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

namespace utils {
	inline void LoadLightningShader(const Camera& pCamera, Shader& _shader, const glm::vec3& _lightPos) {
		_shader.use();
		_shader.SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
		_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		_shader.SetVec3("lightPos", _lightPos);
		_shader.SetVec3("viewPos", pCamera.GetPosition());
		_shader.setMat4("projection", pCamera.GetProjectionMatrix());
		_shader.setMat4("view", pCamera.GetViewMatrix());
	}

	inline void LoadLighningTextureShaders(const Camera& pCamera, Shader& _shader, const glm::vec3& _lightPos) {
		_shader.use();
		_shader.SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
		_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		_shader.SetVec3("lightPos", _lightPos);
		_shader.SetVec3("viewPos", pCamera.GetPosition());
		_shader.setInt("texture_diffuse1", 0);
		_shader.setMat4("projection", pCamera.GetProjectionMatrix());
		_shader.setMat4("view", pCamera.GetViewMatrix());
	}

	inline void DrawModel(Shader& _shader, glm::mat4& _model_matrix, Model& _model) {
		_shader.setMat4("model", _model_matrix);
		_model.Draw(_shader);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------


}
