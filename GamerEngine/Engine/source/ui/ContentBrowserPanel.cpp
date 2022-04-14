#include "pch.h"
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace GamerEngine
{
	ContentBrowserPanel::ContentBrowserPanel() : myAssetPath("Assets")
	{
		myCurrentDirectory = myAssetPath;
		myFolder = Texture("resourses/folderIcon.png");

	}

	ContentBrowserPanel::~ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		

		ImGui::Begin("Content Browser");

		if (myCurrentDirectory != myAssetPath)
		{
			if (ImGui::Button("<-"))
			{
				myCurrentDirectory = myCurrentDirectory.parent_path();
			}
		}

		const float cellSize = myIconSize + myIconPadding;
		const float width = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(width / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
		{
			auto relativePath = std::filesystem::relative(directoryEntry.path(), myAssetPath);
			std::string relativePathString = relativePath.filename().string();

			//uint64_t id = myFolder.GetRenderID();
			ImGui::ImageButton(reinterpret_cast<void*>(myFolder.GetRenderID()), { myIconSize, myIconSize });

			if (directoryEntry.is_directory())
			{
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					myCurrentDirectory /= directoryEntry.path().filename();
				}
			}
			else
			{
				// TODO : Setup so you can open files
			}

			ImGui::NextColumn();
		}

		ImGui::Columns(1);
		ImGui::End();
	}
}