#include "Scene.h"
#include "Window.h"
#include "Input.h"

Scene::Scene(Camera* c, Window* w, Input* i) : _camera(c), _window(w), _input(i)
{

}

void Scene::setTime(float time)
{
	_t = time;
}

void Scene::setDeltaTime(float dt)
{
	_dt = dt;
}

float Scene::getTime() {
	return _t;
}

float Scene::deltaTime() {
	return _dt;
}

Window* Scene::window()
{
	return _window;
}

Input* Scene::input()
{
	return _input;
}

Camera* Scene::camera()
{
	return _camera;
}
