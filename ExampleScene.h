#pragma once
#include "Scene.h"
class ExampleScene :
	public Scene
{
public:
	using Scene::Scene; //inherit Scene's constructor

	void setup() override;
	void update(double delta_time) override;
	void draw() override;
};

