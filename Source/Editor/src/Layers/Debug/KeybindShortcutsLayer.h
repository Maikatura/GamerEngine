#pragma once
#include "Layers/Layer.h"

class KeybindShortcutsLayer : public Layer
{
	bool myOpenSave = false;
public:
	KeybindShortcutsLayer();

	bool OnImGuiRender() override;
	void OnUpdate() override;


};

