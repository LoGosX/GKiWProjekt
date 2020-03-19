#pragma once
#include "glm/vec2.hpp"
#include "Window.h"
#include <array>

class GLFWwindow;


class Input
{
public:
	bool keyPressed(int keycode);
	bool keyReleased(int keycode);
	glm::vec2 mousePosition();
	void update();
	Input(Window* window);
private:
	glm::dvec2 _mouse_position;
	GLFWwindow* _window;
	
	std::array<bool, 1024> _keys;
};

