#pragma once
#include <glm/glm.hpp>

struct TransformComponent
{
	glm::vec3 myPosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 myRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 myScale = { 1.0f, 1.0f ,1.0f };
};