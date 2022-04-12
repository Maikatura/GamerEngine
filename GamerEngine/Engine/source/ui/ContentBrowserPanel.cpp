#include "pch.h"
#include "ContentBrowserPanel.h"
#include <imgui.h>


ContentBrowserPanel::ContentBrowserPanel() : myFilePath("Assets")
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::OnImGuiRender()
{
	ImGui::Begin("Content Browser");

	auto relativePath = std::filesystem::relative(myFilePath);

	for(auto& itPath : relativePath)
	{
		ImGui::Button(itPath.filename().string().c_str());
	}


	ImGui::End();
}
