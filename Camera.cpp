#include "Camera.h"

Camera::Camera()
{
	updateProjection();
}

glm::mat4 Camera::getMVP(const Entity& entity) const
{
	auto model = entity.getModelMatrix();
	auto view = glm::lookAt(
		this->position, //position
		this->position + this->forwardVector(),
		this->upVector() //the up vector of camera
		);
	auto mvp = projection * view * model;
	return mvp;
}

glm::mat4 Camera::getVP() const
{
	auto view = glm::lookAt(
		this->position, //position
		this->position + this->forwardVector()/*glm::vec3(0,0,0)*/, //camera is looking at the center of the world
		this->upVector() //the up vector of camera
		);
	auto vp = projection * view;
	return vp;
}

glm::mat4 Camera::getView() const
{
	auto view = glm::lookAt(
		this->position, //position
		this->position + this->forwardVector(), //look destination (1 unit in front of main camera)
		this->upVector() //the up vector of camera
		);
	return view;
}

glm::mat4 Camera::getProjection() const
{
	return projection;
}

glm::mat4 Camera::getTranslation() const
{
	return glm::translate(glm::mat4(1.f), position);
}

glm::mat4 Camera::getIdentityVP() const
{
	auto view = glm::lookAt(
		glm::vec3(0, 0, 0), //identity position is set to (0,0,0) It allows to draw stuff around camera like skybox or maybe gui
		this->forwardVector(),
		this->upVector() //the up vector of camera
		);
	auto identityVP = projection * view;
	return identityVP;
}

void Camera::updateProjection()
{
	projection = glm::perspective(glm::radians(fov), aspectRatio, nearClippingPlane, farClippingPlane);
}

void Camera::setFov(float f)
{
	fov = f;
	updateProjection();
}

void Camera::setAspectRatio(float ar)
{
	aspectRatio = ar;
	updateProjection();
}
