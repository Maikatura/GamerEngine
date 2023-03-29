#pragma once
#include "Renderer/Light/DirectionalLight.h"
#include "Renderer/Light/EnvironmentLight.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Renderer/Light/Light.h"


class LightAssetHandler
{
	static inline std::vector<std::shared_ptr<Light>> myLights{};
	static inline std::shared_ptr<DirectionalLight> myDirectionalLight;
	static inline std::shared_ptr<EnvironmentLight> myEnvironmentLight;

public:

	~LightAssetHandler();

	static std::shared_ptr<DirectionalLight> CreateDirectionalLight(Vector3f aColor, float anIntensity, Vector3f aRotation);
	static std::shared_ptr<EnvironmentLight> CreateEnvironmentLight(const std::wstring& aTextureName);


	static std::shared_ptr<PointLight> CreatePointLight(Vector3f aColor, float anIntensity, float aRange, Vector3f aPosition);
	static std::shared_ptr<SpotLight> CreateSpotLight(Vector3f aColor, float anIntensity, float aRange, Vector3f aPosition, float aInnerCone, float aOuterCone);
};
