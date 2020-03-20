#include "InputHandler.h"
#include "Camera.h"
#include "Input.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

void InputHandler::rotateCamera(Camera* camera, float xDelta, float yDelta, float deltaTime)
{
	float horizontalAngle = camera_rotation_speed * deltaTime * float(xDelta),
		verticalAngle = camera_rotation_speed * deltaTime * float(yDelta);
	camera->rotate(glm::vec3(-verticalAngle, -horizontalAngle, 0));
}

void InputHandler::handleMouse(Camera* camera, Window* w, Input* input, float dt)
{
	auto mp = input->mousePosition();
	auto offset = mp - _last_mouse_pos;
	_last_mouse_pos = mp;
	rotateCamera(camera, offset.x, offset.y, dt);
}

void InputHandler::handleCamera(Camera* camera, Window* window, Input* input, float dt)
{
	glm::vec3 direction(0);
	if (input->keyPressed(GLFW_KEY_W))
	{
		direction += camera->forwardVector();
	}
	if (input->keyPressed(GLFW_KEY_S))
	{
		direction -= camera->forwardVector();
	}
	if (input->keyPressed(GLFW_KEY_A))
	{
		direction -= camera->rightVector();
	}
	if (input->keyPressed(GLFW_KEY_D))
	{
		direction += camera->rightVector();
	}
	if (input->keyPressed(GLFW_KEY_SPACE))
	{
		//camera()->position += camera()->upVector() * speed;
		direction.y += 1;
	}
	if (input->keyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		//camera()->position -= camera()->upVector() * speed;
		direction.y -= 1;
	}
	float mag = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (mag != 0) {
		direction = direction / mag;
		camera->position += (direction * camera_speed * dt);
	}

	if (input->keyPressed(GLFW_KEY_ESCAPE))
		window->close();
	if (input->keyPressed(GLFW_KEY_R))
	{
		camera->position = { 0, 0, 0 };
		camera->rotation = { 0, 0, 0 };
	}
}
