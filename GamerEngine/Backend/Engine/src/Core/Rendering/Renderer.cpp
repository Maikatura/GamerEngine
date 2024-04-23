#include "GraphicsEngine.pch.h"
#include <Core/Rendering/Renderer.h>
#include <Core/Model/Entity.h>

#include "LineRenderer.h"
#include "Scene/SceneManager.h"
#include "Components/TransfromComponent.h"
#include "Font/Font.h"
#include "Font/MSDFData.h"

void Renderer::Render(GamerEngine::Entity* aEntity, ModelComponent& aModel, TransformComponent& aTransform)
{

	if (!aModel.GetModel())
	{
		return;
	}

	//Transform transform = Transform();

	auto transformedBounds = aModel.GetModel()->GetBoxBounds().Transform(aTransform.GetPosition(), aTransform.GetRotation(), aTransform.GetScale());

	if (!transformedBounds.IsOnFrustum(myCamera->GetFrustum()))
	{
		return;
	}

	auto it = std::ranges::find_if(myUpdateModels,
		[&aModel](const RenderBuffer& renderBuffer)
		{
			return renderBuffer.myModel->GetName() == aModel.GetModel()->GetName();
		});

	if (it != myUpdateModels.end())
	{
		aModel.GetModel()->AddRenderedInstance(aEntity->GetID(), aTransform.GetMatrix());
		return;
	}

	aModel.GetModel()->ClearInstanceData();
	RenderBuffer renderBuffer = RenderBuffer{ aEntity->GetID(), aTransform.GetMatrix(), aModel.GetModel() };

	LineRenderer::Get().DrawAABB3D(transformedBounds);
	myUpdateModels.push_back(renderBuffer);


}

void Renderer::RenderSprite(ParticleEmitter* aSprite, TransformComponent& aTransfrom)
{
	mySpritesToRender.push_back({ aTransfrom.GetMatrix(), aSprite });
}

void Renderer::RenderLight(Light* aLight)
{
	if (!SceneManager::Get().GetScene())
	{
		return;
	}

	SceneManager::Get().GetScene()->RenderLight(aLight);
}

void Renderer::RenderString(const std::string& aString, Ref<GamerEngine::Font> aFont, const Matrix4x4f& aTransform, const Vector4f& aColor)
{
	const auto& fontGeometry = aFont->GetMSDFData()->FontGeometry;
	const auto& metrics = fontGeometry.getMetrics();
	auto fontAtlas = aFont->GetAtlasTexture();

	double x = 0.0;
	double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
	double y = 0.0;

	char character = 'C';
	auto glyph = fontGeometry.getGlyph(character);

	if (!glyph)
	{
		glyph = fontGeometry.getGlyph('?');
	}

	if (!glyph)
	{
		return;
	}

	double al, ab, ar, at;
	glyph->getQuadAtlasBounds(al, ab, ar, at);

	Vector2f texCoordMin((float)al, (float)ab);
	Vector2f texCoordMax((float)ar, (float)at);


	double pl, pb, pr, pt;
	glyph->getQuadPlaneBounds(pl, pb, pr, pt);
	Vector2f quadMin((float)al, (float)ab);
	Vector2f quadMax((float)ar, (float)at);

	quadMin *= (float)fsScale, quadMin *= (float)fsScale;
	quadMin += Vector2f{ (float)x,(float)y };
	quadMax += Vector2f{ (float)x,(float)y };

	float texelWidth = 1.0f / (float)fontAtlas->GetWidth();
	float texelHeight = 1.0f / (float)fontAtlas->GetHeight();
	texCoordMin += Vector2f(texelWidth, texelHeight);
	texCoordMax += Vector2f(texelWidth, texelHeight);

	// Render
	Vector4f position1 = aTransform * Vector4f(quadMin, 0.0f, 1.0f);
	Vector2f texcord1 = { 0.0f, 0.0f };

	// ++

	Vector4f position2 = aTransform * Vector4f(quadMin.x, quadMax.y, 0.0f, 1.0f);
	Vector2f texcord2 = { 0.0f, 1.0f };

	// ++

	Vector4f position3 = aTransform * Vector4f(quadMax, 0.0f, 1.0f);
	Vector2f texcord3 = { 1.0f, 1.0f };

	// ++

	Vector4f position4 = aTransform * Vector4f(quadMax.x, quadMin.y, 0.0f, 1.0f);
	Vector2f texcord4 = { 1.0f, 0.0f };



	double advance = glyph->getAdvance();
	char nextCharacter = 'C';
	fontGeometry.getAdvance(advance, character, nextCharacter);

	float kerningOffset = 0.0f;
	x += fsScale * advance + kerningOffset;

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

void Renderer::SwapBuffers()
{
	std::swap(myUpdateModels, myRenderModels);
	myUpdateModels.clear();
}
