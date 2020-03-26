#include "RaymarchScene.h"
#include "Raymarching2DSphereDrawableObject.h"
#include "shaderprogram.h"
#include "allmodels.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Input.h"

float sincos(float x, float z, float c1, float c2, float c3) {
	return c1 * sin(c2 * x) * cos(c3 * z);
}

float sinxcoszDE(glm::vec3 p, float c1, float c2, float c3) {
	return p.y - sincos(p.x, p.z, c1, c2, c3);
}

glm::vec2 waveGrad(glm::vec2 p, float c1, float c2, float c3) {
	//wave's gradient at point p = (x, z)
	return {
		c1 * c2 * cos(c2 * p.x) * cos(c3 * p.y),
		-c1 * c3 * sin(c2 * p.x) * sin(c3 * p.y)
	};
}

float c1 = 5, c2 = 0.2, c3 = 0.1;
glm::vec3 closestPointToWave(glm::vec3 p) {
	auto gradient = [&p, cc1 = c1, cc2 = c2, cc3 = c3](glm::vec2 x) -> glm::vec2 {
		auto f = waveGrad(x, cc1, cc2, cc3);
		float sc = sincos(x.x, x.y, cc1, cc2, cc3);
		return {
			2 * (x.x - p.x) + 2 * (sc - p.y) * f.x,
			2 * (x.y - p.z) + 2 * (sc - p.y) * f.y,
		};
	};
	glm::vec2 solution = { p.x, p.z };
	for (int i = 0; i < 100; i++) {
		solution = solution  - gradient(solution) * 0.01f;
	}
	//fprintf(stdout, "%f, %f, %f", solution.x, solution.y);
	return { solution.x, sincos(solution.x, solution.y, c1, c2, c3), solution.y};
}

glm::vec3 spherePosition;
glm::vec3 debugSpherePosition;
glm::vec3 sphereVelocity;
float sphereRadius = 0.01;
float debugSphereRadius = 0.005;
float sphereMass = 1;
float nt = -1;
glm::vec3 grav(0, -0.1, 0);

glm::vec3 sphereColideWithWave(glm::vec3& sphere_position, glm::vec3& sphere_velocity, float sphere_radius, float dt) {
	auto point = closestPointToWave(sphere_position);
	float distance = glm::distance(sphere_position, point);
	if (distance < sphere_radius) {
		float intersection = sphere_radius - distance;
		glm::vec3 dir = glm::normalize(sphere_position - point);
		sphere_position = point + dir * sphere_radius;
		sphere_velocity -= dir * glm::dot(dir, sphere_velocity) * sphereMass * dt;
	}
	return point;
}

void updateSphereWave(float dt) {
	if (nt < 0) {
		nt = 30;
		spherePosition = { rand() / float(RAND_MAX) * 5, 10, rand() / float(RAND_MAX) * 5 };
		sphereVelocity = { 0, 0, 0 };
	}
	nt -= dt;

	auto p = sphereColideWithWave(spherePosition, sphereVelocity, sphereRadius, dt);
	glm::vec3 grav(0, -1, 0);
	if (glm::distance(p, spherePosition) == 0.f) {
		auto wg = waveGrad({ p.x, p.z }, c1, c2, c3);
		auto g = glm::vec3(wg.x, 0, wg.y);
		//glm::vec3 dir = glm::normalize(g);
		grav = glm::dot(grav, g) * g * sphereMass;
	}
	sphereVelocity += grav * dt;
	spherePosition += sphereVelocity * dt;
}

#include "Menger3.h"

bool sphereGrounded = false;
glm::vec3 sphereJumpDir = { 0,0,0 };
void updateSphereSierpinski(float dt) {
	sphereVelocity += grav * dt;
	spherePosition += sphereVelocity * dt;
	//sphereVelocity = glm::normalize(sphereVelocity) * fmin(0.1f, glm::length(sphereVelocity));

	auto pair = sierpinski3(spherePosition);
	glm::vec3 gravv = grav;
	float distance = pair.first;
	for (int i = 0; distance < sphereRadius && i < 5; i++) {
		auto point = pair.second;
		float intersection = sphereRadius - distance;
		glm::vec3 dir = glm::normalize(spherePosition - point);
		spherePosition += dir * intersection;
		sphereVelocity += dir * glm::dot(dir, -sphereVelocity) * 2.f * dt;

		pair = sierpinski3(spherePosition);
		distance = pair.first;
		sphereGrounded = true;
		sphereJumpDir = glm::vec3(0, 1, 0) * std::max(0.f, glm::dot(dir, glm::vec3(0, 1, 0)));
	}
}
void updateSphereSierpinski3(float dt) {
	if (nt < 0 && false) {
		nt = 10;
		spherePosition = { 1.f, 3.f, 1.f};
		sphereVelocity = { 0, 0, 0 };
	}
	nt -= dt;

	updateSphereSierpinski(dt);
}

