#include "Plane.h"
#include "utils.h"

Plane::Plane(const std::string& path) :
	planeMovement	{ 0.0f, 0.0f, 0.0f },
	rotationDeg		{ 0.0f },
	tiltDeg			{ 0.0f },
	direct			{ glm::sqrt(glm::pow(baseStepX, 2) + glm::pow(baseStepZ, 2)) },
	stepX			{ baseStepX },
	stepY			{ baseStepY },
	stepZ			{ baseStepZ },
	acceleration	{ 0.0f },

	planeObjModel{ path + relativePath , false },
	planeModel{ glm::scale(glm::mat4(1.0), glm::vec3(1.0f)) }
{
	planeModel = glm::translate(planeModel, glm::vec3(0.0f, 25.0f, 0.0f));
	planeModel = glm::translate(planeModel, planeMovement);
	planeModel = glm::rotate(planeModel, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	planeModel = glm::rotate(planeModel, glm::radians(tiltDeg), glm::vec3(1, 0, 0));
}

void Plane::render() {
	utils::DrawModel(MainWindow::instance().lightingWithTextureShader, planeModel, planeObjModel);
}