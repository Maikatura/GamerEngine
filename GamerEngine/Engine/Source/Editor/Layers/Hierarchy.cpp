#include "Hierarchy.h"
#include <ImGui/imgui.h>
#include <GraphicsEngine.h>
#include <ImGui/imgui_stdlib.h>
#include <StringCast.h>
#include <Scene/Scene.h>
#include <iostream>
#include <Model/ModelInstance.h>

bool Hierarchy::OnImGuiRender()
{
	auto models = GraphicsEngine::Get()->GetScene()->CullModels(NULL);

	ImGui::Begin(EditorNames::HierarchyName.c_str());

	DrawWindowPopupMenu();

	for(size_t i = 0; i < models.size(); i++)
	{
		std::shared_ptr<ModelInstance> model = models[i];
		DrawEntity(model, i);
		DragAndDrop(model);
		DrawEntityPopupMenu(model);
	}

	ImGui::End();

	return true;
}

void Hierarchy::DrawEntity(std::shared_ptr<ModelInstance> aModel, int aDebugNumber)
{
	std::string name = aModel->Name;
	if (aModel->Name == "")
	{
		name = Helpers::string_cast<std::string>(aModel->GetModel()->GetName()) + std::to_string(aDebugNumber);
	}

	bool rootNode = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);

	if(ImGui::IsItemClicked() && !ImGui::GetDragDropPayload())
	{
		GraphicsEngine::Get()->GetScene()->SetSelectedObject<ModelInstance>( FileType::Model, (void*)aModel.get());
	}

	if(rootNode)
	{
		ImGui::TreePop();
	}
}

void Hierarchy::DragAndDrop(std::shared_ptr<ModelInstance> aModel)
{
	std::string name = Helpers::string_cast<std::string>(aModel->GetModel()->GetName());

	if(ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("DragAndDropEntity", aModel.get(), sizeof(ModelInstance));
		ImGui::Text(name.c_str());
		ImGui::EndDragDropSource();
	}

	if(ImGui::BeginDragDropTarget())
	{
		auto payload = ImGui::AcceptDragDropPayload("DragAndDropEntity");
		if(payload != nullptr)
		{
			auto dropData = ImGui::GetDragDropPayload();
			ModelInstance* selectedDragModel = (ModelInstance*)dropData->Data;
			selectedDragModel->GetTransform().SetParent(&aModel->GetTransform());
		}
		ImGui::EndDragDropTarget();
	}
}

void Hierarchy::DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel)
{
	if(ImGui::BeginPopupContextItem())
	{
		if(ImGui::MenuItem("Delete"))
		{
			GraphicsEngine::Get()->GetScene()->RemoveObject(aModel);
		}

		ImGui::EndPopup();
	}
}

void Hierarchy::DrawWindowPopupMenu()
{
	if(ImGui::BeginPopupContextWindow())
	{
		if(ImGui::MenuItem("Create"))
		{

		}

		ImGui::EndPopup();
	}
}
