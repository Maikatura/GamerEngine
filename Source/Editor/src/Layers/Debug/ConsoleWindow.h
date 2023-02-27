#pragma once
#include "Layers/Layer.h"

class ConsoleWindow : public Layer
{
public:
	bool OnImGuiRender() override;
};