#include "Editor.pch.h"
#include "Inspector.h"
#include <GraphicsEngine.h>
#include <Model/Entity.h>
#include <Render/SelectionData.h>
#include <Scene/Scene.h>
#include "Utilites/StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Audio/Audio.h"
#include "Components/CameraController.h"
#include "Components/Components.hpp"
#include "Components/NativeScriptComponent.h"
#include "Components/Network/NetworkComponent.h"
#include "Fonts/IconsForkAwesome.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Model/ModelInstance.h"
#include "Particles/ParticleEmitter.h"
#include "Light/EnvironmentLight.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Handlers/DropHandler.h"
#include "Layers/NetworkingLayer.h"
#include "Layers/Network/MoveObjectMessage.h"
#include "Scene/SceneManager.h"


Inspector::Inspector() : Layer("Inspector")
{ }

void Inspector::OnImGuiRender()
{
	BeginMenu();

	Entity entity = SelectionData::GetEntityObject();

	if (SceneManager::Get().GetScene())
	{
		if (SceneManager::Get().GetScene()->GetRegistry().valid(entity))
		{
			DrawSceneObject(entity);
		}
		else
		{
			DrawFileObject(entity);
		}
	}

	EndMenu();
}

void Inspector::DrawSceneObject(Entity& aEntity)
{
	static ImGuiInputTextFlags flagsReadOnly;
	flagsReadOnly |= ImGuiInputTextFlags_ReadOnly;

	if (aEntity.HasComponent<TagComponent>())
	{
		auto& tag = aEntity.GetComponent<TagComponent>();

		std::string tagName = tag.Tag;

		ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
		if (tagName == "SceneCamera (DONT TOUCH)")
		{
			flags |= ImGuiInputTextFlags_ReadOnly;
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
		}

		ImGui::InputText(ICON_FK_TAG" Tag", &tag.Tag, flags);
		if (tagName == "SceneCamera (DONT TOUCH)")
		{
			ImGui::PopStyleColor();
		}

		if (tag.Tag == "")
		{
			tag.Tag = "Default Name";
		}
	}

	if (aEntity.HasComponent<TransformComponent>())
	{

	}


	DrawComponent<TransformComponent>("Transform", aEntity, [](auto& component, auto aEntity)
		{

			auto& translate = component.GetPosition();
			auto& rotation = component.GetRotation();
			auto& scale = component.GetScale();

			bool myIsEditValues = false;

			if (ImGui::DragFloat3("Position", &translate.x, 0.25f))
			{
				myIsEditValues = true;
			}

			if (ImGui::DragFloat3("Rotation", &rotation.x, 0.25f))
			{
				myIsEditValues = true;
			}

			if (ImGui::DragFloat3("Scale", &scale.x, 0.05f))
			{
				myIsEditValues = true;
			}

			//component.SetPosition(translate);
			//component.SetRotation(rotation);
			//component.SetScale(scale);
		});

	ImGui::SeparateWithSpacing();

	DrawComponent<CameraComponent>("Camera Component", aEntity, [](auto& component, auto aEntity)
	{
			ImGui::DragFloat("Field of View", &component.myFoV);
			ImGui::DragFloat("Near Plane", &component.myNearPlane);
			ImGui::DragFloat("Far Plane", &component.myFarPlane);
			ImGui::Checkbox("Primary", &component.Primary);

			component.Initialize(component.myFoV, component.myNearPlane, component.myFarPlane);
	});

	DrawComponent<ModelComponent>("Model Component", aEntity, [](auto& component, auto aEntity)
		{

			float delay = component.GetDelay();
			ImGui::DragFloat("Delay", &delay);
			component.SetDelay(delay);

			{
				ImGui::BeginGroup();

				std::vector<std::string> allMeshNames;

				if (ImGui::TreeNodeEx("Blendshapes", 0, "Blendshapes"))
				{
					if (component.GetModel())
					{
						for (int i = 0; i < component.GetModel()->GetModel()->GetNumMeshes(); i++)
						{
							auto& meshData = component.GetModel()->GetModel()->GetMeshData(i);

							if (std::find(allMeshNames.begin(), allMeshNames.end(), meshData.myMeshName) == allMeshNames.end())
							{
								allMeshNames.push_back(meshData.myMeshName);

								if (ImGui::TreeNodeEx(meshData.myMeshName.c_str()))
								{
									for (int blendIndex = 0; blendIndex < meshData.Blendshapes.size(); blendIndex++)
									{
										ImGui::SliderFloat(meshData.Blendshapes[blendIndex].Name.c_str(), &meshData.Blendshapes[blendIndex].Value, 0.0f, 100.0f);
									}

									ImGui::TreePop();
								}

							}
						}
					}

					ImGui::TreePop();
				}
				ImGui::EndGroup();
			}

			{
				ImGui::BeginGroup();


				if (component.GetModel())
				{
					const std::wstring& modelName = component.GetModel()->GetModel()->GetName();
					std::string modelPath = Helpers::string_cast<std::string>(modelName);
					ImGui::InputText("Model", &modelPath, flagsReadOnly);

					auto newFile = DropHandler::DropFileEntity(aEntity);

					if (!newFile.empty())
					{
						component.SetModelAsync(newFile);

					}
				}



				ImGui::EndGroup();
			}


			{
				ImGui::BeginGroup();

				if (component.GetModel())
				{



					int size = component.GetModel()->GetModel()->GetMaterialSize();
					ImGui::InputInt("Size", &size);
					component.GetModel()->GetModel()->SetMaterialSize(size);


					std::map<std::string, std::vector<Ref<Material>>> materialGroups;
					for (auto& material : component.GetModel()->GetMaterial())
					{
						materialGroups[Helpers::string_cast<std::string>(material->GetName())].push_back(material);
					}

					std::map<std::string, std::vector<Ref<Material>>> sortedMaterials(materialGroups.begin(), materialGroups.end());

					int i = 0;

					for (auto& [name, group] : sortedMaterials)
					{
						ImGui::Text("Name: %s", name.c_str());

						for (auto& material : group)
						{
							{


								ImGui::BeginGroup();


								auto albedoTex = material->GetAlbedoTexture();



								std::string nameId = "##Albedo" + std::to_string(i);

								std::string name = (albedoTex != nullptr) ? Helpers::string_cast<std::string>(albedoTex->GetName()) : "No Texture";

								if (albedoTex != nullptr)
								{
									ImGui::Image(albedoTex->GetSRV().Get(), { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}
								else
								{
									ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}

								std::wstring newFile = DropHandler::DropFileEntity(aEntity);
								std::filesystem::path aNewPath = newFile;
								if (name != aNewPath.filename() && !aNewPath.filename().string().empty())
								{
									material->SetAlbedoTexture(TextureAssetHandler::GetTexture(newFile));
								}

								//ShowTexturePicker(*component.GetEntity(), material, albedoTex->GetTextureType());



								ImGui::EndGroup();
							}

							// Normal
							{
								ImGui::BeginGroup();

								std::string nameId = "##Normal" + std::to_string(i);


								auto normalTexture = material->GetNormalTexture();
								std::string name = (normalTexture != nullptr) ? Helpers::string_cast<std::string>(normalTexture->GetName()) : "No Texture";

								if (normalTexture != nullptr)
								{
									ImGui::Image(normalTexture->GetSRV().Get(), { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}
								else
								{
									ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}

								std::wstring newFile = DropHandler::DropFileEntity(aEntity);
								std::filesystem::path aNewPath = newFile;
								if (name != aNewPath.filename() && !aNewPath.filename().string().empty())
								{
									material->SetNormalTexture(TextureAssetHandler::GetTexture(newFile));
								}

								ImGui::EndGroup();
							}
							// Material
							{
								ImGui::BeginGroup();

								std::string nameId = "##Material" + std::to_string(i);


								auto materialTex = material->GetMaterialTexture();
								std::string name = (materialTex != nullptr) ? Helpers::string_cast<std::string>(materialTex->GetName()) : "No Texture";

								if (materialTex != nullptr)
								{
									ImGui::Image(materialTex->GetSRV().Get(), { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}
								else
								{
									ImGui::ColorButton("##placeholder", { 1,0,0,1 }, 0, { 20, 20 });
									ImGui::SameLine();
									ImGui::InputText(nameId.c_str(), &name, flagsReadOnly);
								}

								std::wstring newFile = DropHandler::DropFileEntity(aEntity);
								std::filesystem::path aNewPath = newFile;
								if (name != aNewPath.filename() && !aNewPath.filename().string().empty())
								{
									material->SetMaterialTexture(TextureAssetHandler::GetTexture(newFile));
								}

								ImGui::EndGroup();
							}
						}


					}
				}
				else
				{
					ImGui::TextWrapped("There was a problem loading the model...");
				}

				ImGui::EndGroup();

			}


			if (component.GetModel())
			{
				if (component.GetModel()->GetSkeleton()->GetRoot())
				{
					ImGui::BeginGroup();

					/*if(ImGui::ListBoxHeader("Animations"))
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
					}*/

					ImGui::EndGroup();
				}
			}
		});

	DrawComponent<ParticleEmitter>("Particle Emitter", aEntity, [](auto& component, auto aEntity)
		{
			auto& particle = component;

			if (ImGui::TreeNodeEx("ParticleEmitter", ImGuiTreeNodeFlags_DefaultOpen, "Particle Emitter"))
			{
				auto& emitterSettings = particle.GetEmitterSettings();
				auto& particleSettings = particle.GetSettings();

				if (ImGui::TreeNodeEx("Emitter Settings"))
				{
					int i = 0;
					for (auto& behavior : emitterSettings)
					{
						std::string name = "Behavior" + std::to_string(i++);
						if (ImGui::TreeNodeEx(name.c_str()))
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

				if (ImGui::TreeNodeEx("Particle Settings"))
				{
					int i = 0;
					for (auto& setting : particleSettings)
					{
						std::string name = "Setting" + std::to_string(i++);
						if (ImGui::TreeNodeEx(name.c_str()))
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
			}
		});

	DrawComponent<DirectionalLightComponent>("Directional Light", aEntity, [](auto& component, auto aEntity)
	{
		ImGui::Checkbox("Active", &component.Active);
		ImGui::Checkbox("Smooth Shadows", &component.SmoothShadows);
		ImGui::DragFloat3("Direction", &component.Direction.x);
		ImGui::ColorEdit3("Light Color", &component.Color.x);
		ImGui::DragFloat("Intensity", &component.Intensity, 0.1f, 0.0f, 10.0f);
	});

	DrawComponent<SpotLightComponent>("Spot Light", aEntity, [](auto& component, auto aEntity)
	{
		
		ImGui::ColorEdit3("Color", &component.Color.x);
		ImGui::DragFloat("Intensity", &component.Intensity);
		ImGui::DragFloat("Range", &component.Range);
		ImGui::DragFloat("Inner Cone", &component.InnerCone);
		ImGui::DragFloat("Outer Cone", &component.OuterCone);

		int tempIndex = component.mySpotLight->GetLightBufferData().ShadowMapIndex;
		ImGui::DragInt("Index", &tempIndex);

	});

	DrawComponent<PointLightComponent>("Point Light", aEntity, [](auto& component, auto aEntity)
	{
			

			ImGui::ColorEdit3("Color", &component.Color.x);
			ImGui::DragFloat("Intensity", &component.Intensity);
			ImGui::DragFloat("Range", &component.Range);
			ImGui::Checkbox("Cast Shadows", &component.CastShadow);

			int tempIndex = component.myPointLight->GetLightBufferData().ShadowMapIndex;
			ImGui::DragInt("Index", &tempIndex);
	});

	if (aEntity.HasComponent<NativeScriptComponent>())
	{
		auto check = aEntity.GetComponent<NativeScriptComponent>().Instance;
		if (check != nullptr)
		{
			auto randomMove = dynamic_cast<RandomMoverComponent*>(check);
			if (randomMove)
			{
				if (ImGui::TreeNodeEx("RandomMoverComponent", ImGuiTreeNodeFlags_DefaultOpen, "Random Mover Component"))
				{
					Vector3f minArea = randomMove->GetMinArea();
					Vector3f maxArea = randomMove->GetMaxArea();

					ImGui::DragFloat3("MinRandom", &minArea.x, 1.0f);
					ImGui::DragScalarN("MaxRandom", ImGuiDataType_Float, &maxArea.x, 3, 1.0f, &minArea.x);

					if (minArea.x > maxArea.x)
					{
						maxArea.x = minArea.x;
					}

					if (minArea.y > maxArea.y)
					{
						maxArea.y = minArea.y;
					}

					if (minArea.z > maxArea.x)
					{
						maxArea.z = minArea.z;
					}

					randomMove->SetMinArea(minArea);
					randomMove->SetMaxArea(maxArea);

					ImGui::TreePop();
				}
			}
		}
	}



	DrawComponent<Network::NetworkComponent>("Network Component", aEntity, [](auto& component, auto aEntity)
	{
			uint64_t serverID = component.GetID().Get();
			ImGui::Text("Server ID");
			ImGui::SameLine();
			ImGui::DragScalarN("##LOL", ImGuiDataType_U64, &serverID, 1);
			component.SetID(serverID);

			bool isServer = component.IsServer();
			ImGui::Text("Is Server");
			ImGui::SameLine();
			ImGui::Checkbox("##IsServer", &isServer);
			component.SetServer(isServer);

			bool shouldSmooth = component.ShouldSmooth();
			ImGui::Text("Smooth");
			ImGui::SameLine();
			ImGui::Checkbox("##SmoothSync", &shouldSmooth);
			component.SetShouldSmooth(shouldSmooth);
	});
	


	myIsEditValues = myIsEditValuesOld;

	ImGui::SeparateWithSpacing();

	AddComponent(aEntity);

}

void Inspector::AddComponent(Entity& aEntity)
{
	if (ImGui::ButtonCenter("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Model Component"))
		{
			aEntity.AddComponent<ModelComponent>();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Particle Emitter"))
		{
			aEntity.AddComponent<ParticleEmitter>();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Network Component"))
		{
			aEntity.AddComponent<Network::NetworkComponent>();
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Random Mover Component"))
		{
			aEntity.AddComponent<NativeScriptComponent>().Bind<RandomMoverComponent>();
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}
}


template<typename T>
void Inspector::DrawComponent(const std::string& aName, Entity aEntity, std::function<void(T&, Entity)> aFunction)
{

	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (aEntity.HasComponent<T>())
	{
		auto& component = aEntity.GetComponent<T>();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, aName.c_str());
		ImGui::SameLine(ImGui::GetWindowWidth() - 25);
		if (ImGui::Button("+", ImVec2{ 20, 20 }))
		{
			ImGui::OpenPopup("Settings");
		}
		ImGui::PopStyleVar();


		bool removeComponent = false;
		if (ImGui::BeginPopup("Settings"))
		{
			if (ImGui::MenuItem("Remove Component"))
			{
				removeComponent = true;
			}

			ImGui::EndPopup();
		}

		if (open)
		{
			aFunction(component, aEntity);
			ImGui::TreePop();
		}

		if (removeComponent)
		{
			aEntity.RemoveComponent<T>();
		}
	}
}

bool Inspector::ShowTexturePicker(Entity& aEntity, Ref<Material>& selectedMaterial, TextureType textureType)
{
	bool textureChanged = false;
	static bool openPopup = false; // Track whether to open the popup

	static bool lastClickWasDoubleClick = false;
	static double lastClickTime = 0.0;

	ImVec2 groupSize = ImGui::GetItemRectSize();
	if (ImGui::InvisibleButton("GroupButton", groupSize))
	{
		openPopup = true;
	}

	if (openPopup)
	{
		ImGui::OpenPopup("TexturePicker");
		openPopup = false; // Reset the flag to prevent instant closure
	}


	if (ImGui::BeginPopup("TexturePicker"))
	{
		// You can populate this part with your own texture loading code.
		// Here's a basic example of how you might display a list of textures.

		// Assuming you have an array of texture names or paths.
		auto textureList = TextureAssetHandler::GetTextureList();
		static int currentItem = -1; // Store the selected texture index (-1 means none selected).

		for (int i = 0; i < textureList.size(); i++)
		{
			std::string textureName = Helpers::string_cast<std::string>(textureList[i]->GetName());

			ImGui::Image(textureList[i]->GetSRV().Get(), { 20, 20 });
			ImGui::SameLine();
			if (ImGui::Selectable(textureName.c_str(), i == currentItem))
			{
				currentItem = i; // Update the selected texture index.
			}
		}

		if (ImGui::Button("OK"))
		{
			if (currentItem >= 0 && currentItem < textureList.size())
			{
				selectedMaterial->SetTexture(textureType, textureList[currentItem]);
				textureChanged = true; // Set the flag to indicate that the texture has changed.
			}

			ImGui::CloseCurrentPopup(); // Close the popup.
		}

		ImGui::EndPopup();
	}

	return textureChanged;
}

#pragma optimize( "", off )
void Inspector::DrawFileObject(Entity& aEntity)
{
	aEntity;
	Ref<SelectedObejct> selectedObjectData = SelectionData::GetSelectedObject();

	switch (selectedObjectData->FileType)
	{
	case FileType::Audio:
	{
		std::string audio = (const char*)(selectedObjectData->FileData);

		if (ImGui::Button("Play"))
		{
			AudioManager::PlayAudio(audio);
		}

		ImGui::SameLine();

		if (ImGui::Button("Stop"))
		{
			AudioManager::StopAudio(audio);
		}
		break;
	}

	case FileType::Texture:
	{
		float size = ImGui::GetWindowWidth();
		Texture* texture = ((Texture*)selectedObjectData->FileData);
		ImGui::Image(texture->GetSRV().Get(), { size ,size });
		ImGui::Text("Okay and why?");

		break;
	}

	case FileType::Scene:
	{
		ImGui::TextWrapped("This is a scene I don't know what you wanna change here?");
		ImGui::SeparateWithSpacing();
		ImGui::TextWrapped("But did you know you can drag the scene onto the hierarchy or the scene view to load a scene? You can also double click to open it!");
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
