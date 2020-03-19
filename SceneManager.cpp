#include "SceneManager.h"
#include "ExampleScene.h"



Scene* SceneManager::getCurrentScene() const
{
	return _scene;
}

void SceneManager::setCurrentScene(Scene* scene)
{
	_scene = scene;
}
