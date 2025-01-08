#pragma once

#include "Model.h"
#include "MainWindow.h"
#include "KDTree.h"

class Relief {
public:
	Relief(const std::string path);

	void render();

	KDTree kdTree;
private:
	void _parseFile();

private:
	Model objModel;
	Model ball;

	glm::mat4 model;

	std::vector<glm::vec3> testBall;
};