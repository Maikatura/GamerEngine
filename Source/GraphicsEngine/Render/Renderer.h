#pragma once
#include "RenderBuffer.h"
#include "Components/Components.hpp"
#include "Model/ModelInstance.h"


class Renderer
{
public:

	static void SetCamera(const Matrix4x4f& aViewProjection, const Matrix4x4f& aProjection)
	{
		myViewProjection = aViewProjection;
		myProjection = aProjection;
	}

	static void Render(ModelComponent& aModel, TransformComponent& aTransfrom)
	{
		myModelsToRender.push_back(RenderBuffer{ Matrix4x4f::BuildTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale), aModel.myModel });
	}

	static std::vector<RenderBuffer> GetModels() { return myModelsToRender; }

	static void Clear()
	{
		myModelsToRender.clear();
	}

	static CommonUtilities::Matrix4x4<float> const& GetProjectionMatrix() { return myProjection; }
	static CommonUtilities::Matrix4x4<float> const& GetViewMatrix() { return myViewProjection; }


private:

	inline static Matrix4x4f myViewProjection;
	inline static Matrix4x4f myProjection;

	inline static std::vector<RenderBuffer> myModelsToRender;
};
