#include "ExampleScene.h"
#include "Window.h"
#include "Input.h"
#include "allmodels.h"
#include "shaderprogram.h"
#include "Camera.h"
#include <cmath>
#include "BasicDrawableObject.h"


void rotateCamera(Camera* camera, float xDelta, float yDelta, float deltaTime)
{
	const float mouseSpeed = 1.0f;
	float horizontalAngle = mouseSpeed * deltaTime * float(xDelta),
		verticalAngle = mouseSpeed * deltaTime * float(yDelta);
	camera->rotate(glm::vec3(-verticalAngle, -horizontalAngle, 0));
}

void handleMouse(Camera* camera, Window* w, Input* input, bool centerMouse, float dt)
{
	static glm::vec2 last_mouse_pos = input->mousePosition();
	auto mp = input->mousePosition();
	auto offset = mp - last_mouse_pos;
	last_mouse_pos = mp;
	rotateCamera(camera, offset.x, offset.y, dt);
}


void handleCamera(Camera* camera, Window* window, Input* input, bool superSpeed, float dt)
{
	float speed = 10;
	glm::vec3 direction(0);
	if (input->keyPressed(GLFW_KEY_W))
	{
		direction += camera->forwardVector();
	}
	if (input->keyPressed(GLFW_KEY_S))
	{
		direction -= camera->forwardVector();
	}
	if (input->keyPressed(GLFW_KEY_A))
	{
		direction -= camera->rightVector();
	}
	if (input->keyPressed(GLFW_KEY_D))
	{
		direction += camera->rightVector();
	}
	if (input->keyPressed(GLFW_KEY_SPACE))
	{
		//camera()->position += camera()->upVector() * speed;
		direction.y += 1;
	}
	if (input->keyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		//camera()->position -= camera()->upVector() * speed;
		direction.y -= 1;
	}
	float mag = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
	if (mag != 0) {
		direction = direction / mag;
		camera->position += (direction * speed * dt);
	}
}

std::vector<DrawableObject *> drawable_objects(8);
float t = 0;

void centerCamera(Camera* camera) {
	camera->position = { 0, 0, 0 };
	camera->rotation = { 0, 0, 0 };
}
void ExampleScene::setup()
{
	centerCamera(camera());
	for (int i = 0; i < drawable_objects.size() / 2; i++) {
		drawable_objects[i] = new BasicDrawableObject;
		drawable_objects[i]->shader = ShaderProgram::spLambert;
		drawable_objects[i]->entity.position = camera()->position + camera()->forwardVector() * 5.f + 5 * (1.5f - i) * camera()->rightVector();
	}
	for (int i = drawable_objects.size() / 2; i < drawable_objects.size(); i++) {
		drawable_objects[i] = new BasicDrawableObject;
		drawable_objects[i]->shader = ShaderProgram::spConstant;
		drawable_objects[i]->entity.position = camera()->position + camera()->forwardVector() * 5.f + 5 * (1.5f - i + drawable_objects.size() / 2) * camera()->rightVector() + 5.f * camera()->upVector();
	}
	for (int i = 0; i < 2; i++) {
		drawable_objects[4 * i + 0]->model = &Models::teapot;
		drawable_objects[4 * i + 1]->model = &Models::torus;
		drawable_objects[4 * i + 2]->model = &Models::sphere;
		drawable_objects[4 * i + 3]->model = &Models::cube;
	}
}

void ExampleScene::update(double delta_time)
{
	//teapot_entity.position = { -5, 0, 0 };
	handleCamera(camera(), window(), input(), true, delta_time);
	handleMouse(camera(), window(), input(), false, delta_time);

	t += delta_time;
	//teapot_entity.rotate({ 0, 5000 * delta_time , 0 });

	if (input()->keyPressed(GLFW_KEY_ESCAPE))
		window()->close();
	if (input()->keyPressed(GLFW_KEY_R))
	{
		camera()->position = { 0, 0, 0 };
		camera()->rotation = { 0, 0, 0 };
	}
	fprintf(stdout, "Position(%.3f %.3f %.3f), Rotation(%.3f %.3f %.3f)\n", camera()->position.x, camera()->position.y, camera()->position.z, camera()->rotation.x, camera()->rotation.y, camera()->rotation.z);
}

void ExampleScene::draw() {
	for (int i = 0; i < drawable_objects.size(); i++) {
		drawable_objects[i]->shader->use();
		drawable_objects[i]->draw(*camera());
	}
}