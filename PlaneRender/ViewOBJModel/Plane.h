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

	std::string rootPath;

	void processPlaneInput(GLFWwindow* window);
	void render();
	Model& getObject();
	glm::mat4& getModel();
	glm::mat4 getRotation();
	glm::vec3 getTranslation() const;
	glm::vec3 getPosition();

	void checkCollison(KDTree& tree);
	glm::vec3 getTipPlaneModel();

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
	glm::vec3 planeBase;

	glm::mat4 planeModel;
	glm::mat4 planeRenderModel;

	bool sound_start;

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