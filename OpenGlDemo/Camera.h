#pragma once
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 target;
	float fov;
	float nearClippingPlane;
	float farClipPlane;

	Camera();
	glm::mat4 viewMatrix();
	glm::mat4 projMatrix(float displayRatio);
};