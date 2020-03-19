#pragma once
#include "Entity.h"

class Camera :
	public Entity
{
public:

	
	glm::mat4 getMVP(const Entity& entity) const;
	glm::mat4 getVP() const;
	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
	glm::mat4 getTranslation() const;
	glm::mat4 getIdentityVP() const;

	void setFov(float f);
	void setAspectRatio(float ar);
private:
	friend class RenderSystem;
	Camera();

	void updateProjection();
	float fov{ 45.0f };
	float aspectRatio{ 4.0f / 3 };
	const float nearClippingPlane{ 0.1f };
	const float farClippingPlane{ 10000.0f };


	glm::mat4 projection;
};
