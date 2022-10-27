#include <Editor.pch.h>
#include <filesystem>
#include <Handlers/DropHandler.h>

#include "GraphicsEngine.h"
#include "StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Debugger/ConsoleHelper.h"
#include "Scene/SceneSerializer.h"
#include "Types/FileExtensions.h"

#include "Components/Components.hpp"

void DropHandler::DropFileScene()
{
	auto testing = ImGui::GetDragDropPayload();

	if(!ImGui::IsWindowHovered())
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
	bool hasFound = false;


	if(fbxExt == extension)
	{
		hasFound = true;
		auto& modelComp = GraphicsEngine::Get()->GetScene()->CreateEntity("").AddComponent<ModelComponent>();
		modelComp.myModel = ModelAssetHandler::GetModelInstance(wPath);

		ConsoleHelper::Log(LogType::Info, "Loaded a model '" + path.filename().string() + "' into the scene");
	}

	if(sceneExt == extension)
	{
		hasFound = true;
		GraphicsEngine::Get()->GetScene()->Clear();
		GraphicsEngine::Get()->GetScene() = std::make_shared<Scene>();
		SceneSerializer sceneLoader(GraphicsEngine::Get()->GetScene().get());
		sceneLoader.Deserialize(stringPath);

		ConsoleHelper::Log(LogType::Info, "Loaded a scene '" + path.filename().string() + "'");
	}

	if(hasFound == true)
	{
		ImGui::SetDragDropPayload("", nullptr, 0);
	}
}

void DropHandler::DropFileEntity(Entity& aEntity)
{
	auto testing = ImGui::GetDragDropPayload();

	if(!ImGui::IsWindowHovered())
	{
		return;
	}

	if(!ImGui::IsItemHovered())
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
	bool hasFound = false;

	if(textureExt == extension)
	{
		hasFound = true;

		if(!aEntity.HasComponent<ModelComponent>())
		{
			return;
		}


		auto& modelComp = aEntity.GetComponent<ModelComponent>();
		modelComp.myModel->GetMaterial()->SetAlbedoTexture(TextureAssetHandler::GetTexture(wPath));
		std::filesystem::path modelName = modelComp.myModel->GetModel()->GetName();
		



		ConsoleHelper::Log(LogType::Info, "Set a Texture | Texture" + path.filename().string() + " is now set for " + modelName.filename().string());
	}

	if(animationExt == extension)
	{
		hasFound = true;

		if(!aEntity.HasComponent<ModelComponent>())
		{
			return;
		}
		auto& modelComp = aEntity.GetComponent<ModelComponent>();

		ModelAssetHandler::LoadAnimation(modelComp.myModel->GetModel()->GetName(), wPath);
		modelComp.myModel->PlayAnimation(wPath);
		std::filesystem::path modelName = modelComp.myModel->GetModel()->GetName();

		ConsoleHelper::Log(LogType::Info, "Added a Animation | Model " + modelName.filename().string() + " got the animation " + path.filename().string());
	}

	if(hasFound == true)
	{
		ImGui::SetDragDropPayload("", nullptr, 0);
	}
}
