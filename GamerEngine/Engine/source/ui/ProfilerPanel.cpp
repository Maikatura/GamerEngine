#include "pch.h"
#include "ProfilerPanel.h"
#include <imgui/imgui.h>

namespace GamerEngine
{
	void ProfilerPanel::OnImGuiRender()
	{
		ImGui::Begin("Profiler");

		ImGui::Text("Total Allocation: %s", myAllocation.myAllocated);
		ImGui::Text("Total Freed: %s", myAllocation.myFreed);
		ImGui::Text("Current Allocation: %s", myAllocation.GetCurrent());

		ImGui::End();

	}
}
