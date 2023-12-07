#include <Editor.pch.h>
#include <Layers/UI/Hierarchy.h>

#include <filesystem>
#include <ImGui/imgui.h>

// Helpers
#include <Utilites/StringCast.h>
#include <Handlers/DropHandler.h>
#include <Render/SelectionData.h>

// Scene
#include <Scene/Scene.h>
#include <Model/Entity.h>

//Components
#include <Components/Components.hpp>
#include <Components/CameraController.h>
#include <Particles/ParticleEmitter.h>
#include <Model/ModelInstance.h>

#include "..\..\..\..\Game\src\Components\TransfromComponent.h"
#include "Debugger/ConsoleHelper.h"

#include <Fonts/IconsForkAwesome.h>

#include "imgui_internal.h"
#include "Scene/SceneManager.h"
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

	if(SceneManager::Get().GetScene())
	{

		auto startCursor = ImGui::GetCursorPos();


		DrawWindowPopupMenu();




		SceneManager::Get().GetScene()->GetRegistry().each([&](auto entityID)
		{
			if(entityID == entt::null)
			{
				return;
			}

			if(!SceneManager::Get().GetScene()->GetRegistry().valid(entityID))
			{
				return;
			}

			Entity entity{ entityID, SceneManager::Get().GetScene().get()};

			if(entity.GetComponent<TransformComponent>().HasParent())
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
					Ref<Entity> draggedEntity = MakeRef<Entity>(entityDataVector[i]);
					if(draggedEntity->GetComponent<TransformComponent>().HasParent())
					{
						auto parent = draggedEntity->GetComponent<TransformComponent>().GetParent();
						parent->GetComponent<TransformComponent>().RemoveChild(draggedEntity);
						draggedEntity->GetComponent<TransformComponent>().ClearParent();
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

		bool check1 = (aEntity.GetComponent<TransformComponent>().myChildren.size() == 0);
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
		flags |= ((aEntity.GetComponent<TransformComponent>().myChildren.size() == 0)) ? ImGuiTreeNodeFlags_Leaf : 0;
	}

	flags |= ((aEntity.GetComponent<TransformComponent>().myChildren.size() == 0) && !aEntity.HasComponent<ModelComponent>()) ? ImGuiTreeNodeFlags_Leaf : 0;
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

		for(int i = 0; i < aEntity.GetComponent<TransformComponent>().myChildren.size(); i++)
		{
			Entity entity{ aEntity.GetComponent<TransformComponent>().myChildren[i]->GetHandle(), SceneManager::Get().GetScene().get() };
			DrawEntityNode(entity);
		}
		ImGui::TreePop();
	}

	if(entityDeleted)
	{
		SceneManager::Get().GetScene()->DeleteEntity(aEntity);
	}


}

void Hierarchy::DrawEntityPopupMenu(Ref<ModelInstance> aModel)
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
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Debug Camera"))
		{
			createIndex = 1;
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Particle Emitter"))
		{
			createIndex = 2;
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Directional Light"))
		{
			createIndex = 3;
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Point Light"))
		{
			createIndex = 4;
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Spot Light"))
		{
			createIndex = 5;
			entity = SceneManager::Get().CreateEntityType(createIndex);
		}

		if(ImGui::MenuItem("Create Network Manager"))
		{
			createIndex = 6;
			entity = SceneManager::Get().CreateEntityType(createIndex);
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

void Hierarchy::DrawArmature(Entity& aEntity, Ref<ModelInstance> aModel)
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

		for(int i = 0; i < aModel->GetNumMeshes(); i++)
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

bool Hierarchy::LoopThoughChildren(Ref<Entity> aEntity)
{
	for(Ref<Entity> ent : aEntity->GetComponent<TransformComponent>().myChildren)
	{
		if(ent->GetID() == aEntity->GetID())
		{
			return true;
		}
		else if(ent->GetComponent<TransformComponent>().myChildren.size() > 0)
		{
			return LoopThoughChildren(ent);
		}
	}

	return false;
}

void Hierarchy::CheckIfUserWantToSetParent(Entity& aEntity)
{

	Ref<Entity> entity = MakeRef<Entity>(aEntity);

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
				Ref<Entity> aChildEntity = MakeRef<Entity>(aChildEntityList[i]);

				bool shouldExit = false;
				if(!entity->GetComponent<TransformComponent>().HasParent() && !aChildEntity->GetComponent<TransformComponent>().HasParent())
				{

					entity->GetComponent<TransformComponent>().SetChild(aChildEntity);
					aChildEntity->GetComponent<TransformComponent>().SetParent(entity);
					hasMoved = true;
				}
				else if(entity->GetComponent<TransformComponent>().HasParent() && !aChildEntity->GetComponent<TransformComponent>().HasParent())
				{

					Ref<Entity> entityParent = entity;
					bool done = false;
					while(!done)
					{
						if(entityParent->GetID() == aChildEntity->GetID())
						{
							done = true;
							shouldExit = true;
							std::cout << entityParent->GetComponent<TagComponent>().Tag << std::endl;
						}

						if(entityParent->GetComponent<TransformComponent>().HasParent())
						{
							entityParent = entityParent->GetComponent<TransformComponent>().GetParent();
						}
						else
						{
							done = true;
						}

					}

					if(!shouldExit)
					{
						shouldExit = LoopThoughChildren(entity);
					}

					if(shouldExit) return;

					entity->GetComponent<TransformComponent>().SetChild(aChildEntity);
					aChildEntity->GetComponent<TransformComponent>().SetParent(entity);
					hasMoved = true;
				}
				else if(!entity->GetComponent<TransformComponent>().HasParent() && aChildEntity->GetComponent<TransformComponent>().HasParent())
				{
					auto parent = aChildEntity->GetComponent<TransformComponent>().GetParent();
					parent->GetComponent<TransformComponent>().RemoveChild(aChildEntity);

					entity->GetComponent<TransformComponent>().SetChild(aChildEntity);
					aChildEntity->GetComponent<TransformComponent>().SetParent(entity);
					hasMoved = true;
				}
				else if(entity->GetComponent<TransformComponent>().HasParent() && aChildEntity->GetComponent<TransformComponent>().HasParent())
				{

					Ref<Entity> entityParent = entity;


					bool done = false;
					while(!done)
					{
						if(entityParent->GetID() == aChildEntity->GetID())
						{
							done = true;
							shouldExit = true;
							std::cout << entityParent->GetComponent<TagComponent>().Tag << std::endl;
						}

						if(entityParent->GetComponent<TransformComponent>().HasParent())
						{
							entityParent = entityParent->GetComponent<TransformComponent>().GetParent();
						}
						else
						{
							done = true;
						}

					}

					if(!shouldExit)
					{
						shouldExit = LoopThoughChildren(entity);
					}

					if(shouldExit)
					{
						return;
					}

					auto parent = aChildEntity->GetComponent<TransformComponent>().GetParent();
					parent->GetComponent<TransformComponent>().RemoveChild(aChildEntity);

					entity->GetComponent<TransformComponent>().SetChild(aChildEntity);
					aChildEntity->GetComponent<TransformComponent>().SetParent(entity);
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
