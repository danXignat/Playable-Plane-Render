#pragma once
#include "Includes.h"
#include "MainWindow.h"
#include "KDTree.h"

class Plane {
public:
	static constexpr const char* const relativePath{ "\\Models\\Plane\\source\\PlaneClosedChute.obj" };
	static constexpr const float baseStepX{ -0.1f }; //-0.05
	static constexpr const float baseStepY{ 0.0002f }; //0.0002
	static constexpr const float baseStepZ{ -0.1f }; //-0.05

public:
	Plane(const std::string& path, Camera& pCamera);

	void processPlaneInput(GLFWwindow* window);
	glm::vec3 getTranslation() const;
	glm::mat4 getRotation();
	void render();

	void checkCollison(KDTree& tree);
	glm::vec3 getTipPlaneModel();

	bool isOnPlane();

private:
	void _updateCamera();

private:
	Model ball;
	Model planeObjModel;
	Camera& pCamera;

	glm::vec3 cameraOffset;
	glm::vec3 planeMovement;

	glm::vec3 tipPlaneOffset;
	glm::vec3 anteriorTipPlane;
	glm::vec3 tipPlane;

	glm::mat4 planeModel;
	glm::mat4 planeRenderModel;

	bool move;
	bool collision;
	float rotationDeg;
	float tiltDeg;
	float turnDeg;
	float stepX;
	float stepY;
	float stepZ;
	float acceleration;
};