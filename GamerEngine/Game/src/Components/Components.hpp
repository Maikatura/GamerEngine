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




	Ref<DirectionalLight> myDirectionalLight;
};

struct PointLightComponent : public Component
{
	PointLightComponent()
	{
		Color = { 1, 1, 0 };
		Intensity = 1.0f;
		Range = 4.0f;

		myPointLight = LightAssetHandler::CreatePointLight(Color, Intensity, Range, { 0,0,0 });
	}

	void OnUpdate() override
	{
		myPointLight->SetColor(Color);
		myPointLight->SetRange(Range);
		myPointLight->SetIntensity(Intensity);
	}

	Vector3f Color;
	float Intensity;
	float Range;

	Ref<PointLight> myPointLight;
};

struct SpotLightComponent : public Component
{
	SpotLightComponent()
	{
		Color = { 1, 0, 1 };
		Intensity = 1.0f;
		Range = 4.0f;
		InnerCone = 20.0f;
		OuterCone = 130.0f;

		mySpotLight = LightAssetHandler::CreateSpotLight(Color, Intensity, Range, {0,0,0}, InnerCone, OuterCone);
	}

	void OnUpdate() override
	{
		mySpotLight->SetColor(Color);
		mySpotLight->SetIntensity(Intensity);
		mySpotLight->SetRange(Range);
		mySpotLight->SetInnerCone(InnerCone);
		mySpotLight->SetOuterCone(OuterCone);
	}

	Vector3f Color;
	float Intensity;
	float Range;
	float InnerCone;
	float OuterCone;

	Ref<SpotLight> mySpotLight;
};

template<typename... Component>
struct ComponentGroup
{};


using AllComponents =
ComponentGroup<TransformComponent,
	NativeScriptComponent, ModelComponent, CameraComponent, DirectionalLightComponent>;
