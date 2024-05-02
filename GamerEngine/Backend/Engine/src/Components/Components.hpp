#pragma once
#include <Math/MathTypes.hpp>
#include "AssetHandlers/LightAssetHandler.h"
#include "Scene/SceneManager.h"


class LightComponent
{

public:
	virtual ~LightComponent() = default;

	LightComponent()
	{
			Color = { 1,1,1 };
			Intensity = 1.0f;
			CastShadow = true;
			Active = true;
			SmoothShadow = false;
	}

	virtual void OnUpdate() = 0;

	virtual Light* GetLight() = 0;

	Vector3f Color;
	float Intensity;
	bool CastShadow;
	bool Active;
	bool SmoothShadow ;


};


class DirectionalLightComponent : public LightComponent
{
public:
	

	DirectionalLightComponent()
	{
		myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 1.0f, { -140,60,178 });
	}


	void OnUpdate() override
	{
		myDirectionalLight->SetColor(Color);
		myDirectionalLight->SetIntensity(Intensity * 0.00001f);
		myDirectionalLight->SetCastShadows(CastShadow);
		myDirectionalLight->SetActive(Active);

		SceneManager::Get().GetScene()->SetDirectionalLight(myDirectionalLight);
	}

	DirectionalLight* GetLight() override
	{
		return myDirectionalLight.get();
	}

	Ref<DirectionalLight> myDirectionalLight;
};

class PointLightComponent : public LightComponent
{
public:
	PointLightComponent()
	{
		Range = 4.0f;
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

	PointLight* GetLight() override
	{
		return myPointLight.get();
	}
	
	float Range;
	

	Ref<PointLight> myPointLight;
};

class SpotLightComponent : public LightComponent
{
public:
	SpotLightComponent()
	{
		
		Range = 4.0f;
		InnerCone = 20.0f;
		OuterCone = 130.0f;
		

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

	SpotLight* GetLight() override
	{
		return mySpotLight.get();
	}

	
	float Range;
	float InnerCone;
	float OuterCone;
	

	Ref<SpotLight> mySpotLight;
};

namespace GamerEngine
{
	struct ScriptComponent
	{
		std::string ClassName;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};
}


template<typename... Component>
struct ComponentGroup
{};



