#include "pch.h"
#include "InspectorPanel.h"

#include <imgui/imgui.h>

#include "utils/imgui_widgets.h"

namespace GamerEngine
{
	
	void InspectorPanel::OnImGuiRender(GamerEngine::SceneView* scene_view)
	{
		ImGui::Begin("Inspector");

		auto mesh = scene_view->GetMesh();

		if (mesh)
		{
			ImGui::Begin("Inspector");


			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::Text("Transform");

				GamerEngine::DrawImGuiTransform("Pos","Pos", mesh->myModel.myTransform.myPosition);
				GamerEngine::DrawImGuiTransform("Rot","Rot", mesh->myModel.myTransform.myRotation);
				GamerEngine::DrawImGuiTransform("Scale","Scale", mesh->myModel.myTransform.myScale);
				
			}

			ImGui::End();
		}

		ImGui::End();
	}

}