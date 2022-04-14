#include "pch.h"
#include "NetworkPanel.h"

#include <imgui/imgui.h>

namespace GamerEngine
{
	void NetworkPanel::OnImGuiRender()
	{
		ImGui::Begin("Network Browser");

		if (ImGui::Button("Host", ImVec2(16, 8)))
		{
			// Host
		}

		if (ImGui::Button("Join", ImVec2(16, 8)))
		{
			// Join
		}

		ImGui::Checkbox("Open to public", &myIsOpenPublicly);

		ImGui::End();
	}
}
