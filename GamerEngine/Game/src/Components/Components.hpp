#pragma once
#include <Math/MathTypes.hpp>

#include "AssetHandlers/LightAssetHandler.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Component.h"
#include "Components/CameraComponent.h"
#include "Utilites/StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"

#include "Components/TransfromComponent.h"
#include "Components/ModelComponent.h"
#include "Components/TagComponent.h"
#include "Components/IDComponent.h"

struct NativeScriptComponent;






class DirectionalLightComponent : public Component
{
public:
	Vector3f Color;
	float Intensity = 1;
	Vector3f Direction;
	bool Active = true;
	bool SmoothShadows = false;

	DirectionalLightComponent()
	{
		myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 1.0f, { -140,60,178 });
	}




	std::shared_ptr<DirectionalLight> myDirectionalLight;
};

struct PointLightComponent : public Component
{
	PointLightComponent()
	{
		Intensity = 1.0f;
		Range = 4.0f;

		myPointLight = LightAssetHandler::CreatePointLight({ 1,1,0 }, Intensity, 4, { 0,0,0 });
	}

	void OnUpdate() override
	{
		myPointLight->SetRange(Range);
		myPointLight->SetIntensity(Intensity);
		myPointLight->Update();
	}

	float Intensity;
	float Range;

	std::shared_ptr<PointLight> myPointLight;
};

struct SpotLightComponent : public Component
{
	SpotLightComponent()
	{
		mySpotLight = LightAssetHandler::CreateSpotLight({1,1,1}, 1, 2, {0,0,0}, 1, 3);
	}

	void OnUpdate() override
	{}

	std::shared_ptr<SpotLight> mySpotLight;
};

template<typename... Component>
struct ComponentGroup
{};


using AllComponents =
ComponentGroup<TransformComponent,
	NativeScriptComponent, ModelComponent, CameraComponent, DirectionalLightComponent>;
