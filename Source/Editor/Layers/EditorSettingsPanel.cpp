#include <Layers/EditorSettingsPanel.h>
#include <ImGui/imgui.h>
#include "Windows.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <rapidjson/JsonReader.hpp>

#include <GraphicsEngine.h>

#include "Time.hpp"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"

EditorSettingsPanel::EditorSettingsPanel()
{
	OnAttach();
}

void EditorSettingsPanel::OnAttach()
{
	
}

bool EditorSettingsPanel::OnImGuiRender()
{
	if(!myIsOpen)
	{
		return false;
	}


	ImGui::Begin(EditorNames::SettingsName.c_str(), &myIsOpen);

	std::string fps = "FPS: " + std::to_string(1.0f / Time::GetDeltaTime());
	std::string delta = "Delta Time: " + std::to_string(Time::GetDeltaTime());
	ImGui::Text(fps.c_str());
	ImGui::Text(delta.c_str());

	mySettings.myClearColor = GraphicsEngine::Get()->GetClearColor();
	ImGui::ColorEdit4("Clear Color", &mySettings.myClearColor.x);
	GraphicsEngine::Get()->SetColor(mySettings.myClearColor);

	if(ImGui::Button("Load"))
	{
		LoadConfig();
		GraphicsEngine::Get()->SetColor(mySettings.myClearColor);
	}

	ImGui::SameLine();

	if(ImGui::Button("Save"))
	{
		SaveConfig();
	}

	ImGui::NewLine();
	ImGui::TextWrapped("Save and load scene");
	if (ImGui::Button("Save Scene"))
	{
		SceneSerializer test(GraphicsEngine::Get()->GetScene().get());
		test.Serialize("Assets\\Scenes\\scenetesting.csf");
	}

	if(ImGui::Button("Load Scene"))
	{

		GraphicsEngine::Get()->GetScene()->Clear();
		GraphicsEngine::Get()->GetScene() = std::make_shared<Scene>();
		SceneSerializer test(GraphicsEngine::Get()->GetScene().get());
		test.Deserialize("Assets\\Scenes\\scenetesting.csf");
	}

	ImGui::End();

	return true;
}

void EditorSettingsPanel::LoadConfig()
{

	#pragma region ImGui Style 1

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.GrabRounding = style.FrameRounding = 2.3f;
	
	#pragma endregion

	auto document = JsonReader::ReadJson(myConfigPath.c_str());

	mySettings.myClearColor.x = document["x"].GetFloat();
	mySettings.myClearColor.y = document["y"].GetFloat();
	mySettings.myClearColor.z = document["z"].GetFloat();
	mySettings.myClearColor.w = document["w"].GetFloat();

	if(document.HasMember("DebugConsole"))
	{
		mySettings.myUseConsole = document["DebugConsole"].GetBool();
	}

	GraphicsEngine::Get()->SetColor(mySettings.myClearColor);
}

void EditorSettingsPanel::SaveConfig()
{
	std::ofstream stream(myConfigPath.c_str());
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

	writer.StartObject();

	std::string x("x");
	writer.String(x.c_str(), static_cast<rapidjson::SizeType>(x.length()));
	writer.Double(mySettings.myClearColor.x);

	std::string y("y");
	writer.String(y.c_str(), static_cast<rapidjson::SizeType>(y.length()));
	writer.Double(mySettings.myClearColor.y);

	std::string z("z");
	writer.String(z.c_str(), static_cast<rapidjson::SizeType>(z.length()));
	writer.Double(mySettings.myClearColor.z);

	std::string w("w");
	writer.String(w.c_str(), static_cast<rapidjson::SizeType>(w.length()));
	writer.Double(mySettings.myClearColor.w);

	std::string useConsole("DebugConsole");
	writer.String(useConsole.c_str(), static_cast<rapidjson::SizeType>(useConsole.length()));
	writer.Bool(mySettings.myUseConsole);

	writer.EndObject();

	stream.close();
}
