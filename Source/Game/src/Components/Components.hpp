#pragma once
#include "Renderer/Scene/UUID.h"
#include <string>
#include <Math/MathTypes.hpp>
#include <Renderer/Model/ModelInstance.h>

#include "Renderer/AssetHandlers/LightAssetHandler.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Component.h"
#include "Components/CameraComponent.h"

class ScriptableEntity;

class IDComponent
{
public:
	UUID2 ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
};

class TagComponent
{
public:
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag)
	{}
};

class TransformComponent
{
public:
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };



	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Vector3f& translation)
		: Translation(translation)
	{}

	void LookAt(const Vector3f aPosition)
	{
		if(aPosition.x == Translation.x &&
			aPosition.y == Translation.y &&
			aPosition.z == Translation.z)
		{
			return;
		}

		Vector3f lookAtPos;
		lookAtPos.x = Translation.x - aPosition.x;
		lookAtPos.y = Translation.y - aPosition.y;
		lookAtPos.z = Translation.z - aPosition.z;

		float pitch = 0.0f;
		if(lookAtPos.y != 0.0f)
		{
			const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
			pitch = atan(lookAtPos.y / distance);
		}

		float yaw = 0.0f;
		if(lookAtPos.x != 0.0f)
		{
			yaw = atan(lookAtPos.x / lookAtPos.z);
		}

		if(lookAtPos.z > 0.0f)
		{
			yaw += 3.1415f;
		}


		Rotation = { pitch * (180.0f / 3.14f), yaw * (180.0f / 3.14f), 0.0f };
		Rotation;
	}

	Matrix4x4f GetMatrix()
	{
		return Matrix4x4f::BuildTransform(Translation, Rotation, Scale);
	}
};

class ModelComponent : public Component
{
public:
	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(std::shared_ptr<ModelInstance> aModel) : myModel(aModel)
	{ }

	std::shared_ptr<ModelInstance> GetModel()
	{
		if (!myModel)
		{
			return nullptr;
		}

		return myModel;
	}

	void Update() override
	{
		myModel->Update();
	}

	void SetModel(std::shared_ptr<ModelInstance> aModel)
	{
		myModel = aModel;
		myModel->Init(GetModel()->GetModel());
	}

private:

	std::shared_ptr<ModelInstance> myModel = nullptr;
};

struct NativeScriptComponent
{
	ScriptableEntity* Instance = nullptr;

	ScriptableEntity* (*InstantiateScript)();
	void (*DestroyScript)(NativeScriptComponent*);

	template<typename T>
	void Bind()
	{
		InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
	}
};

class DirectionalLightComponent
{
public:
	Vector3f Color;
	float Intensity = 1;
	Vector3f Direction;
	bool Active = true;
	bool SmoothShadows = false;

	DirectionalLightComponent()
	{
		myDirectionalLight = LightAssetHandler::CreateDirectionalLight({ 1,1,1 }, 2.0f, { -140,60,178 });
	}




	std::shared_ptr<DirectionalLight> myDirectionalLight;
};

struct PointLightComponent
{
	PointLightComponent()
	{
		Intensity = 1.0f;
		Range = 4.0f;

		myPointLight = LightAssetHandler::CreatePointLight({ 1,1,0 }, Intensity, 4, { 0,0,0 });
	}

	void Update()
	{
		myPointLight->SetRange(Range);
		myPointLight->SetIntensity(Intensity);
	}

	float Intensity;
	float Range;

	std::shared_ptr<PointLight> myPointLight;
};

struct SpotLightComponent
{
	SpotLightComponent()
	{
		mySpotLight = LightAssetHandler::CreateSpotLight({1,1,1}, 1, 2, {0,0,0}, 1, 3);
	}
	std::shared_ptr<SpotLight> mySpotLight;
};

template<typename... Component>
struct ComponentGroup
{};

using AllComponents =
ComponentGroup<TransformComponent,
	NativeScriptComponent, ModelComponent, CameraComponent, DirectionalLightComponent>;
