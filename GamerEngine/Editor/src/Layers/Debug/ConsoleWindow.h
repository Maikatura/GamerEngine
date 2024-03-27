#pragma once
#include "Layers/Layer.h"

class ConsoleWindow : public Layer
{
	public:
		ConsoleWindow();

		void OnImGuiRender() override;

	
		
	
	private:
		int myOldSize = 0;
		bool scrollToBottom;
};