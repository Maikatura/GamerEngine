#pragma once
#include <Layers/Layer.h>

#include <Types/EnumTypes.h>
#include <Core/Model/Entity.h>
#include "Core/Model/Material.h"

class Inspector : public Layer
{
public:

	Inspector();

	void OnImGuiRender() override;

	void DrawSceneObject(GamerEngine::Entity aEntity);
	void DrawFileObject(GamerEngine::Entity aEntity);


	template <typename T>
	static void DrawComponent(const std::string& aName, GamerEngine::Entity aEntity, std::function<void(T&, GamerEngine::Entity)> aFunction, Ref<Texture> aTexture = nullptr);

	void AddComponent(GamerEngine::Entity aEntity);
	bool ShowTexturePicker(GamerEngine::Entity aEntity, Ref<Material>& selectedMaterial, TextureType textureType);

private:

	bool myIsEditValues = false;
	bool myIsEditValuesOld = false;

	FileType myCurrentFileType = FileType::Error;
};
