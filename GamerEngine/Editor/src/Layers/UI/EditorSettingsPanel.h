#pragma once
#include <Layers/Layer.h>
#include "Math/MathTypes.hpp"

struct EditorSetting
{
	Vector4f myClearColor;

	Vector4f myBlendColor1;
	Vector4f myBlendColor2;

	std::string PresetPath1;
	std::string PresetPath2;

	Vector4f myCurrentBlendColor;

	float myBlendValue = 0.0f;

	bool myIsBlending = false;
	bool myUseConsole = false;
};

class EditorSettingsPanel : public Layer
{



public:

	EditorSettingsPanel();

	void OnAttach() override;
	bool OnImGuiRender() override;

    static void LoadConfig();
	static void SaveConfig();

	static void SavePresetFile(const std::string& aPath, Vector4f aColor);
	static void LoadPreset(const std::string& aPath, Vector4f& aColor);


private:

	void RenderEditorSettings();
	void RenderSceneSettings();
	void RenderDebugSettings();

	inline static EditorSetting mySettings;
	inline static const std::string myConfigPath = "Editor\\Settings\\";

	int myCurrentTab = 0;
	bool myIsOpen = true;


};
