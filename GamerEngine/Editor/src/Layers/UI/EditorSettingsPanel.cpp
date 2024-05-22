#include "Editor.pch.h"
#include <fstream>
#include <ImGui/imgui.h>
#include <Layers/UI/EditorSettingsPanel.h>
#include <rapidjson/JsonReader.hpp>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <GraphicsEngine.h>
#include "Time.hpp"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Core/RenderING/Renderer.h"
#include <imgui_markdown/imgui_markdown.h>
#include "Scene/SceneManager.h"
#include <Layers/LayerHeader.h>

ImFont* H1 = NULL;
ImFont* H2 = NULL;
ImFont* H3 = NULL;

#ifdef UNICODE
#undef GetObject
#endif

ImGui::MarkdownConfig mdConfig;



EditorSettingsPanel::EditorSettingsPanel() : Layer("Editor Settings", false, true, "Info")
{
	
}

void EditorSettingsPanel::OnAttach()
{
	LoadConfig();

	

	Layer::OnAttach();
}

void EditorSettingsPanel::OnImGuiRender()
{

	

	if(!myIsOpen)
	{
		return;
	}
	
	BeginMenu();
	

	std::vector<std::string> menuNames = { "Editor", "Scene" };
#if _DEBUG
	menuNames.push_back("Debug");
#endif

	ImGui::TabMaker("SettingsPageMenu", menuNames, &myCurrentTab);

	if (myCurrentTab == 0)
	{
		RenderEditorSettings();
	}

	if (myCurrentTab == 1)
	{
		RenderSceneSettings();
	}

#if _DEBUG
	if (myCurrentTab == 2)
	{
		RenderDebugSettings();
	}
#endif

	EndMenu();

}

void EditorSettingsPanel::OnUpdate()
{

	



	Layer::OnUpdate();
}

bool EditorSettingsPanel::OnDetach()
{
	SaveConfig();
	
	return Layer::OnDetach();
}

void EditorSettingsPanel::LoadConfig()
{

#pragma region ImGui Style 1

	//EditorColorScheme::ApplyTheme();
	

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



	auto& settings = GraphicsEngine::Get()->GetEngineSettings();


	const std::string path = myConfigPath + "EditorConfig.data";

	if (!std::filesystem::exists(path))
	{
		SaveConfig();
	}
	rapidjson::Document document = JsonReader::ReadJson(path.c_str());
	
	// mySettings.myBlendValue = document["BlendValue"].GetFloat();
	// LoadVector4File(document["PresetPath1"].GetString(), mySettings.myBlendColor1);
	// LoadVector4File(document["PresetPath2"].GetString(), mySettings.myBlendColor2);

	settings.CameraPos = LoadVector3<float>(document,"CameraPosition");
	settings.CameraRot = LoadVector3<float>(document, "CameraRotation");


	settings.myIsBlending = document["IsBlending"].GetBool();

	if(document.HasMember("DebugConsole"))
	{
		settings.myUseConsole = document["DebugConsole"].GetBool();
	}

	if (settings.myIsBlending)
	{
		GamerEngine::Renderer::SetClearColor(settings.myCurrentBlendColor);
	}
	else
	{
		GamerEngine::Renderer::SetClearColor(settings.myClearColor);
	}


}

void EditorSettingsPanel::SaveConfig()
{
	std::ofstream stream(myConfigPath + "EditorConfig.data");
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::Writer writer(osw);



	writer.StartObject();

	auto& settings = GraphicsEngine::Get()->GetEngineSettings();
	

	std::string useConsole("DebugConsole");
	writer.String(useConsole.c_str(), static_cast<rapidjson::SizeType>(useConsole.length()));
	writer.Bool(settings.myUseConsole);


	std::string isBlending("IsBlending");
	writer.String(isBlending.c_str(), static_cast<rapidjson::SizeType>(isBlending.length()));
	writer.Bool(settings.myIsBlending);

	std::string blendValue("BlendValue");
	writer.String(blendValue.c_str(), static_cast<rapidjson::SizeType>(blendValue.length()));
	writer.Double(settings.myBlendValue);

	auto transform = GamerEngine::Renderer::GetCameraTransform();
	if (transform)
	{
		WriteVector3(writer, "CameraPosition", transform->GetPosition());
		WriteVector3(writer, "CameraRotation", transform->GetRotation());
	}


	
	if(settings.PresetPath1.empty())
	{
		settings.PresetPath1 = "FallbackName1";
	}

	std::string pathPres1("PresetPath1");
	writer.String(pathPres1.c_str(), static_cast<rapidjson::SizeType>(pathPres1.length()));
	std::string filePath1 = myConfigPath + "Presets\\" + settings.PresetPath1 + ".preset";
	writer.String(filePath1.c_str(), static_cast<rapidjson::SizeType>(filePath1.length()));

	if (settings.PresetPath2.empty())
	{
		settings.PresetPath2 = "FallbackName2";
	}

	std::string pathPres2("PresetPath2");
	writer.String(pathPres2.c_str(), static_cast<rapidjson::SizeType>(pathPres2.length()));
	std::string filePath2 = myConfigPath + "Presets\\" + settings.PresetPath2 + ".preset";
	writer.String(filePath2.c_str(), static_cast<rapidjson::SizeType>(filePath2.length()));

	writer.EndObject();

	stream.close();
}

