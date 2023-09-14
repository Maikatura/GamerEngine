#include "GraphicsEngine.pch.h"
#include <Renderer/Render/Renderer.h>
#include <Renderer/Model/Entity.h>

#include "Renderer/Scene/SceneManager.h"


inline Matrix4x4f ComposeFromTRS(const Vector3f& aTranslation, const CommonUtilities::Quaternion<float>& aRotationQuat, const Vector3f& aScale)
{
	return Matrix4x4f::CreateScale(aScale)
		* aRotationQuat.GetRotationMatrix4x4()
		* Matrix4x4f::CreateTranslation(aTranslation);
}

void Renderer::Render(Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransfrom)
{

	if (!aModel.GetModel())
	{
		return;
	}

	//Transform transform = Transform();


	auto test = ComposeFromTRS(aTransfrom.Translation, CommonUtilities::Quaternionf::FromEulers(aTransfrom.Rotation), aTransfrom.Scale);
	//auto test2 = ComposeFromTRS(aTransfrom.Translation, test.GetQuat(), aTransfrom.Scale);
	//transform.ComposeTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale);
	aModel.GetModel()->SetTransform(test);

	myModelsToRender.push_back(RenderBuffer{ aEntity->GetID(), test, aModel.GetModel()});
}

void Renderer::RenderSprite(ParticleEmitter* aSprite, TransformComponent& aTransfrom)
{
	mySpritesToRender.push_back({ Matrix4x4f::BuildTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale), aSprite });
}

void Renderer::RenderLight(Light* aLight)
{
	if(!SceneManager::GetScene())
	{
		return;
	}

	SceneManager::GetScene()->RenderLight(aLight);
}

void Renderer::SetRenderGame(bool aToggleToRenderGame)
{
	myRenderGame = aToggleToRenderGame;
}

void Renderer::SetClearColor(Vector4f aClearColor)
{
	myClearColor = aClearColor;
}

const Vector4f Renderer::GetClearColor()
{
	return myClearColor;
}
