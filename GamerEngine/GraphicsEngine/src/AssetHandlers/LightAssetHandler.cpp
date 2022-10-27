#include "GraphicsEngine.pch.h"
#include <AssetHandlers/LightAssetHandler.h>

LightAssetHandler::~LightAssetHandler()
{
	myLights.clear();
}

std::shared_ptr<DirectionalLight> LightAssetHandler::CreateDirectionalLight(Vector3f aColor, float anIntensity,
                                                                            Vector3f aRotation)
{
	myDirectionalLight = std::make_shared<DirectionalLight>();
	myDirectionalLight->Init(aColor, anIntensity);

	myDirectionalLight->SetDirection(aRotation);

	return myDirectionalLight;
}

std::shared_ptr<EnvironmentLight> LightAssetHandler::CreateEnvironmentLight(const std::wstring& aTextureName)
{
	return myEnvironmentLight;
}
