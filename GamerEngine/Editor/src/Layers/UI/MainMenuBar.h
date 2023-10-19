#pragma once
#include <Layers/Layer.h>
#include <Layers/EditorLayers.h>

#include "Snapshots/SnapshotManager.h"

class MainMenuBar : public Layer
{
public:

	MainMenuBar() = default;
	MainMenuBar(EditorLayers& aLayer);

	void OnImGuiRender() override;



private:

	void RenderMainBar();
	void RenderFooter();

	EditorLayers& myLayers;
	SnapshotManager mySnapshot;
};
