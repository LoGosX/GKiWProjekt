#pragma once
#include "Scene.h"

class DrawableObject;
class InputHandler;

class RaymarchScene :
	public Scene
{
public:
	using Scene::Scene;

	void setup() override;
	void update() override;
	void draw() override;

private:
	DrawableObject* raymarch_sphere;
	InputHandler* input_h;
};

