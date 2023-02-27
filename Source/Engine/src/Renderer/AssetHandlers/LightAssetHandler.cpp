#include "GraphicsEngine.pch.h"
#include <Renderer/AssetHandlers/LightAssetHandler.h>

#include "Renderer/Render/Renderer.h"

LightAssetHandler::~LightAssetHandler()
{
	myLights.clear();
}

std::shared_ptr<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3f aColor, float anIntensity,
	Vector3f aRotation)
{
	myDirectionalLight = std::make_shared<DirectionalLight>();
	myDirectionalLight->Init(aColor, anIntensity);

	myDirectionalLight->SetLightPosition({0, 0, 0});
	myDirectionalLight->SetDirection(aRotation);


	myDirectionalLight->myLightData.LightType = 1;


	constexpr float nearPlane = 30.0f;
	constexpr float farPlane = 50000.0f;
	const Vector2f resolution = { 2048.0f * 4.0f, 2048.0f * 4.0f };
	const float DIVIDE = 0.4f;

	myDirectionalLight->myNearPlane = nearPlane;
	myDirectionalLight->myFarPlane = farPlane;

	//Matrix4x4f lightProjection;

	//lightProjection(1, 1) = static_cast<float>(2) / resolution.x;
	//lightProjection(2, 2) = static_cast<float>(2) / resolution.y;
	//lightProjection(3, 3) = static_cast<float>(1) / (farPlane - nearPlane);
	//lightProjection(4, 3) = static_cast<float>(nearPlane / (nearPlane - farPlane));
	//lightProjection(4, 4) = static_cast<float>(1);

	//

	//myDirectionalLight->myLightData.LightProjection = lightProjection;

	myDirectionalLight->myLightData.LightProjection = Matrix4x4f::CreateOrthographicProjection(-resolution.x * DIVIDE, resolution.x * DIVIDE, -resolution.y * DIVIDE, resolution.y * DIVIDE, nearPlane, farPlane);

	myDirectionalLight->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"DirectionalLight", static_cast<size_t>(resolution.x), static_cast<size_t>(resolution.y));
	myDirectionalLight->myLightData.CastShadows = true;

	Renderer::RenderLight(myDirectionalLight.get());

	return myDirectionalLight;
}

std::shared_ptr<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::wstring& aTextureName)
{
	myEnvironmentLight = std::make_shared<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);
	return myEnvironmentLight;
}

std::shared_ptr<PointLight> LightAssetHandler::CreatePointLight(Vector3f aColor, float anIntensity, float aRange,
	Vector3f aPosition)
{
	myLights.push_back(std::make_shared<PointLight>());
	std::shared_ptr<PointLight> result = std::dynamic_pointer_cast<PointLight>(myLights.back());

	result->Init(aColor, anIntensity);
	result->SetRange(aRange);
	result->SetLightPosition(aPosition);
	result->myLightData.LightType = 2;
	result->myLightData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 25000.0f;
	const Vector2f resolution = { 2048, 2048 };
	result->myShadowMap = std::make_unique<DepthStencil>();
	result->CreatePointLightMap({ resolution.x, resolution.y });

	result->myShadowMap->myViewport = D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<float>(resolution.x), static_cast<float>(resolution.y), 0.0f, 1.0f });


	result->myLightData.NearPlane = nearPlane;
	result->myLightData.FarPlane = aRange;
	result->myLightData.LightProjection = Matrix4x4f::CreateOrthographicProjection(-resolution.x, resolution.x, -resolution.y, resolution.y, nearPlane, farPlane);
	Renderer::RenderLight(result.get());

	return result;
}

std::shared_ptr<SpotLight> LightAssetHandler::CreateSpotLight(Vector3f aColor, float anIntensity, float aRange,
	Vector3f aPosition, float aInnerCone, float aOuterCone)
{
	myLights.push_back(std::make_shared<SpotLight>());
	std::shared_ptr<SpotLight> result = std::dynamic_pointer_cast<SpotLight>(myLights.back());

	result->Init(aColor, anIntensity);
	result->SetRange(aRange);
	result->SetInnerCone(aInnerCone);
	result->SetOuterCone(aOuterCone);
	result->SetPosition(aPosition.x, aPosition.y, aPosition.z);
	result->myLightData.LightType = 3;
	result->myLightData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 25000.0f;
	const Vector2f resolution = { 2048, 2048 };

	result->myLightData.LightProjection(1, 1) = 2.0f / resolution.x;
	result->myLightData.LightProjection(2, 2) = 2.0f / resolution.y;
	result->myLightData.LightProjection(3, 3) = 1.0f / (farPlane - nearPlane);
	result->myLightData.LightProjection(4, 3) = nearPlane / (nearPlane - farPlane);
	result->myLightData.LightProjection(4, 4) = 1.0f;

	result->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"SpotLight", static_cast<size_t>(resolution.x), static_cast<size_t>(resolution.y));

	Renderer::RenderLight(result.get());

	return result;
}
