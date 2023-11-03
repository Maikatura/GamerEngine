#pragma once
#include <Layers/Layer.h>

#include <Types/EnumTypes.h>
#include <Model/Entity.h>

#include "Components/RandomMoverComponent.h"

class Inspector : public Layer
{
public:

	Inspector();

	void OnImGuiRender() override;

	void DrawSceneObject(Entity& aEntity);
	void DrawFileObject(Entity& aEntity);

	void AddComponent(Entity& aEntity);
	bool ShowTexturePicker(Entity& aEntity, Ref<Material>& selectedMaterial, TextureType textureType);

private:

	bool myIsEditValues = false;
	bool myIsEditValuesOld = false;

	FileType myCurrentFileType = FileType::Error;
};
