#pragma once
#include <functional>
#include <ImGui/imgui.h>

#include "imgui/imgui_internal.h"

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

	inline void SeparateWithSpacing()
	{
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
	}

	inline bool ButtonCenter(const char* label, const ImVec2& aSize = { 0,0 }, float alignment = 0.5f)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		float addSize = (aSize.x == 0) ? 0 : (aSize.x - ImGui::CalcTextSize(label).x);
		float size = (ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f) + addSize;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * alignment;
		if(off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

		return ImGui::Button(label, aSize);
	}

	/**
	 * \brief Don't use please
	 * \param label 
	 * \param aSize 
	 * \return 
	 */
	inline bool ButtonColumnFillSpace(const char* label, ImVec2 aSize = { 0,0 })
	{
		float width = (static_cast<int>(aSize.x) == 0) ? ImGui::GetColumnWidth() : aSize.x;
		return ImGui::ButtonCenter(label, {width, aSize.y });
	}


	// Set the value to 0+ since it wants to be tab 0 and up and if value is above selected amount it defaults it to 0
	inline void TabMaker(const std::string& aIdentifier, std::vector<std::string> aListOfTabNames, int* aValueToChange, ImVec2 aSize = {0,0})
	{
		const int size = static_cast<int>(aListOfTabNames.size());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		ImGui::Columns(size, std::string("##" + aIdentifier).c_str(), false);

		for (int i = 0; i < size; i++)
		{
			if(ImGui::ButtonColumnFillSpace(aListOfTabNames[i].c_str(), aSize))
			{
				*aValueToChange = i;
			}
			ImGui::NextColumn();
		}

		if (*aValueToChange >= size || *aValueToChange < 0)
		{
			*aValueToChange = 0;
		}

		ImGui::Columns(1);
		ImGui::PopStyleVar();
	}


	

	
}
