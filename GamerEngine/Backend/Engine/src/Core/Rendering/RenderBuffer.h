#pragma once
#include "Render2D.h"
#include <Math/MathTypes.hpp>
#include "Core/Model/Model.h"

class ParticleEmitter;



class RenderBuffer
{
public:
	uint32_t myId;
	Matrix4x4f myTransform;
	Ref<ModelInstance> myModel;
};

class RenderBuffer2D
{
public:
	bool operator<(RenderBuffer2D& aRenderBuffer2D);

	Matrix4x4f myTransform;
	ParticleEmitter* mySprite;
};