void EditorSettingsPanel::SaveVector4File(const std::string& aPath, Vector4f aColor)
{

	std::ofstream stream(myConfigPath + "Presets\\" + aPath + ".preset");
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

	writer.StartObject();

	std::string x("x");
	writer.String(x.c_str(), static_cast<rapidjson::SizeType>(x.length()));
	writer.Double(aColor.x);

	std::string y("y");
	writer.String(y.c_str(), static_cast<rapidjson::SizeType>(y.length()));
	writer.Double(aColor.y);

	std::string z("z");
	writer.String(z.c_str(), static_cast<rapidjson::SizeType>(z.length()));
	writer.Double(aColor.z);

	std::string w("w");
	writer.String(w.c_str(), static_cast<rapidjson::SizeType>(w.length()));
	writer.Double(aColor.w);
	

	writer.EndObject();

	stream.close();
}

void EditorSettingsPanel::SaveVector3File(const std::string& aPath, Vector3f aColor)
{
	std::ofstream stream(myConfigPath + "Presets\\" + aPath + ".preset");
	rapidjson::OStreamWrapper osw(stream);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);

	writer.StartObject();

	std::string x("x");
	writer.String(x.c_str(), static_cast<rapidjson::SizeType>(x.length()));
	writer.Double(aColor.x);

	std::string y("y");
	writer.String(y.c_str(), static_cast<rapidjson::SizeType>(y.length()));
	writer.Double(aColor.y);

	std::string z("z");
	writer.String(z.c_str(), static_cast<rapidjson::SizeType>(z.length()));
	writer.Double(aColor.z);



	writer.EndObject();

	stream.close();
}

void EditorSettingsPanel::LoadVector4File(const std::string& aPath, Vector4f& aColor)
{
	std::string path = aPath;
	auto document = JsonReader::ReadJson(path.c_str());

	aColor.x = document["x"].GetFloat();
	aColor.y = document["y"].GetFloat();
	aColor.z = document["z"].GetFloat();
	aColor.w = document["w"].GetFloat();
}

void EditorSettingsPanel::LoadVector3File(const std::string& aPath, Vector3f& aColor)
{
	std::string path = aPath;
	auto document = JsonReader::ReadJson(path.c_str());
	
	aColor.x = document["x"].GetFloat();
	aColor.y = document["y"].GetFloat();
	aColor.z = document["z"].GetFloat();
}

void EditorSettingsPanel::WriteVector4(rapidjson::Writer<rapidjson::OStreamWrapper>& aWriter,const std::string& aName, Vector4f aVector4)
{
	aWriter.Key(aName.c_str());
	aWriter.StartObject();
	aWriter.Key("x");
	aWriter.Double(aVector4.x);
	aWriter.Key("y");
	aWriter.Double(aVector4.y);
	aWriter.Key("z");
	aWriter.Double(aVector4.z);
	aWriter.Key("w");
	aWriter.Double(aVector4.w);
	aWriter.EndObject();
}

void EditorSettingsPanel::WriteVector3(rapidjson::Writer<rapidjson::OStreamWrapper>& aWriter,const std::string& aName, Vector3f aVector3)
{
	aWriter.Key(aName.c_str());
	aWriter.StartObject();
	aWriter.Key("x");
	aWriter.Double(aVector3.x);
	aWriter.Key("y");
	aWriter.Double(aVector3.y);
	aWriter.Key("z");
	aWriter.Double(aVector3.z);
	aWriter.EndObject();
}
template<typename T>
Vector4<T> EditorSettingsPanel::LoadVector4(rapidjson::Document& aDocument,const std::string& aName)
{
	if (!aDocument.HasMember(aName.c_str()))
	{
		return Vector4<T>();
	}

	auto document = aDocument[aName.c_str()].GetObject();
	
	Vector4<T> vector4;
	
	vector4.x = document["x"].GetFloat();
	vector4.y = document["y"].GetFloat();
	vector4.z = document["z"].GetFloat();
	vector4.w = document["w"].GetFloat();

	return vector4;
}

