#pragma once

#include <memory>
#include <string>
#include <stb_image.h>

#include "Camera.h"
#include "Includes.h"
#include "settings.h"
#include "Shader.h"
#include "utils.h"
#include "Skybox.h"
#include "Model.h"
#include "Plane.h"
#include "Sun.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

class MainWindow {
public:
	static MainWindow& instance();

	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
	~MainWindow() = default;

	void run();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yOffset);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	MainWindow();

	GLFWwindow* _initWindow();
	std::string _initCurrPath();
	void _initBuffer();
	void _deallocate();
	void _initIcon();

public:
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

	GLFWwindow* window;
	GLuint ProjMatrixLocation, ViewMatrixLocation, WorldMatrixLocation;
	std::unique_ptr<Camera> pCamera;

	unsigned int VBO, cubeVAO, lightVAO;

	std::string currentPath;

	Shader lightingShader;
	Shader lampShader;
	Shader sunShader;

private:
	static std::unique_ptr<MainWindow> m_instance;

};