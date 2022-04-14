#pragma once
#include <glm/glm.hpp>
#include <Math/Vector3.hpp>

struct TransformComponent
{
	CommonUtilities::Vector3<float> myPosition = { 0.0f, 0.0f, 0.0f };
	CommonUtilities::Vector3<float> myRotation = { 0.0f, 0.0f, 0.0f };
	CommonUtilities::Vector3<float> myScale = { 1.0f, 1.0f ,1.0f };
};