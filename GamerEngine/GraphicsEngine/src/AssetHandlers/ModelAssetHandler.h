#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <Model/Model.h>
#include <vector>
#include "Model/ModelInstance.h"

const int InputLayoutSize = 18;

class ModelAssetHandler
{
	inline static std::unordered_map<std::wstring, std::shared_ptr<ModelInstance>> myModelRegistry;

	static bool InitUnitCube();
	static bool InitUnitSphere();

public:

	static void Clear();
	static bool Initialize();

	static void UnloadModel(const std::wstring& aFilePath);

	static void ResetRenderedModels();

	static bool LoadModel(const std::wstring& aFilePath);
	static bool LoadAnimation(const std::wstring& aModelName, const std::wstring& someFilePath);

	static std::shared_ptr<Model> GetModel(const std::wstring& aFilePath);
	static std::shared_ptr<ModelInstance> GetModelInstance(const std::wstring& aModelName);

private:
	static std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> CreateLayout();

};

