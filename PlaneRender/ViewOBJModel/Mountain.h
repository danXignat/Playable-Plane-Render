#pragma once

#include "Model.h"
#include "MainWindow.h"
#include "KDTree.h"

class Mountain {
public:
	Mountain(const std::string path);

	void render();
	bool inArea(const glm::vec3& point);

	KDTree kdTree;
private:
	void _parseFile();

private:
	Model objModel;
	Model ball;

	glm::vec4 bottomLeft{ 0.f, 0.0f, 0.f, 1.0f };
	glm::vec4 bottomRight{ 1000.f, 0.0f, 0, 1.0f };
	glm::vec4 topLeft{ 0.f, 0.0f, -1000.f, 1.0f };
	glm::vec4 topRight{1000.f, 0.0f, -1000.f, 1.0f};

	glm::mat4 model;

	std::vector<glm::vec3> testBall;
};