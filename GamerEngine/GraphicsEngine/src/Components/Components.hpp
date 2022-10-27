#pragma once
#include "Scene/UUID.h"
#include <string>
#include <CommonUtilities/Math/MathTypes.hpp>
#include <Model/ModelInstance.h>

class ScriptableEntity;

struct IDComponent
{
	UUID2 ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
};

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag)
	{}
};

struct TransformComponent
{
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
		Vector3f rotation;
		rotation.x = static_cast<int>(Rotation.x) % static_cast<int>(360.0f);
		rotation.y = static_cast<int>(Rotation.y) % static_cast<int>(360.0f);
		rotation.z = static_cast<int>(Rotation.z) % static_cast<int>(360.0f);
		
		return Matrix4x4f::BuildTransform(Translation, rotation, Scale);
	}
};

struct ModelComponent
{
	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(std::shared_ptr<ModelInstance> aModel) : myModel(aModel)
	{ }

	std::shared_ptr<ModelInstance> myModel;
};

struct CameraComponent
{
#define PI 3.14f
	Matrix4x4f Projection;
	Matrix4x4f ViewProjection;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;

	bool Primary = true;

	CameraComponent()
	{
		Initialize(90, 0.1f, 1500.0f, { 1920, 1080 });
	}
	CameraComponent(const CameraComponent&) = default;

	void Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution = {0,0})
	{
		assert(aNearPlane < aFarPlane);
		assert(aNearPlane >= 0.01f);

		myFoV = aHorizontalFoV;

		myNearPlane = aNearPlane;
		myFarPlane = aFarPlane;

		if (aResolution.x == 0 && aResolution.y == 0)
		{
			Resize(myRes);
		}
		else
		{
			Resize(aResolution);
		}
	}

	void Resize(Vector2ui aResolution)
	{
		myRes.x = aResolution.x;
		myRes.y = aResolution.y;

		const float hFoVRad = myFoV * (PI / 180.f);
		const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

		const float myXScale = 1 / std::tanf(hFoVRad * 0.5f);
		const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
		const float Q = myFarPlane / (myFarPlane - myNearPlane);

		Projection(1, 1) = myXScale;
		Projection(2, 2) = myYScale;
		Projection(3, 3) = Q;
		Projection(3, 4) = 1.0f / Q;
		Projection(4, 3) = -Q * myNearPlane;
		Projection(4, 4) = 0.0f;

	}

	float GetResScale()
	{
		return static_cast<float>(myRes.y) / static_cast<float>(myRes.x);
	}

private:
	Vector2ui myRes;

};

struct NativeScriptComponent
{
	ScriptableEntity* Instance = nullptr;

	ScriptableEntity* (*InstantiateScript)(){};
	void (*DestroyScript)(NativeScriptComponent*){};

	template<typename T>
	T& Get()
	{
		return *(T*)Instance;
	}

	template<typename T>
	T* GetPtr()
	{
		return (T*)Instance;
	}

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
};

template<typename... Component>
struct ComponentGroup
{};

using AllComponents =
ComponentGroup<TransformComponent,
	NativeScriptComponent, ModelComponent, CameraComponent, DirectionalLightComponent>;
