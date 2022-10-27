#pragma once
#include <Layers/Layer.h>

#include <Types/EnumTypes.h>
#include <Model/Entity.h>

class Inspector : public Layer
{
public:
	bool OnImGuiRender() override;

	void DrawSceneObject(Entity& aEntity);
	void DrawFileObject(Entity& aEntity);

	void AddComponent(Entity& aEntity);

private:

	std::wstring DropFile();

	FileType myCurrentFileType = FileType::Error;
};
