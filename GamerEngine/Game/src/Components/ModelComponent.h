#pragma once
#include <memory>
#include <string>
#include <vector>

#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/Component.h"
#include "..\..\..\Engine\src\Model\Model.h"
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
	ModelComponent(Ref<ModelInstance> aModel) : myModel(aModel)
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

				for (int i = 0; i < myModel->GetNumMeshes(); i++)
				{
					COMInitializer comInitializer;
					auto& model = myModel->GetMeshData(i);
					model.MaterialData.SetAlbedoTexture(TextureAssetHandler::GetTexture(modelData.Albedo[i]));
					model.MaterialData.SetNormalTexture(TextureAssetHandler::GetTexture(modelData.Normal[i]));
					model.MaterialData.SetMaterialTexture(TextureAssetHandler::GetTexture(modelData.Material[i]));
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

	Ref<ModelInstance> GetModel()
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

	void SetModel(Ref<ModelInstance> aModel)
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
	Ref<ModelInstance> myModel = nullptr;
};