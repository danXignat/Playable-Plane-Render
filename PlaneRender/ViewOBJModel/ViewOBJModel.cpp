// ViewOBJModel.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Shader.h"
#include "Model.h"
#include "FlyingCube.h"
#include "Camera.h"
#include "Skybox.h"
#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

// settings
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 760;

GLuint ProjMatrixLocation, ViewMatrixLocation, WorldMatrixLocation;
Camera* pCamera = nullptr;

void Cleanup()
{
	delete pCamera;
}

void LoadLightningShader(Shader& _shader, const glm::vec3& _lightPos);
void LoadLighningTextureShaders(Shader& _shader, const glm::vec3& _lightPos);
void DrawModel(Shader& _shader, glm::mat4& _model_matrix, Model& _model);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;

// plane movements

glm::vec3 planeMovement{ 0.0f, 0.0f, 0.0f };
float rotationDeg = 90.0f;
float tiltDeg = 0.0f;
const float baseStepX = 0.0f;
const float baseStepY = 0.05f;
const float baseStepZ = 0.05f;
float direct = glm::sqrt(glm::pow(baseStepX, 2) + glm::pow(baseStepZ, 2));
float stepX = baseStepX;
float stepY = baseStepY;
float stepZ = baseStepZ;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << "X: " << planeMovement[0] << "|  Y: " << planeMovement[1] << "|  Z: " << planeMovement[2] << "\n";

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
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//planeMovement[0] += step;
		//pCamera->Set(planeMovement);
		rotationDeg += 0.1f;
		if (rotationDeg == 360.0f)
			rotationDeg = 0.0f;
		if (tiltDeg < 80.0f);
			tiltDeg += 0.1f;
		
		//rotate left
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//planeMovement[0] -= step;
		rotationDeg -= 0.1f;
		if (rotationDeg == 0.0f)
			rotationDeg = 360.0f;
		if (tiltDeg > -80.0f);
			tiltDeg -= 0.1f;
		//rotate right
	}

	stepX = glm::sin(glm::radians(rotationDeg)) * direct;
	stepZ = glm::cos(glm::radians(rotationDeg)) * direct;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		planeMovement[1] += stepY;
		//ascend
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		planeMovement[1] -= stepY;
		//descend
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		planeMovement[0] += stepX;
		planeMovement[2] += stepZ;
		//move forth
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		planeMovement[0] -= stepX;
		planeMovement[2] -= stepZ;
		//move backward
	}
}

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 7", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewInit();

	glEnable(GL_DEPTH_TEST);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//VAO, VBO
	unsigned int VBO, cubeVAO;

	// first, configure the cube's VAO (and VBO)
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Create camera
	pCamera = new Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0, 0.5, -2.0));


	
	glm::vec3 lightPos(0.0f, 4.0f, 0.0f);
	glm::vec3 cubePos(0.0f, 5.0f, 1.0f);

	wchar_t buffer[MAX_PATH];
	GetCurrentDirectoryW(MAX_PATH, buffer);

	std::wstring executablePath(buffer);
	std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string currentPath = converter.to_bytes(wscurrentPath);

	//skybox faces declaration
	std::vector<std::string> faces{
		(currentPath + "\\SkyboxImg\\RightImage.png").c_str(),
		(currentPath + "\\SkyboxImg\\LeftImage.png").c_str(),
		(currentPath + "\\SkyboxImg\\TopImage.png").c_str(),
		(currentPath + "\\SkyboxImg\\BottomImage.png").c_str(),
		(currentPath + "\\SkyboxImg\\FrontImage.png").c_str(),
		(currentPath + "\\SkyboxImg\\BackImage.png").c_str()
	};

	Shader lightingShader((currentPath + "\\Shaders\\PhongLight.vs").c_str(), (currentPath + "\\Shaders\\PhongLight.fs").c_str());
	Shader lightingWithTextureShader((currentPath + "\\Shaders\\PhongLightWithTexture.vs").c_str(), (currentPath + "\\Shaders\\PhongLightWithTexture.fs").c_str());
	Shader lampShader((currentPath + "\\Shaders\\Lamp.vs").c_str(), (currentPath + "\\Shaders\\Lamp.fs").c_str());

	std::string planeObjFileName = (currentPath + "\\Models\\Plane\\source\\sr71.obj");
	Model planeObjModel(planeObjFileName, false);
	
	std::string runawayObjFileName = (currentPath + "\\Models\\Runaway\\source\\Runway\\runaway1.obj");
	Model runawayObjModel(runawayObjFileName, false);

	std::string pistaObjFileName = (currentPath + "\\Models\\pista\\pista.obj");
	Model pistaObjModel(pistaObjFileName, false);
	
	Skybox skybox(faces);
	Shader skyboxShader((currentPath+"\\Shaders\\Skybox.vs").c_str(), (currentPath+"\\Shaders\\Skybox.fs").c_str());

	// render loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		// per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightPos.x = 2.5 * cos(glfwGetTime());
		lightPos.z = 2.5 * sin(glfwGetTime());

		cubePos.x = 10 * sin(glfwGetTime());
		cubePos.z = 10 * cos(glfwGetTime());

		LoadLightningShader(lightingShader, lightPos);

		LoadLighningTextureShaders(lightingWithTextureShader, lightPos);

		//glm::vec3 cameraOffsetRotation = glm::vec3(planeMovement[0], planeMovement[1], planeMovement[2]);

		//pCamera->Set(cameraOffsetRotation);
		
		glm::mat4 planeModel = glm::scale(glm::mat4(1.0), glm::vec3(0.1f));
		//planeModel = glm::rotate(planeModel, glm::radians(90), glm::vec3(0.0f, 1.0f, 0.0f);
		planeModel = glm::translate(planeModel, planeMovement);
		planeModel = glm::rotate(planeModel, glm::radians(90.0f), glm::vec3(0.0f, 0.1f, 0.0f));
		//planeModel = glm::rotate(planeModel, glm::radians(tiltDeg), glm::vec3(0, 0, 1));
		planeModel = glm::rotate(planeModel, glm::radians(rotationDeg), glm::vec3(0, 1, 0));
		DrawModel(lightingWithTextureShader, planeModel, planeObjModel);


		glm::mat4 pistaModel = glm::scale(glm::mat4(1.0), glm::vec3(0.001f));
		pistaModel = glm::translate(pistaModel, glm::vec3(0.0f, 0.0f, -500.0f));
		DrawModel(lightingWithTextureShader, pistaModel, pistaObjModel);

		glm::mat4 runawayModel = glm::scale(glm::mat4(1.0), glm::vec3(0.001f));
		runawayModel = glm::translate(runawayModel, glm::vec3(0.0f, -900.0f, -6000.0f));
		DrawModel(lightingWithTextureShader, runawayModel, runawayObjModel);

		// also draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", pCamera->GetProjectionMatrix());
		lampShader.setMat4("view", pCamera->GetViewMatrix());
		glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.05f)); // a smaller cube
		lampShader.setMat4("model", lightModel);

		skyboxShader.use();
		skyboxShader.setMat4("view", glm::mat4(glm::mat3(pCamera->GetViewMatrix())));
		skyboxShader.setMat4("projection", pCamera->GetProjectionMatrix());
		skybox.Draw(skyboxShader);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	Cleanup();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources
	glfwTerminate();
	return 0;
}

void LoadLightningShader(Shader& _shader, const glm::vec3& _lightPos) {
	_shader.use();
	_shader.SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
	_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	_shader.SetVec3("lightPos", _lightPos);
	_shader.SetVec3("viewPos", pCamera->GetPosition());
	_shader.setMat4("projection", pCamera->GetProjectionMatrix());
	_shader.setMat4("view", pCamera->GetViewMatrix());
}

void LoadLighningTextureShaders(Shader& _shader, const glm::vec3& _lightPos) {
	_shader.use();
	_shader.SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
	_shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	_shader.SetVec3("lightPos", _lightPos);
	_shader.SetVec3("viewPos", pCamera->GetPosition());
	_shader.setInt("texture_diffuse1", 0);
	_shader.setMat4("projection", pCamera->GetProjectionMatrix());
	_shader.setMat4("view", pCamera->GetViewMatrix());
}

void DrawModel(Shader& _shader, glm::mat4& _model_matrix, Model& _model) {
	_shader.setMat4("model", _model_matrix);
	_model.Draw(_shader);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	pCamera->Reshape(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	pCamera->MouseControl((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
	pCamera->ProcessMouseScroll((float)yOffset);
}
