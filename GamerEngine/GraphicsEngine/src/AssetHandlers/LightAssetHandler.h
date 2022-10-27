#pragma once
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Light/Light.h"


class LightAssetHandler
{
	static inline std::vector<std::shared_ptr<Light>> myLights{};
	static inline std::shared_ptr<DirectionalLight> myDirectionalLight;
	static inline std::shared_ptr<EnvironmentLight> myEnvironmentLight;

public:

	~LightAssetHandler();

	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(Vector3f aColor, float anIntensity, Vector3f aRotation);
	static std::shared_ptr<EnvironmentLight> CreateEnvironmentLight(const std::wstring& aTextureName);
};
