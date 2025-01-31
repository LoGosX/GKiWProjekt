#pragma once
#include "glm/vec2.hpp"

class Camera;
class Window;
class Input;

class InputHandler
{
public:

	virtual void handleMouse(Camera* camera, Window* w, Input* input, float dt);

	virtual void handleCamera(Camera* camera, Window* window, Input* input, float dt);

	float camera_speed{ 10 };
	float camera_rotation_speed{ 0.5 };

private:
	bool _first_camera_move{ true };
	void rotateCamera(Camera* camera, float xDelta, float yDelta, float deltaTime);
	glm::vec2 _last_mouse_pos;
};

