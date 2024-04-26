#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>
#include <string>

#include <yaml-cpp/yaml.h>

#include "Utilites/Pointers.h"


class EditorLayers
{
	std::vector<Ref<Layer>> myLayers;

public:
	EditorLayers();
	~EditorLayers();

	void Init();


	template<typename T, typename... Args>
	void AddLayer(Args&&... args);

	void Destory();


	void OnUpdate();
	void OnAttach();
	void OnDetach();
	void OnImGuiRender();

	void BeginFrame();
	void EndFrame();

	void LoadLayout(YAML::Node window, std::vector<Ref<Layer>>::const_reference layer);
	bool ShouldRunEngine();
	void SetShouldEngineRun(bool aCond);

private:

	friend class MainMenuBar;

	bool myShouldRunEngine = false;

	std::string myImGuiSettingsFile = "Editor\\Settings\\Layouts\\";

	void AddDefaultLayers();

	bool myShouldProfile = false;

};
