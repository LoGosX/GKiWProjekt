#pragma once
#include "DrawableObject.h"

class Window;

class Raymarching2DSphereDrawableObject :
	public DrawableObject
{
public:
	Raymarching2DSphereDrawableObject(Window* window);

	void setShaderUniforms(const Camera& camera) override;

private:
	Window* _window;
};

