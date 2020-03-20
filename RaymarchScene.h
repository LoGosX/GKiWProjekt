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
	void update(double) override;
	void draw() override;

private:
	DrawableObject* raymarch_sphere;
	InputHandler* input_h;
};

