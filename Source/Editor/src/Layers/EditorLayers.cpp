#include "Editor.pch.h"
#include <Layers/EditorLayers.h>
#include <iostream>
#include <Renderer/GraphicsEngine.h>
#include <Renderer/Framework/DX11.h>
#include "ImGuizmo/ImGuizmo.h"
#include <Layers/LayerHeader.h>
#include "Renderer/Scene/Scene.h"
#include <Fonts/IconsForkAwesome.h>

#include "Renderer/Debugger/ConsoleHelper.h"

EditorLayers::EditorLayers()
{}

EditorLayers::~EditorLayers()
{

}

void EditorLayers::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.IniFilename = NULL;

	//ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(GraphicsEngine::Get()->GetWindowHandle());
	ImGui_ImplDX11_Init(DX11::Device.Get(), DX11::Context.Get());

	ImGui::LoadIniSettingsFromDisk((myImGuiSettingsFile + "EditorLayout.data").c_str());

	io.Fonts->AddFontDefault();

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FK, 13.0f, &config, icon_ranges);

	

	AddDefaultLayers();
}

void EditorLayers::AddLayer(std::shared_ptr<Layer> aLayer)
{
	myLayers.push_back(aLayer);
}

void EditorLayers::Destory()
{
	OnDetach();
	ImGui::SaveIniSettingsToDisk((myImGuiSettingsFile + "EditorLayout.data").c_str());

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EditorLayers::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	

	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();

	OnAttach();
}

void EditorLayers::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

bool EditorLayers::ShouldRunEngine()
{
	return myShouldRunEngine;
}

void EditorLayers::AddDefaultLayers()
{
	AddLayer(std::make_shared<MainMenuBar>(*this));
	AddLayer(std::make_shared<ConsoleWindow>());
	AddLayer(std::make_shared<FileExplorer>());
	AddLayer(std::make_shared<Hierarchy>());
	AddLayer(std::make_shared<Inspector>());
	AddLayer(std::make_shared<AnimatorLayer>());
	AddLayer(std::make_shared<EditorView>());
	AddLayer(std::make_shared<KeybindShortcutsLayer>());
}

void EditorLayers::OnUpdate()
{
	for(int i = 0; i < myLayers.size(); i++)
	{
		if(myLayers[i]->HasBeenAdded())
		{
			myLayers[i]->OnUpdate();
		}
	}
}

void EditorLayers::OnAttach()
{
	for(int i = 0; i < myLayers.size(); i++)
	{
		if (!myLayers[i]->HasBeenAdded())
		{
			myLayers[i]->OnAttach();
		}
	}
}

void EditorLayers::OnImGuiRender()
{
	DX11::Context->OMSetRenderTargets(1, DX11::BackBuffer.GetAddressOf(), NULL);

	for(int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->HasBeenAdded())
		{
			if(!myLayers[i]->OnImGuiRender())
			{
				ConsoleHelper::Log(LogType::Info, "Removed a layer from editor");
				myLayers[i].swap(myLayers[myLayers.size() - 1]);
				myLayers[myLayers.size() - 1]->OnDetach();

				myLayers.pop_back();
			}
		}
	}
}

void EditorLayers::OnDetach()
{
	for(int i = 0; i < myLayers.size(); i++)
	{
		myLayers[i]->OnDetach();
	}
}

