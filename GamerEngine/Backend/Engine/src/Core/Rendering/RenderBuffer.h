#pragma once
#include <Math/MathTypes.hpp>
#include "Core/Model/Model.h"

class ParticleEmitter;


namespace GamerEngine
{
	class RenderBuffer
	{
	public:
		uint32_t myId;
		Matrix4x4f myTransform;
		Ref<GamerEngine::Model> myModel;
	};

	class RenderBuffer2D
	{
	public:
		bool operator<(RenderBuffer2D& aRenderBuffer2D);

		Matrix4x4f myTransform;
		ParticleEmitter* mySprite;
	};

	struct TextVertex
	{
		Vector4f Position;
		Vector4f Color;
		Vector2f TexCoord;

		int EntityID;
	};
}

