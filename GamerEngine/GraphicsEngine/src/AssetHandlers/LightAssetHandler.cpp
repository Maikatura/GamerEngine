#include "GraphicsEngine.pch.h"
#include <AssetHandlers/LightAssetHandler.h>

#include "Render/Renderer.h"

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
	myDirectionalLight->myLightBufferData.LightType = 1;


	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 250000.0f;
	const Vector2f resolution = { 2048, 2048 };

	myDirectionalLight->myLightBufferData.NearPlane = nearPlane;
	myDirectionalLight->myLightBufferData.FarPlane = farPlane;

	myDirectionalLight->myLightBufferData.LightProjection(1, 1) = 2.f / resolution.x;
	myDirectionalLight->myLightBufferData.LightProjection(2, 2) = 2.f / resolution.y;
	myDirectionalLight->myLightBufferData.LightProjection(3, 3) = 1.f / (farPlane - nearPlane);
	myDirectionalLight->myLightBufferData.LightProjection(4, 3) = nearPlane / (nearPlane - farPlane);
	myDirectionalLight->myLightBufferData.LightProjection(4, 4) = 1.f;


	myDirectionalLight->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"DirectionalLight", resolution.x, resolution.y);
	myDirectionalLight->myLightBufferData.CastShadows = true;

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
	result->myLightBufferData.LightType = 2;
	result->myLightBufferData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 250000.0f;
	const Vector2f resolution = { 2048, 2048 };
	result->myShadowMap = std::make_unique<DepthStencil>();
	result->CreatePointLightMap({ resolution.x, resolution.y });

	result->myShadowMap->myViewport = D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<float>(resolution.x), static_cast<float>(resolution.y), 0.0f, 1.0f });


	result->myLightBufferData.NearPlane = nearPlane;
	result->myLightBufferData.FarPlane = aRange * 100;;


	float hFoV = 90.2f * (3.14159f / 180);
	float vFoV = 2 * atan(tan(hFoV / 2)) * ((float)resolution.y / (float)resolution.x);

	float xScale = 1 / tan(hFoV * .5f);
	float yScale = 1 / tan(vFoV * .5f);
	float Q = farPlane / (farPlane - nearPlane);

	result->myLightBufferData.LightProjection(1, 1) = xScale;
	result->myLightBufferData.LightProjection(2, 2) = yScale;
	result->myLightBufferData.LightProjection(3, 3) = Q;
	result->myLightBufferData.LightProjection(3, 4) = 1 / Q;
	result->myLightBufferData.LightProjection(4, 3) = -Q * nearPlane;
	result->myLightBufferData.LightProjection(4, 4) = 0.0f;

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
	result->myLightBufferData.LightType = 3;
	result->myLightBufferData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 250000.0f;
	const Vector2f resolution = { 2048, 2048 };

	result->myLightBufferData.LightProjection(1, 1) = 2.0f / resolution.x;
	result->myLightBufferData.LightProjection(2, 2) = 2.0f / resolution.y;
	result->myLightBufferData.LightProjection(3, 3) = 1.0f / (farPlane - nearPlane);
	result->myLightBufferData.LightProjection(4, 3) = nearPlane / (nearPlane - farPlane);
	result->myLightBufferData.LightProjection(4, 4) = 1.0f;

	result->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"SpotLight", resolution.x, resolution.y);

	Renderer::RenderLight(result.get());

	return result;
}
