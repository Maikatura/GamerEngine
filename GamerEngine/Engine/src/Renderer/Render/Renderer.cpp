#include "GraphicsEngine.pch.h"
#include <Renderer/Render/Renderer.h>
#include <Renderer/Model/Entity.h>

#include "Renderer/Scene/SceneManager.h"

void Renderer::Render(Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransfrom)
{

	if (!aModel.GetModel())
	{
		return;
	}

	Transform transform = Transform();
	transform.ComposeTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale);
	aModel.GetModel()->SetTransform(transform);

	myModelsToRender.push_back(RenderBuffer{ aEntity->GetID(),aTransfrom.GetMatrix(), aModel.GetModel()});
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
