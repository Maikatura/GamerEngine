#pragma once
#include <memory>
#include <string>
#include <vector>

#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/Component.h"
#include "Model/ModelInstance.h"
#include "Utilites/COMInitializer.h"

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
					myModel = ModelAssetHandler::Get().GetModelInstance(myPath);

				});
		}
		else
		{
			myPath = aModelPath;
			myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
		}


	}

	ModelComponent(ModelData aModelData)
	{
		ModelAssetHandler::Get().EnqueueLoadTask([&, modelData = aModelData]()
			{
				myPath = modelData.Path;
				myDelay = modelData.Delay;

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
				//myIsLoaded = true;
			});
	}

	void Clear()
	{
		myIsLoaded = false;
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
		if (myModel && myIsLoaded)
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
		return myIsLoaded;
	}

	void SetModel(const std::wstring& aModelPath)
	{
		myPath = aModelPath;
		myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
		
	}

	void SetModelAsync(const std::wstring& aModelPath)
	{
		myPath = aModelPath;
		ModelAssetHandler::Get().EnqueueLoadTask([&]()
			{
				

				
				myModel = ModelAssetHandler::Get().GetModelInstance(myPath);
				//myIsLoaded = true;
				


			});

	}

	float& GetDelay()
	{
		return myDelay;
	}

	void SetDelay(float aDelay)
	{
		myDelay = aDelay;
	}

	bool IsLoaded()
	{
		return myIsLoaded;
	}

	void SetLoaded(bool isLoaded)
	{
		myIsLoaded = isLoaded;
	}

private:


	float myDelay = 0.0f;
	bool myIsLoaded = false;
	std::wstring myPath;
	std::shared_ptr<ModelInstance> myModel = nullptr;
};