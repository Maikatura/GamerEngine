#pragma once

#include <memory>
#include <Layers/Layer.h>

#include "Model/EnumTypes.h"

class Inspector : public Layer
{
public:
	bool OnImGuiRender() override;

	void DrawSceneObject();
	void DrawFileObject();

private:
	FileType myCurrentFileType;
};
