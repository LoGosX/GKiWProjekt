#pragma once
#include "Entity.h"
class ShaderProgram;
namespace Models {
	class Model;
}
class Camera;

class DrawableObject
{
public:
	Entity entity;
	ShaderProgram* shader;
	Models::Model* model;
	virtual void setShaderUniforms(const Camera& camera) = 0;
};

