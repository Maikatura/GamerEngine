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
	void CheckInput(EditorKeybinds aKeyInput)
	{
		switch (aKeyInput) {
			case EditorKeybinds::TRANSLATE: 
				break;

			case EditorKeybinds::ROTATE: 
				break;

			case EditorKeybinds::SCALE: 
				break;
		}
	}


	static std::string GetEditModeName()
	{
		if (ImGuizmo::MODE::LOCAL == myMode)
		{
			return "Local";
		}
		else
		{
			return "World";
		}
	}

	static ImGuizmo::MODE GetEditMode()
	{
		return myMode;
	}

	static void SetEditModeType(ImGuizmo::MODE aMode)
	{
		myMode = aMode;
	}

private:





	inline static ImGuizmo::MODE myMode;

};