#include "Plane.h"
#include "utils.h"

Plane::Plane(const std::string& path) :
	planeMovement	{ 0.0f, 0.0f, 0.0f },
	rotationDeg		{ 0.0f },
	tiltDeg			{ 0.0f },
	turnDeg			{ 0.0f },
	stepX			{ 0 },
	stepY			{ 0 },
	stepZ			{ 0 },
	acceleration	{ 0.0f },

	planeObjModel{ path + relativePath , false },
	planeModel{ glm::scale(glm::mat4(1.0), glm::vec3(1.0f)) },
	planeRenderModel{ glm::scale(glm::mat4(1.0), glm::vec3(1.0f)) }
{
	planeModel = glm::translate(planeModel, glm::vec3(110.0f, 25.0f, 0.0f));
	planeModel = glm::translate(planeModel, planeMovement);
	planeModel = glm::rotate(planeModel, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	planeRenderModel = glm::translate(planeRenderModel, glm::vec3(110.0f, 25.0f, 0.0f));
	planeRenderModel = glm::translate(planeRenderModel, planeMovement);
	planeRenderModel = glm::rotate(planeRenderModel, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	
	//planeModel = glm::rotate(planeModel, glm::radians(tiltDeg), glm::vec3(1, 0, 0));
}

void Plane::processPlaneInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotationDeg -= 0.001f;
		if (rotationDeg < -360.0f)
			rotationDeg = 0.0f;
		turnDeg -= 0.001f;
		if (turnDeg < -80.0f)
			turnDeg = 80.0f;
	}
	else if (!(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)){
		turnDeg += 0.05f;
		if (turnDeg > 0.0f)
			turnDeg = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotationDeg += 0.001f;
		if (rotationDeg > 360.0f)
			rotationDeg = 0.0f;
		turnDeg += 0.001f;
		if (turnDeg > 80.0f)
			turnDeg = 80.0f;
	}
	else if (!(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)){
		turnDeg -= 0.05f;
		if (turnDeg < 0.0f)
			turnDeg = 0.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (acceleration > 5.0f) {
			tiltDeg -= 0.1f;
			stepY += 0.001;
			if (tiltDeg < -30.0f)
				tiltDeg = -30.0f;
			if (stepY > 0.3f)
				stepY = 0.3f;
		}
	}
	else {
		tiltDeg += 0.5f;
		stepY -= 0.05f;
		if (tiltDeg > 0.0f)
			tiltDeg = 0.0f;
		if (stepY < 0)
			stepY = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (acceleration > 5.0f) {
			tiltDeg -= 0.1f;
			stepY += 0.001;
			if (tiltDeg < -30.0f)
				tiltDeg = -30.0f;
			if (stepY > 0.3f)
				stepY = 0.3f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		acceleration += 0.01f;
		if (acceleration > 8.0f)
			acceleration = 8.0f;
	}
	else {
		acceleration -= 0.005f;
		if(acceleration < 0.0f)
			acceleration = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

	}
	std::cout << "Acceleration: " << acceleration << ", Rotation: " << rotationDeg << ", Turn: " << turnDeg << ", Tilt: " << tiltDeg << ", Forward : " << stepY << ", Upward : " << glm::tan(stepY) << std::endl;

	stepX = glm::cos(rotationDeg) * baseStepX * acceleration;
	stepZ = glm::sin(rotationDeg) * baseStepZ * acceleration;

	planeMovement[0] += stepX;
	planeMovement[1] += stepY;
	planeMovement[2] += stepZ;
}

glm::vec3 Plane::getTranslation() const
{
	return glm::vec3(planeRenderModel[3]);
}

glm::mat4 Plane::getRotation()
{
	glm::mat4 planeRotation = glm::mat4(1.0f);
	planeRotation = glm::rotate(planeRotation, glm::radians(tiltDeg), glm::vec3(0, 0, 1));    
	planeRotation = glm::rotate(planeRotation, -rotationDeg, glm::vec3(0, 1, 0)); 
	return planeRotation;
}

void Plane::render() {
	//glm::mat4 auxiliaryModel;

	planeRenderModel = glm::translate(planeModel, planeMovement);
	planeRenderModel = glm::rotate(planeRenderModel, glm::radians(tiltDeg), glm::vec3{ 0, 0, 1 });
	planeRenderModel = glm::rotate(planeRenderModel, -rotationDeg, glm::vec3{ 0, 1, 0 });
	planeRenderModel = glm::rotate(planeRenderModel, turnDeg, glm::vec3{ 1, 0, 0 });
	

	utils::DrawModel(MainWindow::instance().sunShader, planeRenderModel, planeObjModel);
}