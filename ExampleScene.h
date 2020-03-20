#pragma once
#include "Scene.h"
#include <vector>

class DrawableObject;
class InputHandler;

class ExampleScene :
	public Scene
{
public:
	using Scene::Scene; //inherit Scene's constructor

	void setup() override;
	void update(double delta_time) override;
	void draw() override;

private:
	std::vector<DrawableObject*> drawable_objects;
	DrawableObject* wave;
	float t = 0;
	InputHandler* input_handler;
};

