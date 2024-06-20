#pragma once
#include <memory>
#include <string>
#include <vector>

#include "AssetHandlers/ModelAssetHandler.h"
#include <Core/Model/Model.h>

#include "AssetHandlers/ShaderHandler.h"
#include "Utilites/COMInitializer.h"
#include "Utilites/StringCast.h"


class MeshMaterialData
{
public:
	std::wstring PixelShader;
	std::wstring Albedo;
	std::wstring Normal;
	std::wstring Material;
};

class ModelData
{
public:
	std::wstring Path;

	std::vector<MeshMaterialData> MeshMaterialData;


	float Delay;
};

class ModelComponent
{
public:
	ModelComponent() = default;
	ModelComponent(const ModelComponent&) = default;
	ModelComponent(Ref<GamerEngine::Model> aModel) : myModel(aModel)
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

					model.myPixelShader = ShaderHandler::Get().LoadPixelShader(Helpers::string_cast<std::string>(modelData.MeshMaterialData[i].PixelShader));
					model.MaterialData.SetAlbedoTexture(TextureAssetHandler::GetTexture(modelData.MeshMaterialData[i].Albedo));
					model.MaterialData.SetNormalTexture(TextureAssetHandler::GetTexture(modelData.MeshMaterialData[i].Normal));
					model.MaterialData.SetMaterialTexture(TextureAssetHandler::GetTexture(modelData.MeshMaterialData[i].Material));
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

	Ref<GamerEngine::Model> GetModel()
	{
		if (!myModel)
		{
			return nullptr;
		}

		return myModel;
	}

	void OnUpdate()
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

	void SetModel(Ref<GamerEngine::Model> aModel)
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

	Vector3f& GetMinBounds()
	{
		return myBoundsMin;
	}

	Vector3f& GetMaxBounds()
	{
		return myBoundsMax;
	}


private:


	float myDelay = 0.0f;
	bool myIsLoaded = false;
	std::wstring myPath;
	Ref<GamerEngine::Model> myModel = nullptr;

	Vector3f myBoundsMin;
	Vector3f myBoundsMax;

};