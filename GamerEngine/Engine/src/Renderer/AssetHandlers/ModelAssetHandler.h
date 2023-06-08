#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <Renderer/Model/Model.h>
#include <vector>
#include "Renderer/Model/ModelInstance.h"

namespace ofbx
{
	struct IScene;
}

const int InputLayoutSize = 19;

class ModelAssetHandler
{
	std::unordered_map<std::wstring, std::shared_ptr<ModelInstance>> myModelRegistry;

	bool InitUnitCube();
	bool InitUnitSphere();


	ofbx::IScene* LoadModelScene(const std::string& aPath);




	inline static std::shared_ptr<ModelAssetHandler> myInstance;

public:


	static ModelAssetHandler& Get();

	void Clear();
	bool Initialize();

	void UnloadModel(const std::wstring& aFilePath);

	void ResetRenderedModels();

	bool LoadModelNewTesting(const std::wstring& aFilePath);

	bool LoadModelData(const std::wstring& aFilePath);
	bool LoadAnimationData(const std::wstring& aModelName, const std::wstring& someFilePath);

	std::shared_ptr<Model> GetModel(const std::wstring& aFilePath);
	std::shared_ptr<ModelInstance> GetModelInstance(const std::wstring& aModelName);

private:
	std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> CreateLayout();

};

