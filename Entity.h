#pragma once
#include <glm\glm.hpp>
#include "glm\ext.hpp"

class Entity
{
public:
	Entity();
	~Entity();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 getModelMatrix() const;
	void move(glm::vec3 vector);
	void rotate(glm::vec3 rotation);

	glm::vec3 upVector() const;
	glm::vec3 forwardVector() const;
	glm::vec3 rightVector() const;
};