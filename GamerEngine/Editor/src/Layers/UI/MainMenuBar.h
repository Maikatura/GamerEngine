#pragma once
#include <Layers/Layer.h>


#include "Snapshots/SnapshotManager.h"

class MainMenuBar : public Layer
{
public:

	MainMenuBar();

	void OnImGuiRender() override;



private:

	void RenderMainBar();
	void RenderFooter();

	SnapshotManager mySnapshot;
};
