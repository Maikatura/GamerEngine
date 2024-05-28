#include "Editor.pch.h"

#include <fstream>
#include <Layers/EditorLayers.h>
#include <iostream>
#include <GraphicsEngine.h>
#include <Core/Framework/DX11.h>
#include "ImGuizmo/ImGuizmo.h"
#include <Layers/LayerHeader.h>
#include "Scene/Scene.h"
#include <Fonts/IconsForkAwesome.h>
#include "Debugger/ConsoleHelper.h"
//#include "Profiler/Profiler.h"


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
	ImGui_ImplDX11_Init(DX11::Get().Get().GetDevice(), DX11::Get().GetContext());

	ImGui::LoadIniSettingsFromDisk((myImGuiSettingsFile + "EditorLayout.data").c_str());

	io.Fonts->AddFontDefault();

	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
	static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FK, 13.0f, &config, icon_ranges);

	AddDefaultLayers();

	YAML::Node data;
	try
	{
		data = YAML::LoadFile("Editor\\Settings\\Layouts\\EditorLayout.layout");
	}
	catch(YAML::ParserException e)
	{
		return;
	}

	auto windows = data["Windows"];
	for (int i = 0; i < myLayers.size(); i++)
	{
		if (windows)
		{
			for(auto window : windows)
			{
				LoadLayout(window, myLayers[i]);
			}
		}
	}
}

template<typename T, typename... Args>
void EditorLayers::AddLayer(Args&&... args)
{
	Ref<Layer> layer = MakeRef<T>(std::forward<Args>(args)...);

	bool windowExist = false;
	for (int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->GetLayerName() == layer->GetLayerName())
		{
			myLayers[i]->SetOpen(true);
			windowExist = true;
		}
	}

	if (!windowExist)
	{
		layer->SetLayers(this);
		myLayers.push_back(layer);
	}
}


void EditorLayers::Destory()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	out << YAML::Key << "Windows" << YAML::Value << YAML::BeginSeq;
	for(int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->ShouldBeSaved())
		{
			out << YAML::BeginMap; // Window
			out << YAML::Key << "Name" << YAML::Value << myLayers[i]->GetLayerName();
			out << YAML::Key << "IsOpen" << YAML::Value << myLayers[i]->IsOpen();
			out << YAML::EndMap; // Window
		}
	}

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout("Editor\\Settings\\Layouts\\EditorLayout.layout");
	fout << out.c_str();
	fout.close();

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
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();

	
	//PROFILE_FRAME();
	

	for (int i = 0; i < myLayers.size(); i++)
	{
		if (!myLayers[i]->HasBeenAdded())
		{
			myLayers[i]->OnBeginFrame();
		}
	}

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

void EditorLayers::LoadLayout(YAML::Node window, std::vector<Ref<Layer>>::const_reference aLayer)
{
	if(window["Name"].as<std::string>() == aLayer->GetLayerName())
	{
		aLayer->SetOpen(window["IsOpen"].as<bool>());
	}
}

bool EditorLayers::ShouldRunEngine()
{
	return myShouldRunEngine;
}

void EditorLayers::SetShouldEngineRun(bool aCond)
{
	myShouldRunEngine = aCond;
}

void EditorLayers::AddDefaultLayers()
{
	AddLayer<MainMenuBar>();
	AddLayer<ConsoleWindow>();
	AddLayer<FileExplorer>();
	AddLayer<Hierarchy>();
	AddLayer<Inspector>();
	AddLayer<AnimatorLayer>();
	AddLayer<SceneView>();
	AddLayer<KeybindShortcutsLayer>();
	AddLayer<NetworkingLayer>();
	AddLayer<HelpPanel>();
	AddLayer<ProfilerLayer>(myShouldProfile);
	AddLayer<TextureBrowserLayer>();
	AddLayer<EditorSettingsPanel>();
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
	auto renderView = DX11::Get().GetRenderTargetView();
	DX11::Get().GetContext()->OMSetRenderTargets(1, &renderView, NULL);

	for(int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->HasBeenAdded() && myLayers[i]->IsOpen())
		{
			myLayers[i]->OnImGuiRender();
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

