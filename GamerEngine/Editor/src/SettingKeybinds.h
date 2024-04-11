#pragma once

enum class EditorKeybinds
{
	TRANSLATE,
	ROTATE,
	SCALE,

};




class SettingKeybinds
{
public:
	static ImGuizmo::MODE GetEditMode()
	{
		return myMode;
	}

	static void SetEditModeType(ImGuizmo::MODE aMode)
	{
		myMode = aMode;
	}

	static std::string GetEditModeName()
	{
		return (ImGuizmo::MODE::LOCAL == myMode) ? "Local" : "World";
	}

private:





	inline static ImGuizmo::MODE myMode;

};