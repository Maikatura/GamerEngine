#pragma once
#include <Layers/EditorValues.hpp>

class Layer 
{
public:
	Layer(const std::string& aLayerName, bool aAlwaysOpen = false, bool aSaveLayerSettings = true);
	virtual ~Layer() = default;

	virtual bool OnRender();
	virtual bool EndFrame();
	virtual bool OnImGuiRender();

	virtual void OnUpdate();

	virtual void OnAttach();
	virtual bool OnDetach();
	virtual bool OnShutdown();

	bool HasBeenAdded();

	bool BeginMenu(bool* isOpen = 0, ImGuiWindowFlags aFlag = 0);
	void EndMenu();


protected:
	bool mySaveLayer = true;
	bool myIsAlwaysOpen = false;
	bool myIsOpen = true;
	bool hasInited = false;
	std::string myLayerName;
};