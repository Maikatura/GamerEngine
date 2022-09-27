#pragma once
#include <Layers/Layer.h>

#include "Math/MathTypes.hpp"

struct EditorSetting
{
	Vector4f myClearColor;

	bool myUseConsole;
};

class EditorSettingsPanel : public Layer
{



public:

	EditorSettingsPanel();

	void OnAttach() override;
	bool OnImGuiRender() override;

    static void LoadConfig();
	static void SaveConfig();

private:


	inline static EditorSetting mySettings;
	inline static const std::string myConfigPath = "Editor\\Settings\\Editor\\EditorConfig.data";
	bool myIsOpen = true;
};
