#pragma once
#include <Layers/EditorValues.hpp>

class Layer 
{
public:
	Layer(const std::string& aLayerName, bool aAlwaysOpen = false, bool aSaveLayerSettings = true, const std::string& aCategory = "");
	virtual ~Layer() = default;

	virtual void OnBeginFrame();
	virtual bool OnRender();
	virtual bool EndFrame();
	virtual void OnImGuiRender();


	virtual void OnUpdate();

	virtual void OnAttach();
	virtual bool OnDetach();
	virtual bool OnShutdown();

	bool ShouldBeSaved();

	std::string GetCategory();
	std::string GetLayerName();
	void SetOpen(bool aOpenState);
	bool IsOpen();
	bool HasBeenAdded();

	bool BeginMenu(ImGuiWindowFlags aFlag = 0);
	void EndMenu();

protected:
	bool mySaveLayer = true;
	bool myIsAlwaysOpen = false;
	bool myIsOpen = true;
	bool hasInited = false;
	std::string myLayerName;
	std::string myCategory;
};