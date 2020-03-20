#include "RaymarchScene.h"
#include "Raymarching2DSphereDrawableObject.h"
#include "shaderprogram.h"
#include "allmodels.h"
#include "InputHandler.h"
#include "Camera.h"

void RaymarchScene::setup()
{
	input_h = new InputHandler;
	raymarch_sphere = new Raymarching2DSphereDrawableObject(window());
	raymarch_sphere->shader = ShaderProgram::raymarch2D;
	raymarch_sphere->model = &Models::plane;
}
void RaymarchScene::update(double dt)
{
	input_h->handleCamera(camera(), window(), input(), dt);
	input_h->handleMouse(camera(), window(), input(), dt);
	fprintf(stdout, "Position(%.3f %.3f %.3f), Rotation(%.3f %.3f %.3f)\n", camera()->position.x, camera()->position.y, camera()->position.z, camera()->rotation.x, camera()->rotation.y, camera()->rotation.z);
}

void RaymarchScene::draw()
{
	raymarch_sphere->shader->use();
	raymarch_sphere->setShaderUniforms(*camera());
	raymarch_sphere->model->drawSolid(false);
}
