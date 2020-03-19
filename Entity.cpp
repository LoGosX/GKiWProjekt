#include "Entity.h"



Entity::Entity()
{
	position = rotation = { 0,0,0 };
	scale = { 1,1,1 };
}


Entity::~Entity()
{
}

glm::mat4 Entity::getModelMatrix() const
{
	glm::mat4 rotationMat(1.0f);
	rotationMat = glm::rotate(rotationMat, rotation.x, glm::vec3(1, 0, 0));
	rotationMat = glm::rotate(rotationMat, rotation.y, glm::vec3(0, 1, 0));
	rotationMat = glm::rotate(rotationMat, rotation.z, glm::vec3(0, 0, 1));

	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), scale);

	glm::mat4 transloationMatrix = glm::translate(glm::mat4(1.f), position);

	glm::mat4 modelMat = transloationMatrix * rotationMat * scaleMat;

	return modelMat;
}

void Entity::move(glm::vec3 vector)
{
	position += vector;
}

void Entity::rotate(glm::vec3 rotation)
{
	this->rotation += rotation;
}

glm::vec3 Entity::upVector() const
{
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
	//up vector
	return glm::cross(rightVector(), forwardVector());
}


glm::vec3 Entity::forwardVector() const
{
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
	return glm::vec3(
		cos(/*verticalAngle*/rotation.x) * sin(/*horizontalAngle*/rotation.y),
		sin(/*verticalAngle*/rotation.x),
		cos(/*verticalAngle*/rotation.x) * cos(/*horizontalAngle*/rotation.y)
		);
}

glm::vec3 Entity::rightVector() const
{
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
	//right vector
	return glm::vec3(
		sin(/*horizontalAngle*/rotation.y - 3.14f / 2.0f),
		0,
		cos(/*horizontalAngle*/rotation.y - 3.14f / 2.0f)
		);
}