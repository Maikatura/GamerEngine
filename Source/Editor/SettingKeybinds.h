#pragma once

enum class EditorKeybinds
{
	TRANSLATE,
	ROTATE,
	SCALE,

};


class EditorSettingsPanel
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

private:
};