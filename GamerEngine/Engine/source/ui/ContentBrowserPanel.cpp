#include "pch.h"
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace GamerEngine
{
	ContentBrowserPanel::ContentBrowserPanel() : myAssetPath("Assets")
	{
		myCurrentDirectory = myAssetPath;
		myFolderIcon.LoadTexture(	"resources/icons/Icon_Directory.png");
		myModelIcon.LoadTexture(	"resources/icons/Icon_File.png");
	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::Begin("Content Browser", (bool*)1, flags);

		if (myCurrentDirectory != myAssetPath)
		{
			if (ImGui::Button("<-"))
			{
				myCurrentDirectory = myCurrentDirectory.parent_path();
			}
		}

		myIconPadding = myIconSize / 8;
		const float cellSize = myIconSize + myIconPadding;
		const float width = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(width / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 10,10});
		ImGui::PopStyleVar();
		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
		{
			auto relativePath = std::filesystem::relative(directoryEntry.path(), myAssetPath);
			std::string relativePathString = relativePath.filename().string();

			auto iconTexture = myModelIcon.GetRenderID();

			if (directoryEntry.is_directory())
				iconTexture = myFolderIcon.GetRenderID();
			else
				iconTexture = GetIconID(relativePath.extension().string());
			
			ImGui::ImageButton(reinterpret_cast<void*>(iconTexture), { myIconSize, myIconSize });
			if (directoryEntry.is_directory())
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					myCurrentDirectory /= directoryEntry.path().filename();

			ImGui::Text(relativePathString.c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::End();
	}

	unsigned ContentBrowserPanel::GetIconID(const std::string& aExtension)
	{
		if (aExtension == ".fbx")		return myModelIcon.GetRenderID(); // Model File
		if (aExtension == ".obj")		return myModelIcon.GetRenderID(); // Model File
		if (aExtension == ".shader")	return myModelIcon.GetRenderID(); // Shader File
		if (aExtension == ".cs")		return myModelIcon.GetRenderID(); // C# Code File

		return myModelIcon.GetRenderID();
	}
}

