#pragma once
#include "RenderBuffer.h"
#include "Components/Components.hpp"
#include <vector>

#include "Components/ModelComponent.h"
#include "Components/TransfromComponent.h"
#include "Light/Light.h"
#include "Sort/Sort.hpp"


class ParticleEmitter;


namespace GamerEngine
{
	class Font;
	class Entity;

	class Renderer
	{
	public:

		static void SetCamera(GamerEngine::CameraComponent* aCamera, GamerEngine::TransformComponent& aTransform, const Matrix4x4f& aViewProjection, const Matrix4x4f& aProjection)
		{
			myCamera = aCamera;
			myCameraTransform = &aTransform;
			myViewProjection = aViewProjection;
			myProjection = aProjection;
		}

		static GamerEngine::CameraComponent* GetCamera()
		{
			if (myCamera)
			{
				return myCamera;
			}

			return nullptr;
		}

		static GamerEngine::TransformComponent* GetCameraTransform()
		{
			return myCameraTransform;
		}



		static void Render(GamerEngine::Entity* aEntity, ModelComponent& aModel, GamerEngine::TransformComponent& aTransfrom);
		static void RenderSprite(ParticleEmitter* aSprite, GamerEngine::TransformComponent& aTransfrom);
		static void RenderLight(Light* aLight);
		static void RenderString(const std::string& aString, Ref<GamerEngine::Font> aFont, const Matrix4x4f& aTransform, const Vector4f& aColor);




		static std::vector<RenderBuffer>& GetModels() { return myRenderModels; }
		static std::vector<RenderBuffer2D>& GetSprites() { return mySpritesToRender; }
		static std::vector<TextVertex>& GetText() { return myTextToRender; }


		static void Clear()
		{
			myRenderModels.clear();
			mySpritesToRender.clear();
			myTextToRender.clear();
		}


		static CommonUtilities::Matrix4x4<float>& GetProjectionMatrix() { return myProjection; }
		static CommonUtilities::Matrix4x4<float>& GetViewMatrix() { return myViewProjection; }


		static bool IsVisable(GamerEngine::TransformComponent& aTransform)
		{
			aTransform;
			return true;
		}

		static void SetRenderGame(bool aToggleToRenderGame);

		static void SetClearColor(Vector4f aClearColor);

		static const Vector4f GetClearColor();
		static void SwapBuffers();

	private:

		inline static Matrix4x4f myViewProjection = Matrix4x4f();
		inline static Matrix4x4f myProjection = Matrix4x4f();

		inline static std::vector<RenderBuffer> myUpdateModels;
		inline static std::vector<RenderBuffer> myRenderModels;
		inline static std::vector<RenderBuffer2D> mySpritesToRender;
		inline static std::vector<TextVertex> myTextToRender;

		inline static Vector4f myClearColor;

		inline static bool myRenderGame = false;
		inline static TransformComponent* myCameraTransform;
		inline static GamerEngine::CameraComponent* myCamera;



	};
}
