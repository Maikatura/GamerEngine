#pragma once
#include <Layers/Layer.h>
#include <memory>
#include <vector>
#include <string>

#include <yaml-cpp/yaml.h>


class EditorLayers
{
	std::vector<std::shared_ptr<Layer>> myLayers;

public:
	EditorLayers();
	~EditorLayers();

	void Init();


	void AddLayer(std::shared_ptr<Layer> aLayer);

	void Destory();


	void OnUpdate();
	void OnAttach();
	void OnDetach();
	void OnImGuiRender();

	void BeginFrame();
	void EndFrame();

	void LoadLayout(YAML::Node window, std::vector<std::shared_ptr<Layer>>::const_reference layer);
	bool ShouldRunEngine();
	void SetShouldEngineRun(bool aCond);

private:

	friend class MainMenuBar;

	bool myShouldRunEngine = false;

	std::string myImGuiSettingsFile = "Editor\\Settings\\Layouts\\";

	void AddDefaultLayers();

	bool myShouldProfile = false;

};
