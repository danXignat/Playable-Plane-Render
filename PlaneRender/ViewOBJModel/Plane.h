#pragma once
#include "Includes.h"
#include "MainWindow.h"

class Plane {
public:
	static constexpr const char* const relativePath{ "\\Models\\Plane\\source\\PlaneClosedChute.obj" };
	static constexpr const float baseStepX{ 0.0f };
	static constexpr const float baseStepY{ 0.02f };
	static constexpr const float baseStepZ{ -0.05f };

public:
	Plane(const std::string&);

	void render();

private:
	Model planeObjModel;
	glm::mat4 planeModel;

	glm::vec3 planeMovement;
	float rotationDeg;
	float tiltDeg;
	double direct;
	float stepX;
	float stepY;
	float stepZ;
	float acceleration;
};