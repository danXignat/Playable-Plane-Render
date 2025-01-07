#pragma once
#include "Includes.h"
#include "MainWindow.h"

class Plane {
public:
	static constexpr const char* const relativePath{ "\\Models\\Plane\\source\\PlaneClosedChute.obj" };
	static constexpr const float baseStepX{ -0.05f };
	static constexpr const float baseStepY{ 0.0002f };
	static constexpr const float baseStepZ{ -0.05f };

public:
	Plane(const std::string& path, Camera& pCamera);

	void processPlaneInput(GLFWwindow* window);
	glm::vec3 getTranslation() const;
	glm::mat4 getRotation();
	void render();

private:
	void _updateCamera();

private:
	Camera& pCamera;
	glm::vec3 cameraOffset;

	Model planeObjModel;
	glm::mat4 planeModel;
	glm::mat4 planeRenderModel;

	glm::vec3 planeMovement;
	float rotationDeg;
	float tiltDeg;
	float turnDeg;
	float stepX;
	float stepY;
	float stepZ;
	float acceleration;
};