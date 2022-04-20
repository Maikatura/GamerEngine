#include "pch.h"
#include "InspectorPanel.h"

#include <imgui/imgui.h>
#include <Components/TransformComponent.h>
#include "utils/imgui_widgets.h"
#include "utils/EngineContext.h"
#include "ECS/Entity.h"

namespace GamerEngine
{
	
	void InspectorPanel::OnImGuiRender(GamerEngine::EngineContext* aContext)
	{
		ImGui::Begin("Inspector");

		auto entity = aContext->mySelectedEntity;

		if (entity.IsNull())
		{
			ImGui::End();
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();

			if (ImGui::CollapsingHeader("Transform"))
			{
				GamerEngine::DrawImGuiTransform("Pos", transform.myPosition);
				GamerEngine::DrawImGuiTransform("Rot",	transform.myRotation);
				GamerEngine::DrawImGuiTransform("Scale", transform.myScale);
			}

		}

		ImGui::End();
	}

}