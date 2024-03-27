#include "Editor.pch.h"
#include "KeybindShortcutsLayer.h"
#include <filesystem>
#include "Debugger/ConsoleHelper.h"
#include "Handlers/FileDialog.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"


KeybindShortcutsLayer::KeybindShortcutsLayer() : Layer("Keybinds", true, false)
{
	Layer::OnAttach();
}

void KeybindShortcutsLayer::OnImGuiRender()
{
	{
		if(Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('S') && !Input::IsMouseDown(VK_LBUTTON))
		{
			const std::string path = FileDialog::SaveFile("Scene File (*.csf)\0*.csf\0");
			if (!path.empty())
			{
				SceneManager::Get().SaveScene(path + ".csf");
				ConsoleHelper::Log(LogType::Info, "Saved scene to '%s'", path.c_str());	
			}
			
			
		}
	}
}

void KeybindShortcutsLayer::OnUpdate()
{
	{
		/*if(Input::IsKeyPressed(VK_F6))
		{
			unsigned int currentRenderMode = static_cast<unsigned int>(GraphicsEngine::Get()->GetRenderMode());
			currentRenderMode++;
			if(currentRenderMode == static_cast<unsigned char>(RenderMode::COUNT))
			{
				currentRenderMode = 0;
			}

			std::cout<< "Render Mode: " << currentRenderMode << "\n";

			GraphicsEngine::Get()->SetRenderMode(static_cast<RenderMode>(currentRenderMode));
		}*/
	}
}
