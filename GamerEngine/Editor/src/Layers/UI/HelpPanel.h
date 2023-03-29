#pragma once
#include <Layers/Layer.h>



class HelpPanel : public Layer
{
public:

	HelpPanel();

	void OnAttach() override;
	void OnImGuiRender() override;
};