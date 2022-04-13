#include "pch.h"
#include "NavigationPanel.h"

#include <imgui.h>

namespace GamerEngine
{
	void NavigationPanel::OnImGuiRender()
	{

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("Save"))
				{
					std::cout << "New Scene (NOT) created";
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Close"))
				{
					std::cout << "Closed the program";
				}


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("New Scene"))
				{
					std::cout << "New Scene (NOT) created";
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Component"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Credits"))
				{
					myCreditsIsOpen = !myCreditsIsOpen;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}


		if (myCreditsIsOpen)
		{
			ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking;

			ImGui::SetNextWindowSize({200, 300}, ImGuiCond_FirstUseEver);
			ImGui::Begin("Credits", &myCreditsIsOpen, flags);

			CenterText("GamerEngine By:");
			ImGui::NewLine();
			CenterText("Maikatura");
			ImGui::NewLine();
			ImGui::NewLine();

			CenterText("Attributes");
			ImGui::NewLine();
			CenterText("None");

			ImGui::End();
		}
	}

	void NavigationPanel::CenterText(std::string text)
	{
		float win_width = ImGui::GetWindowSize().x;
		float text_width = ImGui::CalcTextSize(text.c_str()).x;

		// calculate the indentation that centers the text on one line, relative
		// to window left, regardless of the `ImGuiStyleVar_WindowPadding` value
		float text_indentation = (win_width - text_width) * 0.5f;

		// if text is too long to be drawn on one line, `text_indentation` can
		// become too small or even negative, so we check a minimum indentation
		float min_indentation = 20.0f;
		if (text_indentation <= min_indentation) {
			text_indentation = min_indentation;
		}

		ImGui::SameLine(text_indentation);
		ImGui::PushTextWrapPos(win_width - text_indentation);
		ImGui::TextWrapped(text.c_str());
		ImGui::PopTextWrapPos();
	}
}
