#include "Editor.pch.h"
#include <ImGui/imgui.h>
#include <Layers/Debug/ConsoleWindow.h>

#include "Debugger/ConsoleHelper.h"


ConsoleWindow::ConsoleWindow() : Layer("Console Window"), scrollToBottom(false)
{
}

void ConsoleWindow::OnImGuiRender()
{
	BeginMenu();

	if (ImGui::Button("Clear")) ConsoleHelper::Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::Separator();

	ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (copy) ImGui::LogToClipboard();


	auto& logs = ConsoleHelper::GetLogs();
	
	for(const auto& item : logs)
	{
		if (ImGui::Selectable(item.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns))
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				// Handle double click event here
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("%s", item.c_str());
		}
	}

	if (static_cast<int>(logs.size()) != myOldSize)
	{
		scrollToBottom = true;
		myOldSize = static_cast<int>(logs.size());
	}

	if (scrollToBottom)
		ImGui::SetScrollHereY(1.0f);

	scrollToBottom = false;
	ImGui::EndChild();
	
	EndMenu();
}
