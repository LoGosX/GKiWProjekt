#include "RenderSystem.h"

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "Window.h"
#include "Camera.h"

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

bool RenderSystem::initialize()
{
	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		return false;
	}

	_window = new Window;
	if (!_window->create(1200, 1200, "OpenGL")) {
		fprintf(stderr, "Nie mo¿na stworzyæ okna.\n");
		return false;
	}

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		return false;
	}

	ShaderProgram::initShaders();
	
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LESS);

	_camera = new Camera;

	return true;
}

void RenderSystem::destroy()
{
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
}


Window* RenderSystem::window() const
{
	return _window;
}

Camera* RenderSystem::camera()
{
	return _camera;
}
