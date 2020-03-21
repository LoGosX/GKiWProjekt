#include "ExampleScene.h"
#include "Window.h"
#include "Input.h"
#include "allmodels.h"
#include "shaderprogram.h"
#include "Camera.h"
#include <cmath>
#include "BasicDrawableObject.h"
#include "InputHandler.h"


void centerCamera(Camera* camera) {
	camera->position = { 0, 0, 0 };
	camera->rotation = { 0, 0, 0 };
}

void ExampleScene::setup()
{
	input_handler = new InputHandler;
	centerCamera(camera());
	for (int i = 0; i < 4 ; i++) {
		auto bdo = new BasicDrawableObject;
		bdo->shader = ShaderProgram::spLambert;
		bdo->entity.position = camera()->position + camera()->forwardVector() * 5.f + 5 * (1.5f - i) * camera()->rightVector();
		drawable_objects.emplace_back(bdo);
	}
	for (int i = 0; i < 4; i++) {
		auto bdo = new BasicDrawableObject;
		bdo->shader = ShaderProgram::spConstant;
		bdo->entity.position = camera()->position + camera()->forwardVector() * 5.f + 5 * (1.5f - i) * camera()->rightVector() + 5.f * camera()->upVector();
		drawable_objects.emplace_back(bdo);
	}
	for (int i = 0; i < 2; i++) {
		drawable_objects[4 * i + 0]->model = &Models::teapot;
		drawable_objects[4 * i + 1]->model = &Models::torus;
		drawable_objects[4 * i + 2]->model = &Models::sphere;
		drawable_objects[4 * i + 3]->model = &Models::cube;
	}
	wave = new BasicDrawableObject;
	wave->shader = ShaderProgram::waveShader;
	wave->model = &Models::teapot;
	wave->entity.position = { 0,0,0 };
	drawable_objects.emplace_back(wave);
}

void ExampleScene::update()
{
	//teapot_entity.position = { -5, 0, 0 };
	input_handler->handleCamera(camera(), window(), input(), deltaTime());
	input_handler->handleMouse(camera(), window(), input(), deltaTime());

	wave->entity.position.x += 0.5 * deltaTime();

	fprintf(stdout, "Position(%.3f %.3f %.3f), Rotation(%.3f %.3f %.3f)\n", camera()->position.x, camera()->position.y, camera()->position.z, camera()->rotation.x, camera()->rotation.y, camera()->rotation.z);
}

void ExampleScene::draw() {
	for (int i = 0; i < drawable_objects.size(); i++) {
		drawable_objects[i]->shader->use();
		drawable_objects[i]->setShaderUniforms(*camera(), t);
		drawable_objects[i]->model->drawSolid(false);
	}
}