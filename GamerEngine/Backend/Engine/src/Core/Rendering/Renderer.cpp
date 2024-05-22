#include "GraphicsEngine.pch.h"
#include <Core/Rendering/Renderer.h>
#include <Core/Model/Entity.h>

#include "LineRenderer.h"
#include "Scene/SceneManager.h"
#include "Components/TransfromComponent.h"
#include "Font/Font.h"
#include "Font/MSDFData.h"

void GamerEngine::Renderer::Render(GamerEngine::Entity* aEntity, ModelComponent& aModel, GamerEngine::TransformComponent& aTransform)
{

	if (!aModel.GetModel())
	{
		return;
	}

	//Transform transform = Transform();

	GamerEngine::Transform transform = aTransform.GetWorldTransform();

	auto transformedBounds = aModel.GetModel()->GetBoxBounds().Transform(transform.Translation, transform.Rotation, transform.Scale);

	/*if (!transformedBounds.IsOnFrustum(myCamera->GetFrustum()))
	{
		return;
	}*/

	auto it = std::ranges::find_if(myUpdateModels,
		[&aModel](const RenderBuffer& renderBuffer)
		{
			return renderBuffer.myModel->GetName() == aModel.GetModel()->GetName();
		});

	if (aModel.GetModel()->HasBeenRendered())
	{
		aModel.GetModel()->AddRenderedInstance(aEntity->GetID(), aTransform.GetWorldMatrix());
		return;
	}

	
	aModel.GetModel()->SetHasBeenRenderer(true);
	RenderBuffer renderBuffer = RenderBuffer{ aEntity->GetID(), aTransform.GetWorldMatrix(), aModel.GetModel() };

	LineRenderer::Get().DrawAABB3D(transformedBounds);
	myUpdateModels.push_back(renderBuffer);

}

void GamerEngine::Renderer::RenderSprite(ParticleEmitter* aSprite, GamerEngine::TransformComponent& aTransfrom)
{
	mySpritesToRender.push_back({ aTransfrom.GetWorldMatrix(), aSprite });
}

void GamerEngine::Renderer::RenderLight(Light* aLight)
{
	if (!SceneManager::Get().GetScene())
	{
		return;
	}

	SceneManager::Get().GetScene()->RenderLight(aLight);
}

void GamerEngine::Renderer::RenderString(const std::string& aString, Ref<GamerEngine::Font> aFont, const Matrix4x4f& aTransform, const Vector4f& aColor)
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
	TextVertex textVertex1;
	textVertex1.Position = aTransform * Vector4f(quadMin, 0.0f, 1.0f);
	textVertex1.Color = aColor;
	textVertex1.TexCoord = { 0.0f, 0.0f };

	// ++
	TextVertex textVertex2;
	textVertex2.Position = aTransform * Vector4f(quadMin.x, quadMax.y, 0.0f, 1.0f);
	textVertex2.Color = aColor;
	textVertex2.TexCoord = { 0.0f, 1.0f };

	// ++
	TextVertex textVertex3;
	textVertex3.Position = aTransform * Vector4f(quadMax, 0.0f, 1.0f);
	textVertex3.Color = aColor;
	textVertex3.TexCoord = { 1.0f, 1.0f };

	// ++
	TextVertex textVertex4;
	textVertex4.Position = aTransform * Vector4f(quadMax.x, quadMin.y, 0.0f, 1.0f);
	textVertex4.Color = aColor;
	textVertex4.TexCoord = { 1.0f, 0.0f };

	

	double advance = glyph->getAdvance();
	char nextCharacter = 'C';
	fontGeometry.getAdvance(advance, character, nextCharacter);

	float kerningOffset = 0.0f;
	x += fsScale * advance + kerningOffset;


	myTextToRender.push_back(textVertex1);
	myTextToRender.push_back(textVertex2);
	myTextToRender.push_back(textVertex3);
	myTextToRender.push_back(textVertex4);
}

void GamerEngine::Renderer::SetRenderGame(bool aToggleToRenderGame)
{
	myRenderGame = aToggleToRenderGame;
}

void GamerEngine::Renderer::SetClearColor(Vector4f aClearColor)
{
	myClearColor = aClearColor;
}

const Vector4f GamerEngine::Renderer::GetClearColor()
{
	return myClearColor;
}

void GamerEngine::Renderer::SwapBuffers()
{
	std::swap(myUpdateModels, myRenderModels);
	myUpdateModels.clear();
}
