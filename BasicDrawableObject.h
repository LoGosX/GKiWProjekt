#pragma once
#include "DrawableObject.h"
class BasicDrawableObject :
	public DrawableObject
{
	void draw(const Camera& camera) override;
};

