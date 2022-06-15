#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <Model/Model.h>

class ModelAssetHandler
{
	inline static std::unordered_map<std::wstring, std::shared_ptr<Model>> myModelRegistry;

	bool InitUnitCube() const;
	bool InitUnitSphere() const;

public:
	bool Initialize() const;

	std::shared_ptr<Model> GetModel(const std::wstring& aFilePath) const;
};

