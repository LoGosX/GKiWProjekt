#include "Raymarching2DSphereDrawableObject.h"
#include "Window.h"
#include "shaderprogram.h"
#include "Camera.h"

Raymarching2DSphereDrawableObject::Raymarching2DSphereDrawableObject(Window* window) {
	_window = window;
}

void Raymarching2DSphereDrawableObject::setShaderUniforms(const Camera& camera, float time)
{
	shader->uniformFloat("time", time);
	shader->uniform2f("g_resolution", _window->width(), _window->height());
	shader->uniform3f("camera_position", camera.position.x, camera.position.y, camera.position.z);
	shader->uniform3f("camera_direction", camera.forwardVector().x, camera.forwardVector().y, camera.forwardVector().z);
	shader->uniform3f("camera_up", camera.upVector().x, camera.upVector().y, camera.upVector().z);
	shader->uniform3f("camera_right", camera.rightVector().x, camera.rightVector().y, camera.rightVector().z);
}
