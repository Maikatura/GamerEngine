#include "Inspector.h"
#include <ImGui/imgui.h>
#include <GraphicsEngine.h>
#include <Imgui/ImGuizmo.h>
#include <DirectXMath.h>
#include "Model/ModelInstance.h"
#include <Scene/Scene.h>
#include <ImGui/imgui_stdlib.h>

#include "Audio/Audio.h"

bool Inspector::OnImGuiRender()
{
	std::shared_ptr<SelectedObejct> selectedObjectData = GraphicsEngine::Get()->GetScene()->GetSelectedObject();
	ModelInstance* selectedObject = selectedObjectData->GetType<ModelInstance>();
	

	ImGui::Begin(EditorNames::PropertiesName.c_str());


	if (selectedObjectData->FileType == FileType::Model)
	{
		DrawSceneObject();
	}
	else
	{
		DrawFileObject();
	}

	ImGui::End();

	return true;
}

void Inspector::DrawSceneObject()
{
	auto selectedObjectData = GraphicsEngine::Get()->GetScene()->GetSelectedObject();
	auto selectedObject = selectedObjectData->GetType<ModelInstance>();

	auto translate = selectedObject->GetTransform().GetWorldMatrix().GetPosition();
	auto rotation = selectedObject->GetTransform().GetRotation();
	auto scale = selectedObject->GetTransform().GetScale();

	ImGui::InputText("Name", &selectedObject->Name);

	if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
	{
		ImGui::DragFloat3("Position", &translate.x, 0.25f);
		ImGui::DragFloat3("Rotation", &rotation.x, 0.25f);
		ImGui::DragFloat3("Scale", &scale.x, 0.05f);

		ImGui::TreePop();
	}


	selectedObject->GetTransform().ComposeTransform(
		translate,
		rotation,
		scale);
}

#pragma optimize( "", off )
void Inspector::DrawFileObject()
{
	std::shared_ptr<SelectedObejct> selectedObjectData = GraphicsEngine::Get()->GetScene()->GetSelectedObject();

	switch(selectedObjectData->FileType)
	{
		case FileType::Audio:
		{
			std::string audio = ((const char*)selectedObjectData->FileData);

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

		default:
		{
			ImGui::Text("No Custom Inspector for this object type yet");
			break;
		}
	}
}
#pragma optimize( "", on ) 
