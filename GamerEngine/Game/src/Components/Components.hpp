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
	

	DirectionalLightComponent()
	{
		Color = { 1,1,1 };
		Intensity = 1.0f;
		CastShadow = true;
		Active = true;
		SmoothShadow = false;
		myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 1.0f, { -140,60,178 });
	}


	void OnUpdate() override
	{
		myDirectionalLight->SetColor(Color);
		myDirectionalLight->SetIntensity(Intensity * 0.00001f);
		myDirectionalLight->SetCastShadows(CastShadow);
		myDirectionalLight->SetActive(Active);

	}

	Vector3f Color;
	float Intensity;
	bool CastShadow;
	bool Active;
	bool SmoothShadow;

	Ref<DirectionalLight> myDirectionalLight;
};

struct PointLightComponent : public Component
{
public:
	PointLightComponent()
	{
		Color = { 1, 1, 0 };
		Intensity = 1.0f;
		Range = 4.0f;
		Active = true;
		CastShadow = true;
		SmoothShadow = true;

		myPointLight = LightAssetHandler::CreatePointLight(Color, Intensity, Range, { 0,0,0 });
	}

	void OnUpdate() override
	{
		myPointLight->SetColor(Color);
		myPointLight->SetRange(Range);
		myPointLight->SetIntensity(Intensity);
		myPointLight->SetCastShadows(CastShadow);
		myPointLight->SetActive(Active);
	}

	Vector3f Color;
	float Intensity;
	float Range;
	bool CastShadow;
	bool Active;
	bool SmoothShadow;

	Ref<PointLight> myPointLight;
};

struct SpotLightComponent : public Component
{
public:
	SpotLightComponent()
	{
		Color = { 1, 0, 1 };
		Intensity = 1.0f;
		Range = 4.0f;
		InnerCone = 20.0f;
		OuterCone = 130.0f;
		SmoothShadow = true;

		mySpotLight = LightAssetHandler::CreateSpotLight(Color, Intensity, Range, {0,0,0}, InnerCone, OuterCone);
	}

	void OnUpdate() override
	{
		mySpotLight->SetColor(Color);
		mySpotLight->SetRange(Range);
		mySpotLight->SetIntensity(Intensity);
		mySpotLight->SetInnerCone(InnerCone);
		mySpotLight->SetOuterCone(OuterCone);
		mySpotLight->SetCastShadows(CastShadow);
		mySpotLight->SetActive(Active);
	}

	Vector3f Color;
	float Intensity;
	float Range;
	float InnerCone;
	float OuterCone;
	bool CastShadow;
	bool Active;
	bool SmoothShadow;

	Ref<SpotLight> mySpotLight;
};

template<typename... Component>
struct ComponentGroup
{};


using AllComponents =
ComponentGroup<TransformComponent,
	NativeScriptComponent, ModelComponent, CameraComponent, DirectionalLightComponent>;
