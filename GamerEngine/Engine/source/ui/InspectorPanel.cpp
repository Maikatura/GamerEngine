#include "pch.h"
#include "InspectorPanel.h"
#include "SceneView.h"
#include "utils/imgui_widgets.h"
#include "utils/EngineContext.h"
#include "ECS/Entity.h"
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

//Components
#include <Components/TransformComponent.h>
#include <Components/TagComponent.h>

namespace GamerEngine
{
	
	void InspectorPanel::OnImGuiRender(GamerEngine::EngineContext* aContext)
	{
		ImGui::Begin("Inspector");

		auto entity = aContext->GetSelectedEntity();

		if (entity.IsNull())
		{
			ImGui::End();
			return;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			
			auto& tag = entity.GetComponent<TagComponent>();
			
		

			ImGui::Text("Tag: "); 
			ImGui::SameLine();
			ImGui::InputText("##Tag", &tag.myTag);

			auto& transform = entity.GetComponent<TransformComponent>();

			if (ImGui::CollapsingHeader("Transform"))
			{
				GamerEngine::DrawImGuiTransform("Pos", transform.myPosition);
				GamerEngine::DrawImGuiTransform("Rot",	transform.myRotation);
				GamerEngine::DrawImGuiTransform("Scale", transform.myScale);
			}

		}

		if (entity.HasComponent<MeshComponent>()) 
		{
			auto& mesh = entity.GetComponent<MeshComponent>();

			
			float* myFloats[] = {
				&mesh.myColor.x,
				&mesh.myColor.y,
				&mesh.myColor.z,
				&mesh.myColor.w
			};

			if (ImGui::CollapsingHeader("Mesh"))
			{
				ImGui::ColorEdit4("Color: ", *myFloats);
			}
		}
			

		ImGui::End();
	}

}