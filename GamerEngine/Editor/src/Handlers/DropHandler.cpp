#include <Editor.pch.h>
#include <filesystem>
#include <Handlers/DropHandler.h>

#include "Renderer/GraphicsEngine.h"
#include "StringCast.h"
#include "Renderer/AssetHandlers/ModelAssetHandler.h"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Renderer/Scene/SceneSerializer.h"
#include "Renderer/Types/FileExtensions.h"

#include "Components/Components.hpp"
#include "Renderer/Render/Renderer.h"
#include "Renderer/Scene/SceneManager.h"

std::wstring DropHandler::DropFileScene(ImRect dropRect, ImGuiID aId)
{
	bool isUsing = false;
	if(aId != 0)
	{
		if(ImGui::BeginDragDropTargetCustom(dropRect, aId))
		{
			isUsing = true;
		}
	}
	else
	{
		if(ImGui::BeginDragDropTarget())
		{

			isUsing = true;
		}
	}


	if(isUsing)
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
		std::wstring wPath = L"";

		if(payload)
		{
			wPath = static_cast<const wchar_t*>(payload->Data);
			const auto stringPath = Helpers::string_cast<std::string>(wPath);
			std::filesystem::path path = stringPath;
			const auto extension = path.extension().string();


			if(fbxExt == extension)
			{
				auto& modelComp = SceneManager::GetScene()->CreateEntity("").AddComponent<ModelComponent>(wPath);
				ConsoleHelper::Log(LogType::Info, "Loaded a model '" + path.filename().string() + "' into the scene");
			}

			if(sceneExt == extension)
			{

				GraphicsEngine::Get()->SetEngineUpdateRuntime(false);

				SceneManager::LoadScene(stringPath);
				

				ConsoleHelper::Log(LogType::Info, "Loaded a scene '" + stringPath + "'");

			}
		}

		ImGui::EndDragDropTarget();
		return wPath;

	}
	return L"";
}

std::wstring DropHandler::DropFileEntity(Entity& aEntity)
{

	bool hasFound = false;
	if(ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
		std::wstring wPath = L"";
		if(payload)
		{
			wPath = static_cast<const wchar_t*>(payload->Data);
			auto stringPath = Helpers::string_cast<std::string>(wPath);
			std::filesystem::path path = stringPath;
			auto extension = path.extension().string();


			if(textureExt0 == extension)
			{
				hasFound = true;

				if(!aEntity.HasComponent<ModelComponent>())
				{
					return L"";
				}



				//ConsoleHelper::Log(LogType::Info, "Set a Texture | Texture" + path.filename().string() + " is now set for " + modelName.filename().string());
			}

			if(animationExt == extension)
			{
				hasFound = true;

				if(!aEntity.HasComponent<ModelComponent>())
				{
					return L"";
				}
				auto& modelComp = aEntity.GetComponent<ModelComponent>();

				ModelAssetHandler::Get().LoadAnimation(modelComp.GetModel()->GetModel()->GetName(), wPath);
				modelComp.GetModel()->PlayAnimation(wPath);
				std::filesystem::path modelName = modelComp.GetModel()->GetModel()->GetName();

				ConsoleHelper::Log(LogType::Info, "Added a Animation | Model " + modelName.filename().string() + " got the animation " + path.filename().string());
			}



		}
		ImGui::EndDragDropTarget();
			return wPath;
	}

	return L"";
}
