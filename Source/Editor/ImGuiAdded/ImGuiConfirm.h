#pragma once
#include <functional>
#include <ImGui/imgui.h>

namespace ImGui
{
	inline void ImGuiCenterNextWindow() noexcept
	{
		static ImVec2 mid = { 0.5f, 0.5f };

		const auto center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, mid);
	}

	inline void Alert(const char* msg, bool* open) noexcept
	{
		if(*open)
		{
			ImGui::OpenPopup(msg);
			ImGuiCenterNextWindow();

			*open = false;
		}

		if(ImGui::BeginPopup(msg))
		{
			ImGui::Text("Alert");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25);
			if(ImGui::Button("X"))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Text(msg);

			ImGui::EndPopup();
		}
	}

	inline void Confirm(const char* popup, std::function<void()> yes, std::function<void()> no, bool& isOpen) noexcept
	{
		if(!isOpen) return;

		ImGui::OpenPopup(popup);

		if(ImGui::BeginPopup(popup))
		{
			ImGui::Text("Are you sure you want to proceed?");
			ImGui::Spacing();

			if(ImGui::Button("Yes"))
			{
				if(yes)
				{
					yes();
					isOpen = false;
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if(ImGui::Button("No"))
			{
				if(no)
				{
					no();
					isOpen = false;
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::CloseCurrentPopup();
	}

	inline void SeperateTheRightWay()
	{
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}

	inline bool ButtonCenter(const char* label, float alignment = 0.5f)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * alignment;
		if(off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

		return ImGui::Button(label);
	}
	
}
