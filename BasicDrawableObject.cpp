#include "BasicDrawableObject.h"
#include "shaderprogram.h"
#include "Camera.h"
#include "model.h"

void BasicDrawableObject::setShaderUniforms(const Camera& camera)
{
	shader->uniformMatrix4f("M", &entity.getModelMatrix()[0][0]);
	shader->uniformMatrix4f("V", &camera.getView()[0][0]);
	shader->uniformMatrix4f("P", &camera.getProjection()[0][0]);
	model->drawSolid(false);
}
