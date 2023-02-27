#include "Editor.pch.h"
#include "KeybindShortcutsLayer.h"
#include <filesystem>
#include "Renderer/GraphicsEngine.h"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Handlers/FileDialog.h"
#include "Renderer/Input/Input.h"
#include "Renderer/Scene/Scene.h"
#include "Renderer/Scene/SceneManager.h"
#include "Renderer/Scene/SceneSerializer.h"


KeybindShortcutsLayer::KeybindShortcutsLayer()
{
	Layer::OnAttach();
}

bool KeybindShortcutsLayer::OnImGuiRender()
{
	{
		if(Input::IsKeyDown(VK_CONTROL) && Input::IsKeyPressed('S') && !Input::IsMouseDown(VK_LBUTTON))
		{
			std::string path = FileDialog::SaveFile("Scene File (*.csf)\0*.csf\0");
			SceneManager::SaveScene(path + ".csf");
			ConsoleHelper::Log(LogType::Info, std::string("Saved scene to '" + path + "'"));
		}
	}
	return true;
}

void KeybindShortcutsLayer::OnUpdate()
{
	{
		if(Input::IsKeyPressed(VK_F6))
		{
			unsigned int currentRenderMode = static_cast<unsigned int>(GraphicsEngine::Get()->GetRenderMode());
			currentRenderMode++;
			if(currentRenderMode == static_cast<unsigned char>(RenderMode::COUNT))
			{
				currentRenderMode = 0;
			}

			std::cout<< "Render Mode: " << currentRenderMode << "\n";

			GraphicsEngine::Get()->SetRenderMode(static_cast<RenderMode>(currentRenderMode));
		}
	}
}
