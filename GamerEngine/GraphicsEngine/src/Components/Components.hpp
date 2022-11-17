#pragma once
#include "Scene/UUID.h"
#include <string>
#include <CommonUtilities/Math/MathTypes.hpp>
#include <Model/ModelInstance.h>

#include "AssetHandlers/LightAssetHandler.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
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
			myModel = std::make_shared<ModelInstance>();
			myModel->Init(std::make_shared<Model>());
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

struct DirectionalLightComponent
{
	Vector3f Color;
	float Intensity;
	Vector3f Direction;
	bool Active;

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
		myPointLight = LightAssetHandler::CreatePointLight({ 1,1,0 }, 2, 4, { 0,0,0 });
	}

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
