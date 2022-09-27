#include "Inspector.h"
#include <ImGui/imgui.h>
#include <GraphicsEngine.h>
#include <Imgui/ImGuizmo.h>
#include <DirectXMath.h>
#include "Model/ModelInstance.h"
#include <Scene/Scene.h>
#include <ImGui/imgui_stdlib.h>
#include <Model/Entity.h>

#include "StringCast.h"
#include "Audio/Audio.h"
#include "Components/CameraController.h"
#include "Components/Components.hpp"
#include "Render/SelectionData.h"
#include "Imgui/imgui_stdlib.h"
#include "ImGuiAdded/ImGuiConfirm.h"

bool Inspector::OnImGuiRender()
{
	std::shared_ptr<SelectedObejct> selectedObjectData = SelectionData::GetSelectedObject();
	ModelInstance* selectedObject = selectedObjectData->GetType<ModelInstance>();

	ImGui::Begin(EditorNames::InspectorName.c_str(), &myIsOpen);

	if (selectedObjectData->FileType == FileType::SceneObject)
	{
		DrawSceneObject();
	}
	else
	{
		DrawFileObject();
	}

	ImGui::End();


	if (!myIsOpen)
	{
		return false;
	}

	return true;
}

void Inspector::DrawSceneObject()
{
	const auto selectedObjectData = SelectionData::GetSelectedObject();
	auto entityPtr = reinterpret_cast<uint32_t>(selectedObjectData->GetType<entt::entity>());
	Entity entity = { static_cast<entt::entity>(entityPtr),  GraphicsEngine::Get()->GetScene().get() };

	if(!GraphicsEngine::Get()->GetScene()->GetRegistry().valid(entity))
	{
		return;
	}

	if (entity.HasComponent<TagComponent>())
	{
		auto& tag = entity.GetComponent<TagComponent>();

		ImGui::InputText("Tag", &tag.Tag);
			if(tag.Tag == "")
			{
				tag.Tag = "Default Name";
			}
	}

	if (entity.HasComponent<TransformComponent>())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto translate = transform.Translation;
		auto rotation = transform.Rotation;
		auto scale = transform.Scale;

		if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
		{
			ImGui::DragFloat3("Position", &translate.x, 0.25f);
			ImGui::DragFloat3("Rotation", &rotation.x, 0.25f);
			ImGui::DragFloat3("Scale", &scale.x, 0.05f);

			ImGui::TreePop();

			
		}

		transform.Translation = translate;
		transform.Rotation = rotation;
		transform.Scale = scale;
	}

	ImGui::SeperateTheRightWay();

	if (entity.HasComponent<CameraComponent>())
	{
		if(ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
		{
			auto& camera = entity.GetComponent<CameraComponent>();

			ImGui::DragFloat("Field of View", &camera.myFoV);
			ImGui::DragFloat("Near Plane", &camera.myNearPlane);
			ImGui::DragFloat("Far Plane", &camera.myFarPlane);
			ImGui::Checkbox("Primary", &camera.Primary);

			camera.Initialize(camera.myFoV, camera.myNearPlane, camera.myFarPlane);

			ImGui::TreePop();
		}
	}

	if (entity.HasComponent<ModelComponent>())
	{
		if(ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_DefaultOpen, "Model"))
		{
			const auto& model = entity.GetComponent<ModelComponent>();
			std::string modelPath = Helpers::string_cast<std::string>(model.myModel->GetModel()->GetName());
			ImGui::TextWrapped(modelPath.c_str());

			ImGui::TreePop();
		}
	}

	if (entity.HasComponent<NativeScriptComponent>())
	{
		auto check = entity.GetComponent<NativeScriptComponent>().GetPtr<CameraController>();
		if(check != nullptr)
		{
			auto& controller = entity.GetComponent<NativeScriptComponent>().Get<CameraController>();

			if(ImGui::TreeNodeEx("CameraController", ImGuiTreeNodeFlags_DefaultOpen, "Camera Controller"))
			{
				ImGui::DragFloat("Speed", &controller.Speed());
				ImGui::DragFloat("Speed Shift", &controller.SpeedMul());

				ImGui::TreePop();
			}
		}
	}

	ImGui::SeperateTheRightWay();

	AddCompoent();

}

void Inspector::AddCompoent()
{
	if(ImGui::ButtonCenter("Add Component"))
	{
		std::cout << "Cant add a component rn" << std::endl;
	}
}

#pragma optimize( "", off )
void Inspector::DrawFileObject()
{
	std::shared_ptr<SelectedObejct> selectedObjectData = SelectionData::GetSelectedObject();

	switch(selectedObjectData->FileType)
	{
		case FileType::Audio:
		{
			std::string audio = (const char*)(selectedObjectData->FileData);

			if(ImGui::Button("Play"))
			{
				AudioManager::PlayAudio(audio);
			}

			ImGui::SameLine();

			if(ImGui::Button("Stop"))
			{
				AudioManager::StopAudio(audio);
			}
			break;
		}

		case FileType::Texture:
		{
			float size = ImGui::GetWindowContentRegionWidth();
			Texture* texture = ((Texture*)selectedObjectData->FileData);
			ImGui::Image(texture->GetSRV().Get(), { size ,size });
			ImGui::Text("Okay and why?");

			break;
		}

		case FileType::Scene:
		{
			ImGui::TextWrapped("This is a scene I don't know what you wanna change here?");
			ImGui::SeperateTheRightWay();
			ImGui::TextWrapped("But did you know you can drag the scene onto the hierarchy to load a scene?");
			break;
		}

		case FileType::Model:
		{
			ImGui::TextWrapped("This is a model I don't know what you wanna change here? Since there are no components (YET)");
			break;
		}

		case FileType::Text:
		{
			ImGui::TextWrapped("LMAO, A TEXT FILE xD");
			break;
		}

		case FileType::Animation:
		{
			ImGui::TextWrapped("LMAO, A ANIMATION FILE xD");
			break;
		}

		default:
		{
			ImGui::Text("No Custom Inspector for this object type yet");
			
			break;
		}
	}
}

#pragma optimize( "", on ) 
