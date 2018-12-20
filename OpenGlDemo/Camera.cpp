#include "Camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera() {
	position = { 0, 0, 0 };
	target = { 0, 0, 0 };
	fov = 60;
	nearClippingPlane = 0.01f;
	farClipPlane = 1000;
}

glm::mat4 Camera::viewMatrix() {
	return glm::lookAt(
		position, target, {0, 1, 0}
	);
}

glm::mat4 Camera::projMatrix(float displayRatio) {
	return glm::perspective(glm::radians(fov), displayRatio, nearClippingPlane, farClipPlane);
}
