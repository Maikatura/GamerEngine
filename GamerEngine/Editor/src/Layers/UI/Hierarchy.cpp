#include <Editor.pch.h>
#include <Layers/UI/Hierarchy.h>

#include <filesystem>
#include <ImGui/imgui.h>

// Helpers
#include <StringCast.h>
#include <Handlers/DropHandler.h>
#include <Renderer/Render/SelectionData.h>

// Scene
#include <Renderer/Scene/Scene.h>
#include <Renderer/Model/Entity.h>

//Components
#include <Components/Components.hpp>
#include <Components/CameraController.h>
#include <Renderer/Particles/ParticleEmitter.h>
#include <Renderer/Model/ModelInstance.h>

#include "Components/ChildComponent.h"
#include "Renderer/Debugger/ConsoleHelper.h"

#include <Fonts/IconsForkAwesome.h>

#include "imgui_internal.h"
#include "Renderer/Scene/SceneManager.h"
#include "Layers/NetworkingLayer.h"
#include "Layers/Network/CreateObjectMessage.h"
#include <Data/HashTable.hpp>

std::vector<Entity> mySelectedEntity;

Hierarchy::Hierarchy() : Layer("Hierarchy")
{}

void Hierarchy::OnImGuiRender()
{
	/*auto models = GraphicsEngine::Get()->GetScene()->CullModels();*/

	BeginMenu();

	if(SceneManager::GetScene())
	{

		auto startCursor = ImGui::GetCursorPos();


		DrawWindowPopupMenu();




		SceneManager::GetScene()->GetRegistry().each([&](auto entityID)
		{
			if(entityID == entt::null)
			{
				return;
			}

			if(!SceneManager::GetScene()->GetRegistry().valid(entityID))
			{
				return;
			}

			Entity entity{ entityID, SceneManager::GetScene().get()};

			if(entity.GetComponent<ChildComponent>().HasParent())
			{
				return;
			}

			DrawEntityNode(entity);
		});

		if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		{
			SelectionData::SetEntityObject({ entt::null, nullptr });

			if(mySelectedEntity.size() >= 1)
			{
				mySelectedEntity.clear();
			}
		}

		auto size = ImGui::GetWindowSize();

		auto endCursor = ImGui::GetCursorPos();

		ImRect dropRect = ImGui::GetCurrentWindow()->WorkRect;
		ImGuiID id = 131023012;
		if(ImGui::BeginDragDropTargetCustom(dropRect, id))
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SceneHierarchy");
			if(payload != nullptr)
			{
				std::vector<Entity>& entityDataVector = *static_cast<std::vector<Entity>*>(payload->Data);
				entityDataVector.resize(payload->DataSize / sizeof(Entity));

				for(size_t i = 0; i < entityDataVector.size(); i++)
				{
					Entity draggedEntity = entityDataVector[i];
					if(draggedEntity.GetComponent<ChildComponent>().HasParent())
					{
						auto parent = draggedEntity.GetComponent<ChildComponent>().GetParent();
						parent.GetComponent<ChildComponent>().RemoveChild(draggedEntity);
						draggedEntity.GetComponent<ChildComponent>().ClearParent();
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		DropHandler::DropFileScene(dropRect, static_cast<ImGuiID>(1238278903));
	}

	EndMenu();

	myEntityList.clear();
}

void Hierarchy::DrawEntityNode(Entity& aEntity)
{
	for(size_t i = 0; i < myEntityList.size(); i++)
	{
		if(aEntity.GetID() == myEntityList[i])
		{
			return;
		}
	}

	if(aEntity.GetComponent<TagComponent>().Tag == "EditorCamera") return;

	ImGuiTreeNodeFlags flags = 0;
	for(size_t i = 0; i < mySelectedEntity.size(); i++)
	{
		flags |= ((mySelectedEntity[i] == aEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	}

	if(aEntity.HasComponent<ModelComponent>())
	{

		bool check1 = (aEntity.GetComponent<ChildComponent>().myChildren.size() == 0);
		bool check2 = false;


		auto& modelComp = aEntity.GetComponent<ModelComponent>();

		bool checkingModel = (modelComp.GetModel() != nullptr);

		if(checkingModel)
		{
			check2 = (modelComp.GetModel()->GetSkeleton()->GetRoot() == nullptr) && (modelComp.GetModel()->GetNumMeshes() <= 0);
		}


		flags |= (check1 && check2) ? ImGuiTreeNodeFlags_Leaf : 0;
	}
	else
	{
		flags |= ((aEntity.GetComponent<ChildComponent>().myChildren.size() == 0)) ? ImGuiTreeNodeFlags_Leaf : 0;
	}

	flags |= ((aEntity.GetComponent<ChildComponent>().myChildren.size() == 0) && !aEntity.HasComponent<ModelComponent>()) ? ImGuiTreeNodeFlags_Leaf : 0;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;



	auto tag = "%s " + aEntity.GetComponent<TagComponent>().Tag;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str(), ICON_FK_CUBE);

	DropHandler::DropFileEntity(aEntity);
	CheckIfUserWantToSetParent(aEntity);

	if(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsItemClicked())
	{
		mySelectedEntity.push_back(aEntity);
		if(mySelectedEntity.size() > 0)
		{
			SelectionData::SetEntityObject(mySelectedEntity[0]);
		}
	}
	else if(ImGui::IsItemClicked() && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{

		if(mySelectedEntity.size() <= 1)
		{
			mySelectedEntity.clear();
			mySelectedEntity.push_back(aEntity);
			SelectionData::SetEntityObject(mySelectedEntity[0]);
		}
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
		if(aEntity.HasComponent<ModelComponent>())
		{
			if(aEntity.GetComponent<ModelComponent>().GetModel())
			{
				DrawArmature(aEntity, aEntity.GetComponent<ModelComponent>().GetModel());
			}
		}

		for(int i = 0; i < aEntity.GetComponent<ChildComponent>().myChildren.size(); i++)
		{
			Entity entity{ aEntity.GetComponent<ChildComponent>().myChildren[i], SceneManager::GetScene().get() };
			DrawEntityNode(entity);
		}
		ImGui::TreePop();
	}

	if(entityDeleted)
	{
		SceneManager::GetScene()->DeleteEntity(aEntity);
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
		int createIndex = -1;

		Entity entity;

		if(ImGui::MenuItem("Create"))
		{
			createIndex = 0;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Debug Camera"))
		{
			createIndex = 1;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Particle Emitter"))
		{
			createIndex = 2;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Directional Light"))
		{
			createIndex = 3;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Point Light"))
		{
			createIndex = 4;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Spot Light"))
		{
			createIndex = 5;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Network Manager"))
		{
			createIndex = 6;
			entity = SceneManager::CreateEntityType(createIndex);
		}

		auto& client = NetworkingLayer::GetClient();
		if(client.GetStatus().Status == TurNet::Status::Connected && createIndex != -1)
		{
			ObjectCreateMessage createMsg;
			createMsg.EntityID = entity.GetUUID();
			createMsg.EntityType = createIndex;

			TurNet::TurMessage outMsg;
			outMsg << createMsg;

			client.SendToServer(outMsg);
		}

		ImGui::EndPopup();
	}
}

void Hierarchy::DrawArmature(Entity& aEntity, std::shared_ptr<ModelInstance> aModel)
{
	if(aModel->GetSkeleton())
	{
		auto root = aModel->GetSkeleton()->GetRoot();
		if(root)
		{
			std::string armatureText = "Armature";
			uint32_t name = CommonUtilities::HashString(armatureText);

			ImGuiTreeNodeFlags flags = 0;
			flags |= ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)name, flags, armatureText.c_str());
			if(opened)
			{
				LoopBones(aModel->GetSkeleton(), root);
				ImGui::TreePop();
			}

		}

		for(int i = 0; i < aModel->GetModel()->GetNumMeshes(); i++)
		{
			uint32_t name = CommonUtilities::HashString(aModel->GetMeshData(i).myMeshName);
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)name, ImGuiTreeNodeFlags_Leaf, aModel->GetMeshData(i).myMeshName.c_str());

			if(ImGui::IsItemClicked() && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
			{
				if(mySelectedEntity.size() <= 1)
				{
					mySelectedEntity.clear();
					mySelectedEntity.push_back(aEntity);
					SelectionData::SetEntityObject(mySelectedEntity[0]);
				}
			}

			if(opened)
			{
				ImGui::TreePop();
			}
		}

	}



}

void Hierarchy::LoopBones(const Skeleton* aSkeleton, const Bone* aBone, unsigned int aBoneID)
{
	const std::string& nameID = aSkeleton->BoneNames[aBoneID];

	uint32_t name = CommonUtilities::HashString(nameID + std::to_string(aBoneID));

	ImGuiTreeNodeFlags flags = 0;
	flags |= (aBone->Children.size() != 0) ? ImGuiTreeNodeFlags_OpenOnArrow : 0;
	flags |= (aBone->Children.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)name, flags, nameID.c_str());

	if(opened)
	{
		for(int i = 0; i < aBone->Children.size(); i++)
		{
			LoopBones(aSkeleton, &aSkeleton->Bones[aBone->Children[i]], aBone->Children[i]);
		}

		ImGui::TreePop();
	}
}

bool Hierarchy::LoopThoughChildren(Entity& aEntity)
{
	for(Entity ent : aEntity.GetComponent<ChildComponent>().myChildren)
	{
		if(ent.GetID() == aEntity.GetID())
		{
			return true;
		}
		else if(ent.GetComponent<ChildComponent>().myChildren.size() > 0)
		{
			return LoopThoughChildren(ent);
		}
	}

	return false;
}

void Hierarchy::CheckIfUserWantToSetParent(Entity& aEntity)
{

	if(ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("SceneHierarchy", static_cast<void*>(&mySelectedEntity), sizeof(Entity) * mySelectedEntity.size());

		for(size_t i = 0; i < mySelectedEntity.size(); i++)
		{
			ImGui::Text(mySelectedEntity[i].GetComponent<TagComponent>().Tag.c_str());
		}

		ImGui::EndDragDropSource();
	}

	if(ImGui::BeginDragDropTarget())
	{
		auto payload = ImGui::AcceptDragDropPayload("SceneHierarchy");
		bool hasMoved = false;
		if(payload != nullptr)
		{
			std::vector<Entity>& entityDataVector = *static_cast<std::vector<Entity>*>(payload->Data);
			entityDataVector.resize(payload->DataSize / sizeof(Entity));

			std::vector<Entity> aChildEntityList;

			for(size_t i = 0; i < entityDataVector.size(); i++)
			{
				aChildEntityList.push_back(entityDataVector.at(i));
			}

			for(int i = 0; i < aChildEntityList.size(); i++)
			{
				Entity aChildEntity = aChildEntityList[i];

				bool shouldExit = false;
				if(!aEntity.GetComponent<ChildComponent>().HasParent() && !aChildEntity.GetComponent<ChildComponent>().HasParent())
				{

					aEntity.GetComponent<ChildComponent>().SetChild(aChildEntity);
					aChildEntity.GetComponent<ChildComponent>().SetParent(aEntity);
					hasMoved = true;
				}
				else if(aEntity.GetComponent<ChildComponent>().HasParent() && !aChildEntity.GetComponent<ChildComponent>().HasParent())
				{

					Entity entityParent = aEntity;
					bool done = false;
					while(!done)
					{
						if(entityParent.GetID() == aChildEntity.GetID())
						{
							done = true;
							shouldExit = true;
							std::cout << entityParent.GetComponent<TagComponent>().Tag << std::endl;
						}

						if(entityParent.GetComponent<ChildComponent>().HasParent())
						{
							entityParent = entityParent.GetComponent<ChildComponent>().GetParent();
						}
						else
						{
							done = true;
						}

					}

					if(!shouldExit)
					{
						shouldExit = LoopThoughChildren(aEntity);
					}

					if(shouldExit) return;

					aEntity.GetComponent<ChildComponent>().SetChild(aChildEntity);
					aChildEntity.GetComponent<ChildComponent>().SetParent(aEntity);
					hasMoved = true;
				}
				else if(!aEntity.GetComponent<ChildComponent>().HasParent() && aChildEntity.GetComponent<ChildComponent>().HasParent())
				{
					auto parent = aChildEntity.GetComponent<ChildComponent>().GetParent();
					parent.GetComponent<ChildComponent>().RemoveChild(aChildEntity);

					aEntity.GetComponent<ChildComponent>().SetChild(aChildEntity);
					aChildEntity.GetComponent<ChildComponent>().SetParent(aEntity);
					hasMoved = true;
				}
				else if(aEntity.GetComponent<ChildComponent>().HasParent() && aChildEntity.GetComponent<ChildComponent>().HasParent())
				{

					Entity entityParent = aEntity;


					bool done = false;
					while(!done)
					{
						if(entityParent.GetID() == aChildEntity.GetID())
						{
							done = true;
							shouldExit = true;
							std::cout << entityParent.GetComponent<TagComponent>().Tag << std::endl;
						}

						if(entityParent.GetComponent<ChildComponent>().HasParent())
						{
							entityParent = entityParent.GetComponent<ChildComponent>().GetParent();
						}
						else
						{
							done = true;
						}

					}

					if(!shouldExit)
					{
						shouldExit = LoopThoughChildren(aEntity);
					}

					if(shouldExit)
					{
						return;
					}

					auto parent = aChildEntity.GetComponent<ChildComponent>().GetParent();
					parent.GetComponent<ChildComponent>().RemoveChild(aChildEntity);

					aEntity.GetComponent<ChildComponent>().SetChild(aChildEntity);
					aChildEntity.GetComponent<ChildComponent>().SetParent(aEntity);
					hasMoved = true;
				}
			}

		}

		if(hasMoved == true)
		{
			mySelectedEntity.clear();
			ImGui::SetDragDropPayload("", nullptr, 0);
		}
		ImGui::EndDragDropTarget();
	}
}

void Hierarchy::DropAFile()
{

}

void Hierarchy::IsItemHovered(Entity& aEntity)
{
	aEntity;
}
