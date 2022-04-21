#include "pch.h"
#include "HierarchyPanel.h"
#include <imgui/imgui.h>
#include "utils/EngineContext.h"

#include "Components/TransformComponent.h"
#include <Components/TagComponent.h>


namespace GamerEngine
{
	HierarchyPanel::HierarchyPanel()
	{
	}

	HierarchyPanel::~HierarchyPanel()
	{
	}

	void HierarchyPanel::OnImGuiRender(EngineContext* aContext)
	{
		ImGui::Begin("Hierarchy");

		auto view = aContext->GetSceneView()->GetScene()->GetRegistry()->view<TransformComponent>();

		for (auto entity : view)
		{
			auto aEntity = Entity(entity, aContext->GetSceneView()->GetScene());
			RenderTreeNode(aContext, aEntity);
		}

		ImGui::End();
	}

	void HierarchyPanel::RenderTreeNode(EngineContext* aContext, Entity aEntity)
	{
		ImGuiTreeNodeFlags flags = (aContext->GetSelectedEntity() == aEntity) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_OpenOnArrow;


		if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity.GetID(), flags, aEntity.GetComponent<TagComponent>().myTag.c_str()))
		{
			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked())
		{
			aContext->SetEntity(aEntity);
		}
		
	}
}
