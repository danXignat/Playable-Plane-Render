#include "MainWindow.h"

MainWindow::MainWindow() :
	window{ _initWindow() },
	pCamera{ std::make_unique<Camera>(SCR_WIDTH, SCR_HEIGHT, glm::vec3(110.0f, 30.0f, 35.0f)) },
	currentPath{ _initCurrPath() },
	lightingShader{ (currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str() },
	sunShader{ (currentPath + "\\Shaders\\PhongLightWithTexture.vs").c_str(), (currentPath + "\\Shaders\\PhongLightWithTexture.fs").c_str() },
	lampShader{ (currentPath + "\\Shaders\\Lamp.vs").c_str(), (currentPath + "\\Shaders\\Lamp.fs").c_str() }/*,
	mainShader{ (currentPath + "\\Shaders\\main.vs").c_str(), (currentPath + "\\Shaders\\main.fs").c_str() }*/
{
	_initIcon();
	_initBuffer();
}

std::unique_ptr<MainWindow> MainWindow::m_instance{ new MainWindow() };

MainWindow& MainWindow::instance() {
	return *m_instance;
}

void MainWindow::run() {
	//Clouds::setRandom();

	glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
	glm::vec3 cubePos(0.0f, 11.0f, 1.0f);

	Mountain mountain{ currentPath };
	Relief relief{ currentPath };
	Plane plane{ currentPath, *pCamera };

	//std::string cloudObjFileName = (currentPath + "\\Models\\Clouds2\\ImageToStl.com_volume_cloud.obj");
	//Model cloudObjModel{ cloudObjFileName, false };

	std::string airPortObjFileName = (currentPath + "\\Models\\Airportnew\\ImageToStl.com_airport.obj");
	Model airPortObjModel{ airPortObjFileName, false };

	Sun sun(2500.0f, 6000.0f); // Orbit radius, elevation range
	float currentTime = sun.getRealTimeInHours()-2; //get time
	Skybox skybox{ currentPath, *pCamera,currentTime };
	sun.initialize(currentPath+"\\Models\\Sun\\sun.obj");

	std::vector<glm::vec3> clouds{};

	std::string mountainauxObjFileName = (currentPath + "\\Models\\Mountain3\\ImageToStl.com_mountain_low_poly_for_distant_mountains.obj");
	Model mountainObjModel{ mountainauxObjFileName, false };

	std::string controlObjFileName = (currentPath + "\\Models\\Control\\ImageToStl.com_air_traffic_control_tower.obj");
	Model controlObjModel{ controlObjFileName, false };

	std::string HangarObjFileName = (currentPath + "\\Models\\Hangar\\ImageToStl.com_cloth_hangar.obj");
	Model hangarObjModel{ HangarObjFileName, false };

	//Shadow shadow {(currentPath + "\\Shaders\\Shadow.vs").c_str(), (currentPath + "\\Shaders\\Shadow.fs").c_str() };

	auto pos = pCamera->GetPosition();
	while (!glfwWindowShouldClose(window)) {
		plane.processPlaneInput(window);
		_processInput();

		glm::mat4 airportModel = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		airportModel = glm::rotate(airportModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
		glm::mat4 reliefModel{ glm::translate(glm::mat4(1.f), glm::vec3(0.f, -2200.f, 0.f)) };
		
		/*glm::mat4 mountain1Model = glm::scale(glm::mat4(1.0), glm::vec3(3.f));
		mountain1Model = glm::rotate(mountain1Model, glm::radians(180.0f), glm::vec3(0, 1, 0));
		mountain1Model = glm::translate(mountain1Model, glm::vec3(-600.0f, -20.0f, 550.0f));*/

		glm::mat4 mountain3Model = glm::scale(glm::mat4(1.0), glm::vec3(1.5f));
		mountain3Model = glm::translate(mountain3Model, glm::vec3(200, -100, 900));
		mountain3Model = glm::rotate(mountain3Model, glm::radians(-90.0f), glm::vec3(0, 1, 0));


		glm::mat4 controlModel = glm::scale(glm::mat4(1.0), glm::vec3(3.0f));
		controlModel = glm::translate(controlModel, glm::vec3(10.0f, 0.0f, -100.0f));
		
		glm::mat4 hangarModel = glm::scale(glm::mat4(1.0), glm::vec3(2.0f));
		hangarModel = glm::translate(hangarModel, glm::vec3(55.0f, 10.0f, 0.0f));

		//std::cout << pCamera->GetPosition().x << " " << pCamera->GetPosition().y << " " << pCamera->GetPosition().z << std::endl;

		/*std::vector<Model> objects;

		objects.push_back(plane.getObject());
		objects.push_back(airPortObjModel);
		objects.push_back(mountain1ObjModel);

		std::vector<glm::mat4> models;

		models.push_back(plane.getModel());
		models.push_back(airportModel);
		models.push_back(mountain1Model);

		glm::vec3 lightPos = sun.getPosition() + glm::vec3(5.f, 0.f, 5.f);

		glm::vec3 lightTarget = plane.getPosition();

		glm::vec3 lightDir = glm::normalize(lightPos - lightTarget);

		glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, 0.1f, 300.0f);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		if (glm::dot(lightDir, up) > 0.99f) {
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		glm::mat4 lightView = glm::lookAt(lightPos, lightTarget, up);

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		shadow.RenderShadowMap(mainShader, lightSpaceMatrix, objects, models, lightPos, *pCamera);*/

		/*glfwSwapBuffers(window);
		glfwPollEvents();*/

		if (pos != pCamera->GetPosition()) {
			pos = pCamera->GetPosition();
		}

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//lightPos.x = 2.5 * cos(glfwGetTime());
		//lightPos.z = 2.5 * sin(glfwGetTime());

		//cubePos.x = 10 * sin(glfwGetTime());
		//cubePos.z = 10 * cos(glfwGetTime());

		//utils::LoadLightningShader(*pCamera, lightingShader, lightPos);
		utils::LoadLighningTextureShaders(*pCamera, sunShader, lightPos);

		currentTime = sun.getRealTimeInHours()-2; // Get current time
		
		sun.render(sunShader, currentTime, pCamera->GetPosition());

		utils::DrawModel(sunShader, airportModel, airPortObjModel);
		
		mountain.render();

		/*for (int i = 0; i < Clouds::NO_CLOUDS; i++) {
			glm::mat4 location{ 1.0f };
			location = glm::translate(location, clouds[i]);
			utils::DrawModel(sunShader, location, cloudObjModel);
		}*/

		utils::DrawModel(sunShader, mountain3Model, mountainObjModel);
		utils::DrawModel(sunShader, controlModel, controlObjModel);
		utils::DrawModel(sunShader, hangarModel, hangarObjModel);

		relief.render();
		
		if (mountain.inArea(plane.getTipPlaneModel())) {
			plane.checkCollison(mountain.kdTree);
		}
		else {
			plane.checkCollison(relief.kdTree);
		}
		plane.render();

		sun.render(sunShader, currentTime, pCamera->GetPosition());
		skybox.render();

		// also draw the lamp object
		//lampShader.use();
		//lampShader.setMat4("projection", pCamera->GetProjectionMatrix());
		//lampShader.setMat4("view", pCamera->GetViewMatrix());
		//glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
		//lightModel = glm::scale(lightModel, glm::vec3(0.05f)); // a smaller cube
		//lampShader.setMat4("model", lightModel);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	_deallocate();
}

void MainWindow::_processInput() {
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		pCamera->state = CameraStates::SPECTATOR;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		pCamera->state = CameraStates::BEHIND_PLANE;
	}
}

std::string MainWindow::getCurrentPath()
{
	return currentPath;
}

void MainWindow::_deallocate() {
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}

void MainWindow::_initIcon() {
	GLFWimage images[1];
	images[0].pixels = stbi_load((currentPath + ICON_PATH).c_str(), &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
}

std::string MainWindow::_initCurrPath() {
	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	std::string path{ converter.to_bytes(wscurrentPath) };

	return path;
}

GLFWwindow* MainWindow::_initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* win = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_LABEL, NULL, NULL);
	if (win == NULL) {
		glfwTerminate();
		throw std::runtime_error("error with opening window");
	}

	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetScrollCallback(win, scroll_callback);
	glfwSetKeyCallback(win, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	glEnable(GL_DEPTH_TEST);

	return win;
}

void MainWindow::_initBuffer() {
	// first, configure the cube's VAO (and VBO)
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}


void MainWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	auto& pCamera = MainWindow::instance().pCamera;

	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void MainWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto& pCamera = MainWindow::instance().pCamera;

	pCamera->MouseControl((float)xpos, (float)ypos);
}

void MainWindow::scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	auto& pCamera = MainWindow::instance().pCamera;

	pCamera->ProcessMouseScroll((float)yOffset);
}

void MainWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& pCamera = MainWindow::instance().pCamera;
	double deltaTime = MainWindow::instance().deltaTime;
	double lastFrame = MainWindow::instance().lastFrame;

	//std::cout << "Plane: X: " << planeMovement[0] << "|  Y: " << planeMovement[1] << "|  Z: " << planeMovement[2] << "\n";
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		pCamera->ProcessKeyboard(RIGHT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		pCamera->ProcessKeyboard(UP, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		pCamera->ProcessKeyboard(DOWN, (float)deltaTime);

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		pCamera->Reset(width, height);
	}

	glm::vec3 camerapos = pCamera->GetPosition();

	//std::cout << "Camera X: " << camerapos[0] << "|  Y: " << camerapos[1] << "|  Z: " << camerapos[2] << "\n";
}