#include "Editor.pch.h"
#include "Inspector.h"
#include <GraphicsEngine.h>
#include <Model/Entity.h>
#include <Render/SelectionData.h>
#include <Scene/Scene.h>
#include "StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Audio/Audio.h"
#include "Components/CameraController.h"
#include "Components/Components.hpp"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Model/ModelInstance.h"
#include "Particles/ParticleEmitter.h"
#include "Light/EnvironmentLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Handlers/DropHandler.h"


bool Inspector::OnImGuiRender()
{
	ImGui::Begin(EditorNames::InspectorName.c_str(), &myIsOpen);

	Entity entity = SelectionData::GetEntityObject();
	if(GraphicsEngine::Get()->GetScene()->GetRegistry().valid(entity))
	{
		DrawSceneObject(entity);
	}
	else
	{
		DrawFileObject(entity);
	}

	ImGui::End();

	if(!myIsOpen)
	{
		return false;
	}

	return true;
}

void Inspector::DrawSceneObject(Entity& aEntity)
{
	static ImGuiInputTextFlags flagsReadOnly;
	flagsReadOnly |= ImGuiInputTextFlags_ReadOnly;

	if(aEntity.HasComponent<TagComponent>())
	{
		auto& tag = aEntity.GetComponent<TagComponent>();

		std::string tagName = tag.Tag;

		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
		if(tagName == "SceneCamera (DONT TOUCH)")
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		}

		ImGui::InputText(ICON_FK_TAG" Tag", &tag.Tag, flags);
		if(tagName == "SceneCamera (DONT TOUCH)")
		{
			ImGui::PopStyleColor();
		}

		if(tag.Tag == "")
		{
			tag.Tag = "Default Name";
		}
	}

	if(aEntity.HasComponent<TransformComponent>())
	{
		auto& transform = aEntity.GetComponent<TransformComponent>();
		auto translate = transform.Translation;
		auto rotation = transform.Rotation;
		auto scale = transform.Scale;

		if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen, "%s Transform", ICON_FK_ARROWS))
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

	ImGui::SeparateWithSpacing();

	if(aEntity.HasComponent<CameraComponent>())
	{
		if(ImGui::TreeNodeEx("CameraComponent", ImGuiTreeNodeFlags_DefaultOpen, "%s Camera Component", ICON_FK_CAMERA))
		{
			auto& camera = aEntity.GetComponent<CameraComponent>();

			ImGui::DragFloat("Field of View", &camera.myFoV);
			ImGui::DragFloat("Near Plane", &camera.myNearPlane);
			ImGui::DragFloat("Far Plane", &camera.myFarPlane);
			ImGui::Checkbox("Primary", &camera.Primary);

			camera.Initialize(camera.myFoV, camera.myNearPlane, camera.myFarPlane);

			ImGui::TreePop();
		}
	}

	if(aEntity.HasComponent<ModelComponent>())
	{
		if(ImGui::TreeNodeEx("ModelComponent", ImGuiTreeNodeFlags_DefaultOpen, "Mesh Renderer"))
		{
			auto& model = aEntity.GetComponent<ModelComponent>();

			{
				ImGui::BeginGroup();

				const std::wstring& modelName = model.GetModel()->GetModel()->GetName();
				std::string modelPath = Helpers::string_cast<std::string>(modelName);
				ImGui::InputText("Model", &modelPath, flagsReadOnly);

				auto newFile = DropHandler::DropFileEntity(aEntity);

				if(!newFile.empty())
				{
					auto modelStuff = ModelAssetHandler::GetModelInstance(newFile);
					model.SetModel(modelStuff);
					
				}

				ImGui::EndGroup();
			}

			// Albedo
			{
				ImGui::BeginGroup();

				auto albedoTex = model.GetModel()->GetMaterial()->GetAlbedoTexture();
				std::string name = (albedoTex != nullptr) ? Helpers::string_cast<std::string>(albedoTex->GetName()) : "No Texture";

				if(albedoTex != nullptr)
				{
					ImGui::Image(albedoTex->GetSRV().Get(), { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Albedo", &name, flagsReadOnly);
				}
				else
				{
					ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Albedo", &name, flagsReadOnly);
				}

				std::wstring newFile = DropHandler::DropFileEntity(aEntity);
				std::filesystem::path aNewPath = newFile;
				if(name != aNewPath.filename() && !aNewPath.filename().string().empty())
				{
					model.GetModel()->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(newFile));
				}

				ImGui::EndGroup();
			}
			// Normal
			{
				ImGui::BeginGroup();

				auto normalTexture = model.GetModel()->GetMaterial()->GetNormalTexture();
				std::string name = (normalTexture != nullptr) ? Helpers::string_cast<std::string>(normalTexture->GetName()) : "No Texture";

				if(normalTexture != nullptr)
				{
					ImGui::Image(normalTexture->GetSRV().Get(), { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Normal", &name, flagsReadOnly);
				}
				else
				{
					ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Normal", &name, flagsReadOnly);
				}

				std::wstring newFile = DropHandler::DropFileEntity(aEntity);
				std::filesystem::path aNewPath = newFile;
				if(name != aNewPath.filename() && !aNewPath.filename().string().empty())
				{
					model.GetModel()->GetMaterial()->SetNormalTexture(TextureAssetHandler::GetTexture(newFile));
				}

				ImGui::EndGroup();
			}
			// Material
			{
				ImGui::BeginGroup();



				auto materialTex = model.GetModel()->GetMaterial()->GetMaterialTexture();
				std::string name = (materialTex != nullptr) ? Helpers::string_cast<std::string>(materialTex->GetName()) : "No Texture";

				if(materialTex != nullptr)
				{
					ImGui::Image(materialTex->GetSRV().Get(), { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Material", &name, flagsReadOnly);
				}
				else
				{
					ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
					ImGui::SameLine();
					ImGui::InputText("Material", &name, flagsReadOnly);
				}

				std::wstring newFile = DropHandler::DropFileEntity(aEntity);
				std::filesystem::path aNewPath = newFile;
				if(name != aNewPath.filename() && !aNewPath.filename().string().empty())
				{
					model.GetModel()->GetMaterial()->SetMaterialTexture(TextureAssetHandler::GetTexture(newFile));
				}



				ImGui::EndGroup();
			}


			if (model.GetModel()->GetSkeleton()->GetRoot()) 
			{
				ImGui::BeginGroup();

				if(ImGui::ListBoxHeader("Animations"))
				{
					for(auto item : model.GetModel()->GetSkeleton()->Animations)
					{
						std::string item_name = Helpers::string_cast<std::string>(item.first);
						if(ImGui::Selectable(item_name.c_str()))
						{
							model.GetModel()->PlayAnimation(item.first);
						}
					}
					ImGui::ListBoxFooter();
					DropHandler::DropFileEntity(aEntity);
				}

				ImGui::EndGroup();
			}

			ImGui::TreePop();
		}
	}

	if(aEntity.HasComponent<ParticleEmitter>())
	{
		auto& particle = aEntity.GetComponent<ParticleEmitter>();

		if(ImGui::TreeNodeEx("ParticleEmitter", ImGuiTreeNodeFlags_DefaultOpen, "Particle Emitter"))
		{
			auto& emitterSettings = particle.GetEmitterSettings();
			auto& particleSettings = particle.GetSettings();

			if(ImGui::TreeNodeEx("Emitter Settings"))
			{
				int i = 0;
				for(auto& behavior : emitterSettings)
				{
					std::string name = "Behavior" + std::to_string(i++);
					if(ImGui::TreeNodeEx(name.c_str()))
					{
						ImGui::DragFloat("Min Frequency", &behavior.myMinFrequency, 0.001f);
						ImGui::DragFloat("Max Frequency", &behavior.myMaxFrequency, 0.001f);

						ImGui::DragFloat("Min Speed", &behavior.myMinSpeed);
						ImGui::DragFloat("Max Speed", &behavior.myMaxSpeed);

						ImGui::DragFloat("Min Rotation Speed", &behavior.myMinRotationSpeed);
						ImGui::DragFloat("Max Rotation Speed", &behavior.myMaxRotationSpeed);

						ImGui::DragFloat3("Min Angle", &behavior.myMinAngle.x);
						ImGui::DragFloat3("Max Angle", &behavior.myMaxAngle.x);



						ImGui::TreePop();
					}


				}

				ImGui::TreePop();
			}

			if(ImGui::TreeNodeEx("Particle Settings"))
			{
				int i = 0;
				for(auto& setting : particleSettings)
				{
					std::string name = "Setting" + std::to_string(i++);
					if(ImGui::TreeNodeEx(name.c_str()))
					{
						ImGui::DragFloat("Drag", &setting.myDrag, 0.001f);
						ImGui::DragFloat("Rotational Drag", &setting.myRotationalDrag, 0.001f);

						ImGui::DragFloat3("Velocity", &setting.myVelocity.x);
						ImGui::DragFloat3("Scale", &setting.myScale.x);

						ImGui::DragFloat3("Acceleration", &setting.myAcceleration.x);
						ImGui::ColorEdit4("Max Rotation Speed", &setting.myColor.x);

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

	if(aEntity.HasComponent<DirectionalLightComponent>())
	{
		auto& directionalLight = aEntity.GetComponent<DirectionalLightComponent>();

		if(ImGui::TreeNodeEx("DirectionalLight", ImGuiTreeNodeFlags_DefaultOpen, "%s Directional Light", ICON_FK_LIGHTBULB_O))
		{
			

			ImGui::Checkbox("Active", &directionalLight.Active);
			ImGui::DragFloat3("Direction", &directionalLight.Direction.x);
			ImGui::ColorEdit3("Light Color", &directionalLight.Color.x);
			ImGui::DragFloat("Intensity", &directionalLight.Intensity, 0.1f, 0.0f, 10.0f);



			ImGui::TreePop();
		}

	}

	if(aEntity.HasComponent<SpotLightComponent>())
	{
		if(ImGui::TreeNodeEx("Spot Light", ImGuiTreeNodeFlags_DefaultOpen, "%s Spot Light", ICON_FK_LIGHTBULB_O)) 
		{

			auto& spotLight = aEntity.GetComponent<SpotLightComponent>();

			auto light = spotLight.mySpotLight;

			auto lightData = light->GetColor();
			auto lightIntensity = light->GetIntensity();
			auto lightRange = light->GetRange();
			auto lightInnerCone = light->GetInnerCone();
			auto lightOuterCone = light->GetOuterCone();


			ImGui::ColorEdit3("Color", &lightData.x);
			ImGui::DragFloat("Intensity", &lightIntensity);
			ImGui::DragFloat("Range", &lightRange);
			ImGui::DragFloat("Inner Cone", &lightInnerCone);
			ImGui::DragFloat("Outer Cone", &lightOuterCone);

			int tempIndex = light->GetLightBufferData().ShadowMapIndex;
			ImGui::DragInt("Index", &tempIndex);

			light->SetColor({ lightData.x, lightData.y, lightData.z });
			light->SetIntensity(lightIntensity);
			light->SetRange(lightRange);
			light->SetInnerCone(lightInnerCone);
			light->SetOuterCone(lightOuterCone);
			ImGui::TreePop();
		}
	}

	if(aEntity.HasComponent<PointLightComponent>())
	{
		if(ImGui::TreeNodeEx("Point Light", ImGuiTreeNodeFlags_DefaultOpen, "%s Point Light", ICON_FK_LIGHTBULB_O))
		{

			auto& pointLight = aEntity.GetComponent<PointLightComponent>();

			auto light = pointLight.myPointLight;

			auto lightData = light->GetColor();
			auto lightIntensity = light->GetIntensity();
			auto lightRange = light->GetRange();

			ImGui::ColorEdit3("Color", &lightData.x);
			ImGui::DragFloat("Intensity", &lightIntensity);
			ImGui::DragFloat("Range", &lightRange);

			int tempIndex = light->GetLightBufferData().ShadowMapIndex;
			ImGui::DragInt("Index", &tempIndex);

			light->SetColor({ lightData.x, lightData.y, lightData.z });
			light->SetIntensity(lightIntensity);
			light->SetRange(lightRange);
			ImGui::TreePop();
		}
	}

	if(aEntity.HasComponent<NativeScriptComponent>())
	{
		auto check = aEntity.GetComponent<NativeScriptComponent>().Instance;
		if(check != nullptr)
		{
			/*auto& controller = aEntity.GetComponent<NativeScriptComponent>().Get<CameraController>();

			if(ImGui::TreeNodeEx("CameraController", ImGuiTreeNodeFlags_DefaultOpen, "%s Camera Controller", ICON_FK_CODE))
			{
				float speed = controller.GetSpeed();
				float speedMul = controller.GetSpeedMul();
				float maxSpeed = controller.GetMaxSpeed();

				ImGui::DragFloat("Speed", &speed);
				ImGui::DragFloat("Speed Shift", &speedMul);
				ImGui::DragFloat("Max Speed", &maxSpeed);

				controller.SetSpeed(speed);
				controller.SetSpeedMul(speedMul);
				controller.SetMaxSpeed(maxSpeed);

				ImGui::TreePop();
			}

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::TextWrapped("You can't listen you should not touch this object\nI know you shouldn't see this object but I haven't fixed that yet!");
			ImGui::PopStyleColor();*/
		}
	}

	ImGui::SeparateWithSpacing();

	AddComponent(aEntity);

}

void Inspector::AddComponent(Entity& aEntity)
{
	if(ImGui::ButtonCenter("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if(ImGui::BeginPopup("AddComponent"))
	{
		if(ImGui::MenuItem("Model Component"))
		{
			aEntity.AddComponent<ModelComponent>();
			ImGui::CloseCurrentPopup();
		}

		if(ImGui::MenuItem("Particle Emitter"))
		{
			aEntity.AddComponent<ParticleEmitter>();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

#pragma optimize( "", off )
void Inspector::DrawFileObject(Entity& aEntity)
{
	aEntity;
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
		ImGui::SeparateWithSpacing();
		ImGui::TextWrapped("But did you know you can drag the scene onto the hierarchy or the scene view to load a scene?");
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

	case FileType::Node:
	{

		std::string* nodeName = static_cast<std::string*>(selectedObjectData->FileData);

		ImGui::InputText("Name", nodeName);

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
