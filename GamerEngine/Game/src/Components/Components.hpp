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
#include "Component.h"
#include "StringCast.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Managers/ThreadPool.h"
#include "Utilites/COMInitializer.h"

struct NativeScriptComponent;
class ScriptableEntity;



class IDComponent : public Component
{
public:
	UUID2 ID;

	IDComponent() = default;
	IDComponent(const IDComponent&) = default;
	IDComponent(UUID2 aUUID)
	{
		ID = aUUID;
	}
};

class TagComponent : public Component
{
public:
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag)
	{}
};

class TransformComponent : public Component
{
	Vector3f Translation = { 0.0f, 0.0f, 0.0f };
	Vector3f Rotation = { 0.0f, 0.0f, 0.0f };
	Vector3f Scale = { 1.0f, 1.0f, 1.0f };

public:
	

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const Vector3f& translation)
		: Translation(translation)
	{}

	Vector3f GetPosition() const
	{
		return Translation;
	}

	Vector3f& GetPosition()
	{
		return Translation;
	}

	void SetPosition(Vector3f aPosition)
	{
		Translation = aPosition;
	}

	Vector3f GetRotation() const
	{
		return Rotation;
	}

	Vector3f& GetRotation()
	{
		return Rotation;
	}

	void SetRotation(Vector3f aRotation)
	{
		Rotation = aRotation;
	}

	Vector3f GetScale() const
	{
		return Scale;
	}

	Vector3f& GetScale()
	{
		return Scale;
	}

	void SetScale(Vector3f aScale)
	{
		Scale = aScale;
	}


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
	}

	Vector3f Forward()
	{
		const float forwardDistance = 5.0f;
		Vector3f forwardVector = CommonUtilities::Quat(Rotation).Forward();
		return Translation + forwardVector * forwardDistance;
	}

	Matrix4x4f GetMatrix()
	{
		return ComposeFromTRS(Translation, CommonUtilities::Quat::FromEulers(ToRadians(Rotation)), Scale);
	}
};

class ModelData
{
public:
	std::wstring Path;

	std::vector<std::wstring> Albedo;
	std::vector<std::wstring> Normal;
	std::vector<std::wstring> Material;
	float Delay;
};

class ModelComponent : public Component
{
public:
	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(std::shared_ptr<ModelInstance> aModel) : myModel(aModel)
	{ }

	ModelComponent(const std::wstring& aModelPath, float aDelay = 0.0f, bool aShouldThread = true)
	{
		
		myPath = aModelPath;
		myDelay = aDelay;

		if (aShouldThread)
		{
			ModelAssetHandler::Get().EnqueueLoadTask([&]()
				{


					if (myDelay <= 0.0f)
					{
						myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
						myHasLoaded = true;
					}

					myDelay -= Time::GetDeltaTime();
				});
		}
		else
		{
			myPath = aModelPath;
			myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
			myHasLoaded = true;
		}

		
	}

	ModelComponent(ModelData aModelData)
	{
		ModelAssetHandler::Get().EnqueueLoadTask([&, modelData = aModelData]() 
			{
				myPath = modelData.Path;
				myModel = ModelAssetHandler::Get().GetModelInstance(modelData.Path);

				if (!myModel)
				{
					return;
				}

				auto& materials = myModel->GetMaterial();

				COMInitializer comInitializer;

				for (int i = 0; i < modelData.Albedo.size(); i++)
				{
					materials[i]->SetAlbedoTexture(TextureAssetHandler::GetTexture(modelData.Albedo[i]));
				}

				for (int i = 0; i < modelData.Normal.size(); i++)
				{
					materials[i]->SetNormalTexture(TextureAssetHandler::GetTexture(modelData.Normal[i]));
				}

				for (int i = 0; i < modelData.Material.size(); i++)
				{
					materials[i]->SetMaterialTexture(TextureAssetHandler::GetTexture(modelData.Material[i]));
				}
			});
	}

	void Clear()
	{
		myHasLoaded = false;
		myModel = nullptr;
		myPath = L"";
	}

	std::shared_ptr<ModelInstance> GetModel()
	{
		if (!myModel)
		{
			return nullptr;
		}

		return myModel;
	}

	void OnUpdate() override
	{
		if(myModel && myHasLoaded)
		{
			myModel->Update();
		}

		
	}

	void OnEditorUpdate()
	{
		if (myModel)
		{
			myModel->EditorUpdate();
		}
	}

	void SetModel(std::shared_ptr<ModelInstance> aModel)
	{
		/*if(myModel)
		{
			myModel->EditorUpdate();
		}*/
	}

	bool HasBeenLoaded()
	{
		return myHasLoaded;
	}

	void SetModel(const std::wstring& aModelPath)
	{
		myPath = aModelPath;
		myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
		myHasLoaded = true;
	}

	void SetModelAsync(const std::wstring& aModelPath)
	{
		myPath = aModelPath;
		ModelAssetHandler::Get().EnqueueLoadTask([&]()
			{
				if (myDelay <= 0.0f)
				{
					myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
					myHasLoaded = true;
				}

				myDelay -= Time::GetDeltaTime();

			});
		
	}

	float GetDelay()
	{
		return myDelay;
	}

	void SetDelay(float aDelay)
	{
		myDelay = aDelay;
	}

private:

	
	float myDelay;
	bool myHasLoaded = false;
	std::wstring myPath;
	std::shared_ptr<ModelInstance> myModel = nullptr;
};

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
