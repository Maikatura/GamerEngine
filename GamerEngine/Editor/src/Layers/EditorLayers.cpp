#include "Editor.pch.h"

#include <fstream>
#include <Layers/EditorLayers.h>
#include <iostream>
#include <GraphicsEngine.h>
#include <Framework/DX11.h>
#include "ImGuizmo/ImGuizmo.h"
#include <Layers/LayerHeader.h>
#include "Scene/Scene.h"
#include <Fonts/IconsForkAwesome.h>
#include "Debugger/ConsoleHelper.h"
#include "Profiler/Profiler.h"


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
	ImGui_ImplDX11_Init(DX11::GetDevice(), DX11::GetContext());

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

void EditorLayers::AddLayer(std::shared_ptr<Layer> aLayer)
{

	bool windowExist = false;
	for(int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->GetLayerName() == aLayer->GetLayerName())
		{
			myLayers[i]->SetOpen(true);
			windowExist = true;
		}
	}

	if(!windowExist)
	{
		myLayers.push_back(aLayer);
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

	
	PROFILE_FRAME();
	

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

void EditorLayers::LoadLayout(YAML::Node window, std::vector<std::shared_ptr<Layer>>::const_reference aLayer)
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
	AddLayer(std::make_shared<MainMenuBar>(*this));
	AddLayer(std::make_shared<ConsoleWindow>());
	AddLayer(std::make_shared<FileExplorer>());
	AddLayer(std::make_shared<Hierarchy>());
	AddLayer(std::make_shared<Inspector>());
	AddLayer(std::make_shared<AnimatorLayer>());
	AddLayer(std::make_shared<EditorView>());
	AddLayer(std::make_shared<KeybindShortcutsLayer>());
	AddLayer(std::make_shared<NetworkingLayer>());
	AddLayer(std::make_shared<HelpPanel>());
	AddLayer(std::make_shared<ProfilerLayer>(myShouldProfile));
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
	DX11::GetContext()->OMSetRenderTargets(1, &DX11::myRenderTargetView, NULL);

	for(int i = 0; i < myLayers.size(); i++)
	{
		if (myLayers[i]->HasBeenAdded() && myLayers[i]->IsOpen())
		{
			myLayers[i]->OnImGuiRender();
			

			/*if(!myLayers[i]->OnImGuiRender())
			{
				ConsoleHelper::Log(LogType::Info, "Removed a layer from editor");
				myLayers[i].swap(myLayers[myLayers.size() - 1]);
				myLayers[myLayers.size() - 1]->OnDetach();

				myLayers.pop_back();
			}*/
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

