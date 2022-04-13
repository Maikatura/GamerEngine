#pragma once

#include "pch.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "../Transform.h"

namespace GamerEngine
{
	static void draw_vec3_widget(const std::string& label, glm::vec3& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
	}

	static void DrawImGuiTransform(const std::string& label, const std::string& aNameTag,  glm::vec3& values, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		std::string valueX = "##" + aNameTag + "X";
		std::string valueY = "##" + aNameTag + "Y";
		std::string valueZ = "##" + aNameTag + "Z";

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(65, 105, 225, 255));
		ImGui::Button("X", buttonSize);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(valueX.c_str(), &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(178, 34, 34, 255));
		ImGui::Button("Y", buttonSize);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(valueY.c_str(), &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(20, 128, 0, 255));
		ImGui::Button("Z", buttonSize);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(valueZ.c_str(), &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::Text(label.c_str());

		ImGui::PopStyleVar();

		//style.Colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.f);

	}
}

