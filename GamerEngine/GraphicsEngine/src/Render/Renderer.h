#pragma once
#include "RenderBuffer.h"
#include "Components/Components.hpp"
#include <vector>

#include "Light/Light.h"
#include "Sort/Sort.hpp"

class ParticleEmitter;
class Entity;

class Renderer
{
public:

	static void SetCamera(const Matrix4x4f& aViewProjection, const Matrix4x4f& aProjection)
	{
		myViewProjection = aViewProjection;
		myProjection = aProjection;
	}

	static void Render(Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransfrom);
	static void RenderSprite(ParticleEmitter* aSprite, TransformComponent& aTransfrom);
	static void RenderLight(Light* aLight);

	static std::vector<RenderBuffer>& GetModels() { return myModelsToRender; }
	static std::vector<RenderBuffer2D>& GetSprites() { return mySpritesToRender; }

	static std::vector<Light*> GetSomeLights()
	{
		std::vector<Light*> myList;

		for(size_t i = 0; i < GetLights().size(); i++)
		{
			if (GetLights()[i]->GetLightBufferData().LightType != 1)
			{
				myList.push_back(GetLights()[i]);
			}
		}
		return myList;
	}

	static std::vector<Light*>& GetLights()
	{
		CommonUtilities::MergeSortPtr(myLightToRender);
		return myLightToRender;
	}

	static void RemoveLight(Light* aLight)
	{
		for(int i = 0; i < myLightToRender.size(); i++)
		{
			if (myLightToRender[i] == aLight)
			{
				myLightToRender.erase(myLightToRender.begin() + i);
				i--;
			}
		}
	}

	static void Clear()
	{
		myModelsToRender.clear();
		mySpritesToRender.clear();
		
	}

	static CommonUtilities::Matrix4x4<float>& GetProjectionMatrix() { return myProjection; }
	static CommonUtilities::Matrix4x4<float>& GetViewMatrix() { return myViewProjection; }

	static bool IsVisable(TransformComponent& aTransform)
	{
		aTransform;
		return true;
	}

	static void SetRenderGame(bool aToggleToRenderGame);

	static void SetClearColor(Vector4f aClearColor);

	static const Vector4f GetClearColor();

	static void ResetLights()
	{
		myLightToRender.clear();
	}

private:

	inline static Matrix4x4f myViewProjection = Matrix4x4f();
	inline static Matrix4x4f myProjection = Matrix4x4f();

	inline static std::vector<RenderBuffer> myModelsToRender;
	inline static std::vector<RenderBuffer2D> mySpritesToRender;
	inline static std::vector<Light*> myLightToRender;

	inline static Vector4f myClearColor;

	inline static bool myRenderGame = false;
};