void updateSphereMenger(float dt, float time) {

	sphereVelocity += grav * dt;
	spherePosition += sphereVelocity * dt;
	//sphereVelocity = glm::normalize(sphereVelocity) * fmin(0.1f, glm::length(sphereVelocity));
	float rotX = 0;
	auto pair = Menger3(spherePosition, rotX);
	glm::vec3 gravv = grav;
	float distance = pair.first;
	for (int i = 0; distance < sphereRadius && i < 5; i++) {
		auto point = pair.second;
		float intersection = sphereRadius - distance;
		glm::vec3 dir = glm::normalize(spherePosition - point);
		spherePosition += dir * intersection;
		sphereVelocity += dir * glm::dot(dir, -sphereVelocity) * 2.f * dt;

		pair = Menger3(spherePosition, rotX);
		distance = pair.first;
		sphereGrounded = true;
		sphereJumpDir = glm::vec3(0, 1, 0) * std::max(0.f, glm::dot(dir, glm::vec3(0, 1, 0)));
	}
}

void RaymarchScene::setup()
{
	srand(time(0));
	input_h = new InputHandler;
	raymarch_sphere = new Raymarching2DSphereDrawableObject(window());
	raymarch_sphere->shader = ShaderProgram::raymarch2D;
	raymarch_sphere->model = &Models::plane;
	spherePosition = glm::vec3(0, 10, 0);
	input_h->camera_speed /= 10;
}

void RaymarchScene::update()
{
	//updateSphereSierpinski(deltaTime());
	updateSphereMenger(deltaTime(), getTime());
	input_h->handleCamera(camera(), window(), input(), deltaTime());
	input_h->handleMouse(camera(), window(), input(), deltaTime());
	float speed = 0.1f;
	if (input()->keyPressed(GLFW_KEY_W)) {
		sphereVelocity += camera()->forwardVector() * speed * deltaTime();
	}if (input()->keyPressed(GLFW_KEY_S)) {
		sphereVelocity -= camera()->forwardVector() * speed * deltaTime();
	}if (input()->keyPressed(GLFW_KEY_A)) {
		sphereVelocity -= camera()->rightVector() * speed * deltaTime();
	}if (input()->keyPressed(GLFW_KEY_D)) {
		sphereVelocity += camera()->rightVector() * speed * deltaTime();
	}if (input()->keyPressed(GLFW_KEY_SPACE) && sphereGrounded) {
		sphereVelocity += sphereJumpDir * speed * 1.f;
	}if (input()->keyPressed(GLFW_KEY_LEFT_CONTROL)) {
		sphereVelocity -= glm::vec3(0, 1, 0) * speed * deltaTime();
	}if (input()->keyPressed(GLFW_KEY_R)) {
		spherePosition = { 1.f, 1.5f + sphereRadius, 0.f };;
		sphereVelocity = { 0, 0, 0 };
	}
	camera()->position = spherePosition - camera()->forwardVector() * 5.f * sphereRadius;
	//debugSpherePosition = { 0,0,0 };
	//debugSpherePosition = sierpinski3point(camera()->position);
	//spherePosition = { 0,0,0 };
	//sphereRadius = 0.01f;
	//debugSpherePosition = spherePosition;		
	//debugSpherePosition = Menger3point(camera()->position);
	//fprintf(stdout, "Position(%.3f %.3f %.3f), Rotation(%.3f %.3f %.3f)\n", camera()->position.x, camera()->position.y, camera()->position.z, camera()->rotation.x, camera()->rotation.y, camera()->rotation.z);
	sphereGrounded = false;
}

void RaymarchScene::draw()
{
	raymarch_sphere->shader->use();
	raymarch_sphere->setShaderUniforms(*camera(), getTime());
	raymarch_sphere->shader->uniformFloat("debugSphereRadius", debugSphereRadius);
	raymarch_sphere->shader->uniform3f("debugSpherePosition", debugSpherePosition.x, debugSpherePosition.y, debugSpherePosition.z);
	raymarch_sphere->shader->uniformFloat("sphereRadius", sphereRadius);
	raymarch_sphere->shader->uniform3f("spherePosition", spherePosition.x, spherePosition.y, spherePosition.z);
	raymarch_sphere->shader->uniform3f("sphereVelocity", sphereVelocity.x, sphereVelocity.y, sphereVelocity.z);
	raymarch_sphere->shader->uniform3f("waveCparams", c1, c2, c3);
	raymarch_sphere->model->drawSolid(false);
}
