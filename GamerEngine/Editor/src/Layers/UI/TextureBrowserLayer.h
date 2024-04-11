#pragma once
#include <Layers/Layer.h>
#include <Layers/EditorLayers.h>

#include "Snapshots/SnapshotManager.h"

class TextureBrowserLayer : public Layer
{
public:

	TextureBrowserLayer();

	void OnImGuiRender() override;



private:
	
};
