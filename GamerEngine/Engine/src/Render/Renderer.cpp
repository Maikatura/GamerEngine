#include "GraphicsEngine.pch.h"
#include <Render/Renderer.h>
#include <Model/Entity.h>

#include "Scene/SceneManager.h"
#include "Components/TransfromComponent.h"

void Renderer::Render(Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransfrom)
{

	if (!aModel.GetModel())
	{
		return;
	}
	
	//Transform transform = Transform();


	auto test = ComposeFromTRS(aTransfrom.GetPosition(), CommonUtilities::Quaternionf::FromEulers(ToRadians(aTransfrom.GetRotation())), aTransfrom.GetScale());
	//auto test2 = ComposeFromTRS(aTransfrom.Translation, test.GetQuat(), aTransfrom.Scale);
	//transform.ComposeTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale);
	aModel.GetModel()->SetTransform(test);

	myModelsToRender.push_back(RenderBuffer{ aEntity->GetID(), test, aModel.GetModel()});
}

void Renderer::RenderSprite(ParticleEmitter* aSprite, TransformComponent& aTransfrom)
{
	mySpritesToRender.push_back({ Matrix4x4f::BuildTransform(aTransfrom.GetPosition(), aTransfrom.GetRotation(), aTransfrom.GetScale()), aSprite });
}

void Renderer::RenderLight(Light* aLight)
{
	if(!SceneManager::Get().GetScene())
	{
		return;
	}

	SceneManager::Get().GetScene()->RenderLight(aLight);
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
