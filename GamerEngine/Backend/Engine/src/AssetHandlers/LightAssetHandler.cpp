#include "GraphicsEngine.pch.h"
#include <AssetHandlers/LightAssetHandler.h>

#include "Core/Rendering/Renderer.h"

LightAssetHandler::~LightAssetHandler()
{
	
}

Ref<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3f aColor, float anIntensity,
	Vector3f aRotation)
{
	myDirectionalLight = MakeRef<DirectionalLight>();
	myDirectionalLight->Init(aColor, anIntensity);

	myDirectionalLight->SetLightPosition({0, 0, 0});
	myDirectionalLight->SetLightDirection({0,0,0});


	myDirectionalLight->myLightData.LightType = 1;

#define DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION 8192.0f

	constexpr float nearPlane = 30.0f;
	constexpr float farPlane = 25000.0f;
	constexpr float DIVIDE = 0.1f;

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

	myDirectionalLight->myLightData.LightProjection = Matrix4x4f::CreateOrthographicProjection(-DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION * DIVIDE, DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION * DIVIDE, -DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION * DIVIDE, DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION * DIVIDE, nearPlane, farPlane);
	myDirectionalLight->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"DirectionalLight", static_cast<size_t>(DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION), static_cast<size_t>(DIRECTIONAL_LIGHT_SHADOW_MAP_RESOLUTION));
	myDirectionalLight->myLightData.CastShadows = true;



	return myDirectionalLight;
}

Ref<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::wstring& aTextureName)
{
	myEnvironmentLight = MakeRef<EnvironmentLight>();
	myEnvironmentLight->myTexture = TextureAssetHandler::GetTexture(aTextureName);
	return myEnvironmentLight;
}

#define POINT_LIGHT_SHADOW_MAP_RESOLUTION 2048

Ref<PointLight> LightAssetHandler::CreatePointLight(Vector3f aColor, float anIntensity, float aRange,
	Vector3f aPosition)
{
	Ref<PointLight> result = MakeRef<PointLight>();

	result->Init(aColor, anIntensity);
	result->SetRange(aRange);
	result->SetLightPosition(aPosition);
	result->myLightData.LightType = 2;
	result->myLightData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 25000.0f;
	const Vector2ui resolution = { POINT_LIGHT_SHADOW_MAP_RESOLUTION, POINT_LIGHT_SHADOW_MAP_RESOLUTION };
	result->myShadowMap = TextureAssetHandler::CreatePointLightMap(L"Point Light", static_cast<size_t>(resolution.x), static_cast<size_t>(resolution.y));

	result->myLightData.NearPlane = nearPlane;
	result->myLightData.FarPlane = farPlane;
	result->myLightData.LightProjection = Matrix4x4f::CreatePerspectiveHFoVResolution(90, { resolution.x, resolution.y }, nearPlane, farPlane);

	return result;
}

Ref<SpotLight> LightAssetHandler::CreateSpotLight(Vector3f aColor, float anIntensity, float aRange,
	Vector3f aPosition, float aInnerCone, float aOuterCone)
{
	Ref<SpotLight> result = MakeRef<SpotLight>();

	result->Init(aColor, anIntensity);
	result->SetRange(aRange);
	result->SetInnerCone(aInnerCone);
	result->SetOuterCone(aOuterCone);
	result->myLightData.LightType = 3;
	result->myLightData.CastShadows = true;

	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 250000.0f;
	const Vector2f resolution = { 2048, 2048 };

	result->myLightData.LightProjection(1, 1) = 2.0f / resolution.x;
	result->myLightData.LightProjection(2, 2) = 2.0f / resolution.y;
	result->myLightData.LightProjection(3, 3) = 1.0f / (farPlane - nearPlane);
	result->myLightData.LightProjection(4, 3) = nearPlane / (nearPlane - farPlane);
	result->myLightData.LightProjection(4, 4) = 1.0f;

	result->myShadowMap = TextureAssetHandler::CreateDepthStencil(L"SpotLight", static_cast<size_t>(resolution.x), static_cast<size_t>(resolution.y));

	return result;
}
