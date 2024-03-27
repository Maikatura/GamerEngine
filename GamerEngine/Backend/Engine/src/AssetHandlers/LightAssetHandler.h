#pragma once
#include "Light/DirectionalLight.h"
#include "Light/EnvironmentLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/Light.h"
#include "Utilites/Pointers.h"


class LightAssetHandler
{
	static inline Ref<DirectionalLight> myDirectionalLight;
	static inline Ref<EnvironmentLight> myEnvironmentLight;

public:

	~LightAssetHandler();

	static Ref<DirectionalLight> CreateDirectionalLight(Vector3f aColor, float anIntensity, Vector3f aRotation);
	static Ref<EnvironmentLight> CreateEnvironmentLight(const std::wstring& aTextureName);


	static Ref<PointLight> CreatePointLight(Vector3f aColor, float anIntensity, float aRange, Vector3f aPosition);
	static Ref<SpotLight> CreateSpotLight(Vector3f aColor, float anIntensity, float aRange, Vector3f aPosition, float aInnerCone, float aOuterCone);
};
