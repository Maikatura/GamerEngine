#include "pch.h"
#include "ContentBrowserPanel.h"
#include <imgui.h>

namespace GamerEngine
{
ContentBrowserPanel::ContentBrowserPanel() : myAssetPath("Assets")
{
	myCurrentDirectory = myAssetPath;
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Content Browser");

	auto relativePath = std::filesystem::relative(myAssetPath);

	if (myCurrentDirectory != myAssetPath)
	{
		if(ImGui::Button("<-"))
		{
			myCurrentDirectory = myCurrentDirectory.parent_path();
		}
	}

	for(auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
	{

		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, myAssetPath);
		std::string relativePathString = relativePath.filename().string();

		if (directoryEntry.is_directory())
		{
			if (ImGui::Button(relativePathString.c_str()))
			{
				myCurrentDirectory /= path.filename();
			}
		}
		else
		{
			if (ImGui::Button(relativePathString.c_str()))
			{
			}
		}


	}


	ImGui::End();
}
}