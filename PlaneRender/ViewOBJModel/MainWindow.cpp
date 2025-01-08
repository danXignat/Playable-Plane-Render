#include "MainWindow.h"

MainWindow::MainWindow() :
	window{ _initWindow() },
	pCamera{ std::make_unique<Camera>(SCR_WIDTH, SCR_HEIGHT, glm::vec3(110.0f, 30.0f, 35.0f)) },
	currentPath{ _initCurrPath() },
	lightingShader{ (currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str() },
	sunShader{ (currentPath + "\\Shaders\\PhongLightWithTexture.vs").c_str(), (currentPath + "\\Shaders\\PhongLightWithTexture.fs").c_str() },
	lampShader{ (currentPath + "\\Shaders\\Lamp.vs").c_str(), (currentPath + "\\Shaders\\Lamp.fs").c_str() }
{
	_initIcon();
	_initBuffer();
}

std::unique_ptr<MainWindow> MainWindow::m_instance{ new MainWindow() };

MainWindow& MainWindow::instance() {
	return *m_instance;
}

void MainWindow::run() {
	glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
	glm::vec3 cubePos(0.0f, 11.0f, 1.0f);

	Mountain mountain{ currentPath };
	Plane plane{ currentPath, *pCamera };

	std::string airPortObjFileName = (currentPath + "\\Models\\Airportnew\\ImageToStl.com_airport.obj");
	Model airPortObjModel{ airPortObjFileName, false };

	std::string towerObjFileName = (currentPath + "\\Models\\Ball\\source\\FreeStone Sphere.obj");
	Model towerObjModel{ towerObjFileName, false };

	Sun sun(2500.0f, 2100.0f); // Orbit radius, elevation range
	float currentTime = sun.getRealTimeInHours()+8; //get time
	Skybox skybox{ currentPath, *pCamera,currentTime };
	sun.initialize(currentPath+"\\Models\\Sun\\sun.obj");

	auto pos = pCamera->GetPosition();

	while (!glfwWindowShouldClose(window)) {
		_processInput();
		plane.processPlaneInput(window);

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

		currentTime = sun.getRealTimeInHours()+8; // Get current time
		// Render the sun
		sun.render(sunShader, currentTime, pCamera->GetPosition());

		glm::mat4 airportModel = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		airportModel = glm::rotate(airportModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
		utils::DrawModel(sunShader, airportModel, airPortObjModel);

		mountain.render();
		plane.render();
		plane.checkCollison(mountain.kdTree);
		sun.render(sunShader, currentTime, pCamera->GetPosition());
		skybox.render();

		/*glm::mat4 towerModel = glm::scale(glm::mat4(1.0), glm::vec3(0.5f));
		towerModel = glm::rotate(towerModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
		utils::DrawModel(lightingWithTextureShader, towerModel, towerObjModel);*/

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