template<typename T>
Vector3<T> EditorSettingsPanel::LoadVector3(rapidjson::Document& aDocument,const std::string& aName)
{
	if (!aDocument.HasMember(aName.c_str()))
	{
		return Vector3<T>();
	}

	Vector3<T> vector3;

	auto document = aDocument[aName.c_str()].GetObject();
	
	vector3.x = document["x"].Get<T>();
	vector3.y = document["y"].Get<T>();
	vector3.z = document["z"].Get<T>();

	return vector3;
}


void EditorSettingsPanel::RenderEditorSettings()
{
	ImGui::SeparateWithSpacing();

	ImGui::TextWrapped("Save and load scene");
	if(ImGui::Button("Save Scene"))
	{
		SceneManager::Get().SaveScene("Assets\\Scenes\\scenetesting.csf");
	}

	if(ImGui::Button("Load Scene"))
	{
		SceneManager::Get().LoadScene("Assets\\Scenes\\scenetesting.csf");
	}


}

void EditorSettingsPanel::RenderSceneSettings()
{
	ImGui::SeparateWithSpacing();


	auto& settings = GraphicsEngine::Get()->GetEngineSettings();


	ImGui::ColorEdit4("Clear Color1", &settings.myClearColor.x);
	ImGui::NewLine();

	ImGui::InputText("File Name 1", &settings.PresetPath1);
	if(ImGui::Button("Load If Preset 1 Exist"))
	{
		std::string pathPres = myConfigPath + "Presets\\" + settings.PresetPath1 + ".preset";
		LoadVector4File(pathPres, settings.myBlendColor1);
	}
	ImGui::ColorEdit4("Clear Color2", &settings.myBlendColor1.x);
	ImGui::InputText("File Name 2", &settings.PresetPath2);
	ImGui::ColorEdit4("Clear Color3", &settings.myBlendColor2.x);
	if(ImGui::Button("Load If Preset 2 Exist"))
	{
		std::string pathPres = myConfigPath + "Presets\\" + settings.PresetPath2 + ".preset";
		LoadVector4File(pathPres, settings.myBlendColor2);
	}

	if(settings.myIsBlending)
	{
		settings.myCurrentBlendColor = Vector4f::Blend(settings.myBlendColor1, settings.myBlendColor2, settings.myBlendValue);
		GamerEngine::Renderer::SetClearColor(settings.myCurrentBlendColor);
	}
	else
	{
		GamerEngine::Renderer::SetClearColor(settings.myClearColor);
	}




	ImGui::Checkbox("Blend", &settings.myIsBlending);


	ImGui::SliderFloat("Blend Value", &settings.myBlendValue, 0.0f, 1.0f);

	Vector4f value = settings.myCurrentBlendColor;

	ImGui::ColorButton("Blend Color", ImVec4(value.x, value.y, value.z, value.w), 0, { 64, 64 });

	ImGui::SeparateWithSpacing();

	if(ImGui::ButtonCenter("Load", { 128, 32 }))
	{
		LoadConfig();
	}

	if(ImGui::ButtonCenter("Save", { 128, 32 }))
	{
		SaveConfig();
		SaveVector4File(settings.PresetPath1, settings.myBlendColor1);
		SaveVector4File(settings.PresetPath2, settings.myBlendColor2);
	}
}

void EditorSettingsPanel::RenderDebugSettings()
{
	ImGui::SeparateWithSpacing();

	std::string fps = "FPS: " + std::to_string(1.0f / Time::GetDeltaTime());
	std::string delta = "Delta Time: " + std::to_string(Time::GetDeltaTime());
	ImGui::Text(fps.c_str());
	ImGui::Text(delta.c_str());


	//	std::string markdownText;
	//	markdownText += "\r\n# H1 Header: Text and Links\r\nYou can add";
	//	//markdownText +=  u8R"[links like this one to enkisoftware](https://www.enkisoftware.com/) and lines will wrap well.\r\nYou can also insert images ![image alt text](image identifier e.g. filename)\r\nHorizontal rules:\r\n***\r\n___\r\n*Emphasis* and **strong emphasis** change the appearance of the text.\r\n## H2 Header: indented text.\r\n  This text has an indent (two leading spaces).\r\n    This one has two.\r\n### H3 Header: Lists\r\n  * Unordered lists\r\n    * Lists can be indented with two extra spaces.\r\n  * Lists can have [links like this one to Avoyd](https://www.avoyd.com/) and *emphasized text*\r\n";
	//
	//	mdConfig.headingFormats[0] = { H1, true };
	//	mdConfig.headingFormats[1] = { H2, true };
	//	mdConfig.headingFormats[2] = { H3, false };
	//
	//	ImGui::Markdown(markdownText.c_str(), markdownText.length(), mdConfig);
}
