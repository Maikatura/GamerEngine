#include "GraphicsEngine.pch.h"
#include <Render/Renderer.h>
#include <Model/Entity.h>

void Renderer::Render(Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransfrom)
{
	
	myModelsToRender.push_back(RenderBuffer{ aEntity->GetID(),aTransfrom.GetMatrix(), aModel.myModel });
	
}

void Renderer::RenderSprite(ParticleEmitter* aSprite, TransformComponent& aTransfrom)
{
	mySpritesToRender.push_back({ Matrix4x4f::BuildTransform(aTransfrom.Translation, aTransfrom.Rotation, aTransfrom.Scale), aSprite });
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
