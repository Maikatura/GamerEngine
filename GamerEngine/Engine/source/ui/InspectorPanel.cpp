#include "pch.h"
#include "InspectorPanel.h"

#include <imgui.h>

#include "utils/imgui_widgets.h"

void InspectorPanel::OnImGuiRender(nui::SceneView* scene_view)
{
	ImGui::Begin("Inspector");

	auto mesh = scene_view->get_mesh();

	if (mesh)
	{
		ImGui::Begin("Inspector");

		ImGui::Text("Transform");
		ImGui::Text("Position");
		nimgui::DrawImGuiPosition("Position", mesh->myTransform);
		ImGui::Text("Rotation");
		nimgui::DrawImGuiRotation("Rotation", mesh->myTransform);
		ImGui::Text("Scale");
		nimgui::DrawImGuiScale("Scale", mesh->myTransform);

		ImGui::End();
	}

	ImGui::End();
}
