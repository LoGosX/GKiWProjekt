#include "Window.h"

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
#include "Input.h"

bool Window::create(int width, int height, const char* name)
{
	_width = width;
	_height = height;
	_window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!_window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		return false;
	}

	

	makeContextCurrent();
	
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void Window::makeContextCurrent() {
	glfwMakeContextCurrent(_window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(0); 
	//glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
}

bool Window::isOpen() const
{
	return !glfwWindowShouldClose(_window);
}

bool Window::pollEvents(Event& ev)
{
	if (_window_events.empty())
		return false;
	ev = std::move(_window_events.front());
	_window_events.pop();
}

void Window::close()
{
	glfwSetWindowShouldClose(_window, 1);
}

void Window::destroy()
{
	glfwDestroyWindow(_window); //Usuñ kontekst OpenGL i okno
}

void Window::clearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffers()
{
	glfwSwapBuffers(_window);
	glfwPollEvents();
}

int Window::width() const
{
	return _width;
}

int Window::height() const
{
	return _height;
}

void Window::centerCursor()
{
	glfwSetCursorPos(_window, _width / 2.f, _height / 2.f);
}

GLFWwindow* Window::windowHandle()
{
	return _window;
}
