#include <Editor.pch.h>
#include <Layers/UI/Hierarchy.h>

#include <filesystem>
#include <ImGui/imgui.h>
#include <GraphicsEngine.h>

// Helpers
#include <StringCast.h>
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

#include "Components/ChildComponent.h"
#include "Debugger/ConsoleHelper.h"


std::vector<Entity> mySelectedEntity;

bool Hierarchy::OnImGuiRender()
{
	/*auto models = GraphicsEngine::Get()->GetScene()->CullModels();*/

	ImGui::Begin(EditorNames::HierarchyName.c_str(), &myIsOpen);


	if(ImGui::IsWindowHovered())
	{
		auto payload = ImGui::GetDragDropPayload();
		if(payload != nullptr)
		{
			if(payload->IsDataType("SceneHierarchy"))
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
		}
	}


	DropHandler::DropFileScene();
	DrawWindowPopupMenu();

	GraphicsEngine::Get()->GetScene()->GetRegistry().each([&](auto entityID)
		{
			if(entityID == entt::null)
			{
				return;
			}

			Entity entity{ entityID, GraphicsEngine::Get()->GetScene().get() };

			if(entity.GetComponent<ChildComponent>().HasParent())
			{
				return;
			}

			DrawEntityNode(entity);
		});

	if(ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		SelectionData::SetEntityObject({ entt::null, nullptr });

		if (mySelectedEntity.size() > 1)
		{
			mySelectedEntity.clear();
			
		}
	}





	ImGui::End();

	myEntityList.clear();

	if(!myIsOpen)
	{
		return false;
	}



	return true;
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

	auto& tag = aEntity.GetComponent<TagComponent>().Tag;

	ImGuiTreeNodeFlags flags = 0;
	for(size_t i = 0; i < mySelectedEntity.size(); i++)
	{
		flags |= ((mySelectedEntity[i] == aEntity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	}
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;


	if(tag == "SceneCamera (DONT TOUCH)")
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
	}

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)aEntity, flags, tag.c_str());

	if(tag == "SceneCamera (DONT TOUCH)")
	{
		ImGui::PopStyleColor();
	}

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

		for(int i = 0; i < aEntity.GetComponent<ChildComponent>().myChildren.size(); i++)
		{
			Entity entity{ aEntity.GetComponent<ChildComponent>().myChildren[i], GraphicsEngine::Get()->GetScene().get() };
			DrawEntityNode(entity);
		}
		ImGui::TreePop();
	}

	if(entityDeleted)
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
			ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
			auto entity = GraphicsEngine::Get()->GetScene()->CreateEntity("Scene Camera");
			entity.AddComponent<CameraComponent>();
			entity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		}

		if(ImGui::MenuItem("Create Particle Emitter"))
		{
			ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
			auto entity = GraphicsEngine::Get()->GetScene()->CreateEntity("Particle Emitter");
			entity.AddComponent<ParticleEmitter>();
		}

		if(ImGui::MenuItem("Create Directional Light"))
		{
			ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
			auto entity = GraphicsEngine::Get()->GetScene()->CreateEntity("Directional Light");
			entity.AddComponent<DirectionalLightComponent>();
		}

		ImGui::EndPopup();
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

}
