#pragma once
#include "Math/MathTypes.hpp"

struct SpriteVertex
{
	Vector3f myPosition;
	float myZRotation = 0.0f;
	Vector2f myScale;
	Vector2f myPivot;
	Vector4f myColor;
};
