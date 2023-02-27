#pragma once
#include <Layers/Layer.h>



class HelpPanel : public Layer
{
public:

	void OnAttach() override;
	bool OnImGuiRender() override;
};