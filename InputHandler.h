#pragma once
#include "glm/vec2.hpp"

class Camera;
class Window;
class Input;

class InputHandler
{
public:

	void handleMouse(Camera* camera, Window* w, Input* input, float dt);

	void handleCamera(Camera* camera, Window* window, Input* input, float dt);

	float camera_speed{ 10 };
	float camera_rotation_speed{ 1 };

private:
	void rotateCamera(Camera* camera, float xDelta, float yDelta, float deltaTime);
	glm::vec2 _last_mouse_pos;
};

