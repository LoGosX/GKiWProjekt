#pragma once

class Scene;

class SceneManager
{
public:
	Scene* getCurrentScene() const;
	void setCurrentScene(Scene* scene);

private:
	Scene* _scene;
};

