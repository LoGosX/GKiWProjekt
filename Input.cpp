#include "Input.h"
#include "Window.h"
#include "GLFW/glfw3.h"

void Input::update()
{

}

Input::Input(Window* window) : _window(window->windowHandle()), _mouse_position(.5, .5) {
	_keys.fill(false);

	
	glfwSetWindowUserPointer(window->windowHandle(), this);
	glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto self = static_cast<Input*>(glfwGetWindowUserPointer(window));
		if (action == GLFW_PRESS)
			self->_keys[key] = true;
		else if(action == GLFW_RELEASE)
			self->_keys[key] = false;
	});
	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y) {
		auto self = static_cast<Input*>(glfwGetWindowUserPointer(window));
		glm::dvec2 p(x, y);
		self->_mouse_position = p;
	});

}

bool Input::keyPressed(int keycode)
{
	return _keys[keycode];
}

bool Input::keyReleased(int keycode)
{
	return !_keys[keycode];
}

glm::vec2 Input::mousePosition()
{
	return glm::vec2(_mouse_position);
}
