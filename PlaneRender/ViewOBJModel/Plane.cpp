#include "Plane.h"
#include "utils.h"

Plane::Plane(const std::string& path, Camera& pCamera) :
	//planeObjModel{ path + "\\Models\\Plane\\source\\PlaneClosedChute.obj" , false },
	state{PlaneState::OnGround},
	pCamera{ pCamera },
	rootPath{path},
	tipPlaneOffset{ -16.359f, 0.027f, - 1.049f },
	cameraOffset{ 0.0f, 10.0f, 35.0f },
	sound_start{false},
	planeModel{ glm::scale(glm::mat4(1.0), glm::vec3(1.0f)) },
	planeRenderModel{ glm::scale(glm::mat4(1.0), glm::vec3(1.0f)) },
	planeMovement{ 0.0f, 0.0f, 0.0f },
	rotationDeg{ 0.0f },
	tiltDeg{ 0.0f },
	turnDeg{ 0.0f },
	stepX{ 0 },
	stepY{ 0 },
	stepZ{ 0 },
	acceleration{ 0.0f },

	planeObjModel{ path + relativePath , false },
	wreckedPlaneObjModel{ path + "\\Models\\Plane\\wreck\\PlaneWreck.obj", false },
	inAirObjModel{ path + "\\Models\\Plane\\wreck\\PlaneClosedAll.obj", false },

	ball{ path + "\\Models\\Ball\\source\\FreeStone Sphere.obj", false },
	planeBase { 110.0f, 25.0f, 0.0f },
	move{false},
	collision{false}
{
	planeModel = glm::translate(planeModel, planeBase);
	planeModel = glm::translate(planeModel, planeMovement);
	planeModel = glm::rotate(planeModel, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	planeModel = glm::rotate(planeModel, glm::radians(tiltDeg), glm::vec3(1, 0, 0));

	planeRenderModel = glm::translate(planeRenderModel, planeBase);
	planeRenderModel = glm::translate(planeRenderModel, planeMovement);
	planeRenderModel = glm::rotate(planeRenderModel, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	glm::vec4 transformedPoint = planeRenderModel * glm::vec4(tipPlaneOffset, 1.0f);
	tipPlane = glm::vec3(transformedPoint);
}

void Plane::processPlaneInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		state = PlaneState::OnGround;
		planeRenderModel = planeModel;
		collision = false;
		move = false;

		planeMovement = { 0.0f, 0.0f, 0.0f };
		rotationDeg = 0.0f;
		tiltDeg = 0.0f;
		turnDeg = 0.0f;
		stepX = 0;
		stepY = 0;
		stepZ = 0;
		acceleration = 0.0f;

		Sound::stopMusic();
		Sound::playMusic(rootPath, Sound::ForestPath);

		sound_start = false;
	}

	if (collision) {
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		rotationDeg -= 0.005f;
		if (rotationDeg < -360.0f)
			rotationDeg = 0.0f;
		std::cout << planeBase[1] + planeMovement[1] << " fata de " << planeMovement[1] + 3.f;
		if (planeBase[1] + planeMovement[1] > planeMovement[1] + 3.f) {
			turnDeg -= 0.005f;
			if (turnDeg < -glm::radians(80.0f))
				turnDeg = -glm::radians(80.0f);
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		rotationDeg += 0.005f;
		if (rotationDeg > 360.0f)
			rotationDeg = 0.0f;
		if (planeBase[1] + planeMovement[1] > planeMovement[1] + 3.f) {
			turnDeg += 0.005f;
			if (turnDeg > glm::radians(80.0f))
				turnDeg = glm::radians(80.0f);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		state = PlaneState::InAir;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (acceleration > 5.0f) {
			tiltDeg += 0.1f;
			stepY += 0.001;
			if (tiltDeg > 30.0f)
				tiltDeg = 30.0f;
			if (stepY > 0.3f)
				stepY = 0.3f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		if (acceleration > 5.0f) {
			tiltDeg -= 0.1f;
			stepY -= 0.001;
			if (tiltDeg < -30.0f)
				tiltDeg = -30.0f;
			if (stepY < -0.3f)
				stepY = -0.3f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		move = true;
		acceleration += 0.01f;
		if (acceleration > 20.0f)
			acceleration = 20.0f;
	}
	else {
		acceleration -= 0.0005f;
		if (acceleration < 0.0f)
			acceleration = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		acceleration -= 0.005f;
		if (acceleration < 0.0f)
			acceleration = 0;
	}

	else if (!(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)) {
		if (tiltDeg < 0) {
			tiltDeg += 0.5f;
			stepY -= 0.05f;
			if (tiltDeg > 0.0f)
				tiltDeg = 0.0f;
			if (stepY < 0)
				stepY = 0;
		}
		if (tiltDeg > 0) {
			tiltDeg -= 0.5f;
			stepY += 0.05f;
			if (tiltDeg < 0.0f)
				tiltDeg = 0.0f;
			if (stepY > 0)
				stepY = 0;
		}
	}

	else if (!(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)) {
		
		if (turnDeg < 0.0f) {
			//std::cout << "Increasing Turn: ";
			turnDeg += 0.005f;
			if (turnDeg > 0.0f)
				turnDeg = 0.0f;
		}
		else if (turnDeg > 0.0f) {
			//std::cout << "Decreasing Turn: ";
			turnDeg -= 0.005f;
			if (turnDeg < 0.0f)
				turnDeg = 0.0f;
		}
	}
	
	//std::cout << "Left turn bound: " << -glm::radians(80.0f) << ", " << "Right turn bound: " << glm::radians(80.0f) << std::endl;
	//std::cout << "Acceleration: " << acceleration << ", Rotation: " << rotationDeg << ", Turn: " << turnDeg << ", Tilt: " << tiltDeg << ", Forward : " << stepY << ", Upward : " << glm::tan(stepY) << "Steps:" << stepX << ":" << stepY << ":" << stepZ << std::endl;

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

void Plane::_updateCamera() {
	glm::vec3 rotatedOffset = glm::vec3(this->getRotation() * glm::vec4(cameraOffset, 1.0f));

	// Compute the camera's position and orientation
	if (pCamera.state == CameraStates::BEHIND_PLANE) {
		glm::vec3 cameraPosition = this->getTranslation() + rotatedOffset;
		pCamera.SetPosition(cameraPosition);
		pCamera.LookAt(this->getTranslation());
	}
}

glm::vec3 Plane::getTipPlaneModel() {
	return tipPlane;
}

void Plane::setState(PlaneState other_state) {
	state = other_state;
}

void Plane::render() {
	_updateCamera();

	planeRenderModel = glm::translate(planeModel, planeMovement);
	planeRenderModel = glm::rotate(planeRenderModel, -glm::radians(tiltDeg), glm::vec3{ 0, 0, 1 });
	planeRenderModel = glm::rotate(planeRenderModel, -rotationDeg, glm::vec3{ 0, 1, 0 });
	planeRenderModel = glm::rotate(planeRenderModel, -turnDeg, glm::vec3{ 1, 0, 0 });

	glm::vec4 transformedPoint = planeRenderModel * glm::vec4(tipPlaneOffset, 1.0f);
	anteriorTipPlane = tipPlane;
	tipPlane = glm::vec3(transformedPoint);

	/*std::cout << "anterior" << anteriorTipPlane.x << " " << anteriorTipPlane.y << " " << anteriorTipPlane.z << std::endl;
	std::cout << "current -> " << tipPlane.x << " " << tipPlane.y << " " << tipPlane.z << std::endl;*/
	
	switch (state)
	{
	case PlaneState::OnGround:
		utils::DrawModel(MainWindow::instance().sunShader, planeRenderModel, planeObjModel);
		break;
	case PlaneState::InAir:
		utils::DrawModel(MainWindow::instance().sunShader, planeRenderModel, inAirObjModel);
		break;
	case PlaneState::Crashed:
		utils::DrawModel(MainWindow::instance().sunShader, planeRenderModel, wreckedPlaneObjModel);
		break;
	default:
		break;
	}

	if (acceleration >= 1 && sound_start == false) {
		Sound::stopMusic();
		Sound::playMusic(rootPath, Sound::JetPath);
		sound_start = true;
	}
}

void Plane::checkCollison(KDTree& tree) {
	if (collision ==  false)
	{
		auto closestPoints{ tree.findClosestPoints(tipPlane, 3) };

		using namespace glm;

		vec3 A{ closestPoints[0] };
		vec3 B{ closestPoints[1] };
		vec3 C{ closestPoints[2] };

		auto modelA = glm::translate(glm::mat4(1.f), A);
		modelA = glm::scale(modelA, glm::vec3(5.f));
		auto modelB = glm::translate(glm::mat4(1.f), B);
		modelB = glm::scale(modelB, glm::vec3(5.f));
		auto modelC = glm::translate(glm::mat4(1.f), C);
		modelC = glm::scale(modelC, glm::vec3(5.f));

		utils::DrawModel(MainWindow::instance().sunShader, modelA, ball);
		utils::DrawModel(MainWindow::instance().sunShader, modelB, ball);
		utils::DrawModel(MainWindow::instance().sunShader, modelC, ball);

		std::cout << A.x << " " << A.y << " " << A.z << std::endl <<
					 B.x << " " << B.y << " " << B.z << std::endl <<
					 C.x << " " << C.y << " " << C.z << std::endl << std::endl;

		vec3 AB{ B - A };
		vec3 AC{ C - A };

		vec3 NORM{ normalize(cross(AB, AC)) };

		float D{ -dot(NORM, A) };
		
		float lhs =
			NORM.x * tipPlane.x +
			NORM.y * tipPlane.y +
			NORM.z * tipPlane.z + D;

		float distStart = glm::dot(NORM, anteriorTipPlane) + D;
		float distEnd = glm::dot(NORM, tipPlane) + D;

		bool isAbove = (tipPlane.y < A.y || tipPlane.y < B.y || tipPlane.y < C.y);
		if (distStart * distEnd < 0.0f && isAbove) {
			move = false;
			collision = true;
			state = PlaneState::Crashed;

			Sound::stopMusic();
			Sound::playMusic(rootPath, Sound::ExplosionPath);
		}

		/*const float epsilon = 1.f;
		if (std::fabs(lhs) < epsilon)  {
			std::cout << "Collided";
			
			move = false;
			collision = true;
		}*/
	}
}


Model& Plane::getObject()
{
	return planeObjModel;
}

glm::mat4& Plane::getModel()
{
	return planeRenderModel;
}

glm::vec3 Plane::getPosition()
{
	return planeMovement + glm::vec3(110.0f, 25.0f, 0.0f);
}
