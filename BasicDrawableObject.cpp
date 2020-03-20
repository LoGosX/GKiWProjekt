#include "BasicDrawableObject.h"
#include "shaderprogram.h"
#include "Camera.h"
#include "model.h"

const int* foo(const int& a) {
	return &a;
}

void boo() {

}

void BasicDrawableObject::setShaderUniforms(const Camera& camera)
{

	shader->uniformMatrix4f("M", glm::value_ptr(entity.getModelMatrix()));
	shader->uniformMatrix4f("M", &entity.getModelMatrix()[0][0]);
	shader->uniformMatrix4f("V", &camera.getView()[0][0]);
	shader->uniformMatrix4f("P", &camera.getProjection()[0][0]);
	glm::value_ptr(camera.getModelMatrix());
	model->drawSolid(false);
	foo(1);
}
