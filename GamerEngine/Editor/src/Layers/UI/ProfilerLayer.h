#pragma once
#include <Layers/Layer.h>
#include <Layers/EditorLayers.h>

#include "Snapshots/SnapshotManager.h"

class ProfilerLayer : public Layer
{
public:

	ProfilerLayer(bool& shouldUseProfiler);

	void OnImGuiRender() override;

	void OnBeginFrame() override;


private:
	bool& myUseProfiler;
};
