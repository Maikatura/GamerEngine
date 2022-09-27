#pragma once
#include <Layers/Layer.h>
#include <Layers/EditorLayers.h>

class MainMenuBar : public Layer
{
public:

	MainMenuBar() = default;
	MainMenuBar(EditorLayers* aLayer);

	bool OnImGuiRender() override;

private:
	EditorLayers* myLayers;
};