#pragma once
#include "DrawableObject.h"
class BasicDrawableObject :
	public DrawableObject
{
	void setShaderUniforms(const Camera& camera) override;
};

