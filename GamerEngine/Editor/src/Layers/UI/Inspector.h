#pragma once
#include <Layers/Layer.h>

#include <Renderer/Types/EnumTypes.h>
#include <Renderer/Model/Entity.h>

#include "Components/RandomMoverComponent.h"

class Inspector : public Layer
{
public:

	Inspector();

	bool OnImGuiRender() override;

	void DrawSceneObject(Entity& aEntity);
	void DrawFileObject(Entity& aEntity);

	void AddComponent(Entity& aEntity);

private:

	bool myIsEditValues = false;
	bool myIsEditValuesOld = false;

	FileType myCurrentFileType = FileType::Error;
};
