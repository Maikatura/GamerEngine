#include "Hierarchy.h"

#include <filesystem>
#include <ImGui/imgui.h>
#include <GraphicsEngine.h>
#include <ImGui/imgui_stdlib.h>
#include <StringCast.h>
#include <Scene/Scene.h>
#include <iostream>
#include <Model/ModelInstance.h>

#include "Components/Components.hpp"
#include <Model/Entity.h>

#include "AssetHandlers/ModelAssetHandler.h"
#include "Components/CameraController.h"
#include "Render/SelectionData.h"
#include "Scene/SceneSerializer.h"
#include "Types/FileExtensions.h"

Entity mySelectedEntity;

bool Hierarchy::OnImGuiRender()
{
	/*auto models = GraphicsEngine::Get()->GetScene()->CullModels();*/

	ImGui::Begin(EditorNames::HierarchyName.c_str(), &myIsOpen);

	
	DropItem();
	DrawWindowPopupMenu();

	GraphicsEngine::Get()->GetScene()->GetRegistry().each([&](auto entityID)
		{
			Entity entity{ entityID, GraphicsEngine::Get()->GetScene().get() };
			DrawEntityNode(entity);
		});

	ImGui::End();

	if (!myIsOpen)
	{
		return false;
	}

	return true;
}

void Hierarchy::DropItem()
{
	auto testing = ImGui::GetDragDropPayload();

	if (!ImGui::IsWindowHovered())
	{
		return;
	}

	if(!testing)
	{
		return;
	}

	if(!testing->IsDataType("CONTENT_BROWSER_ITEM"))
	{
		return;
	}

	std::wstring wPath = static_cast<const wchar_t*>(testing->Data);
	auto stringPath = Helpers::string_cast<std::string>(wPath);
	std::filesystem::path path = stringPath;
	auto extension = path.extension().string();

	if(fbxExt == extension)
	{
		auto& modelComp = GraphicsEngine::Get()->GetScene()->CreateEntity("").AddComponent<ModelComponent>();
		modelComp.myModel = ModelAssetHandler::GetModelInstance(wPath);
	}

	if (sceneExt == extension)
	{
		GraphicsEngine::Get()->GetScene()->Clear();
		GraphicsEngine::Get()->GetScene() = std::make_shared<Scene>();
		SceneSerializer test(GraphicsEngine::Get()->GetScene().get());
		test.Deserialize(stringPath);
	}


	ImGui::SetDragDropPayload("", nullptr, 0);
}

void Hierarchy::DrawEntityNode(Entity aEntity)
{
	auto& tag = aEntity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = ((mySelectedEntity == aEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str());
	if(ImGui::IsItemClicked())
	{
		mySelectedEntity = aEntity;
		SelectionData::SetSelectedObject<entt::entity>(FileType::SceneObject, (void*)mySelectedEntity.GetHandle());
	}

	
	bool entityDeleted = false;
	if(ImGui::BeginPopupContextItem())
	{
		if(ImGui::MenuItem("Delete Entity"))
		{
			entityDeleted = true;
		}
		ImGui::EndPopup();
	}

	if(opened)
	{
		//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//bool opened = ImGui::TreeNodeEx(std::to_string(aEntity.GetUUID()).c_str(), flags, tag.c_str());
		//if(ImGui::IsItemClicked())
		//{
		//	mySelectedEntity = &aEntity;
		//	//SelectionData::SetSelectedObject<entt::entity>(FileType::SceneObject, (void*)mySelectedEntity->GetHandle());

		//}

		//if(opened)
		//	ImGui::TreePop();

		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		GraphicsEngine::Get()->GetScene()->DeleteEntity(aEntity);
	}
	
}

void Hierarchy::DrawEntityPopupMenu(std::shared_ptr<ModelInstance> aModel)
{
	if(ImGui::BeginPopupContextItem())
	{
		if(ImGui::MenuItem("Delete"))
		{
			
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
			GraphicsEngine::Get()->GetScene()->CreateEntity("");
		}

		if(ImGui::MenuItem("Create Debug Camera"))
		{
			auto entity = GraphicsEngine::Get()->GetScene()->CreateEntity("SceneCamera");
			entity.AddComponent<CameraComponent>();
			entity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		}

		ImGui::EndPopup();
	}
}
