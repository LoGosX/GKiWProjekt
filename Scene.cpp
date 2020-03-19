#include "Scene.h"
#include "Window.h"
#include "Input.h"

Scene::Scene(Camera* c, Window* w, Input* i) : _camera(c), _window(w), _input(i)
{

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
