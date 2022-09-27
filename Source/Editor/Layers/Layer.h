#pragma once
#include <Layers/EditorValues.hpp>

class Layer 
{
public:

	virtual bool OnRender();
	virtual bool EndFrame();
	virtual bool OnImGuiRender();

	virtual void OnAttach();
	virtual bool OnDetach();
	virtual bool OnShutdown();

protected:
	bool myIsOpen = true;
};