#include "pch.h"
#include "HierarchyPanel.h"
#include <imgui/imgui.h>
#include "Components/TransformComponent.h"
#include "utils/EngineContext.h"


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

		auto view = aContext->mySceneView->GetScene()->GetRegistry()->view<TransformComponent>();

		for (auto entity : view)
		{
			RenderTreeNode(aContext, entity);
		}

		ImGui::End();
	}

	void HierarchyPanel::RenderTreeNode(EngineContext* aContext, entt::entity& aEntity)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected;



		if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, "LOVE"))
		{
			ImGui::TreePop();
		}
		if (ImGui::IsItemClicked())
		{
			//aContext->mySelectedEntity = Entity(aEntity, aContext->mySceneView->GetScene());
		}

		myId++;
	}
}
