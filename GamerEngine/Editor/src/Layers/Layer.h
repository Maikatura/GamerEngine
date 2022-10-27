#pragma once
#include <Layers/EditorValues.hpp>

class Layer 
{
public:
	virtual ~Layer() = default;

	virtual bool OnRender();
	virtual bool EndFrame();
	virtual bool OnImGuiRender();

	virtual void OnUpdate();

	virtual void OnAttach();
	virtual bool OnDetach();
	virtual bool OnShutdown();

protected:
	bool myIsOpen = true;
};