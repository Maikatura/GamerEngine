#include <Editor.pch.h>
#include <filesystem>
#include <Handlers/DropHandler.h>

#include "Utilites/StringCast.h"
#include "AssetHandlers/ModelAssetHandler.h"
#include "Debugger/ConsoleHelper.h"

#include "Types/FileExtensions.h"
#include "Scene/SceneManager.h"
#include "Components/AllComponents.h"

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


			if(Helpers::ToLowerCase(fbxExt) == Helpers::ToLowerCase(extension))
			{
				SceneManager::Get().GetScene()->CreateEntity("").AddComponent<ModelComponent>(wPath);
				ConsoleHelper::Log(LogType::Info, "Loaded a model '%s' into the scene", path.filename().string().c_str());
			}

			if(sceneExt == Helpers::ToLowerCase(extension))
			{
				GraphicsEngine::Get()->SetEngineUpdateRuntime(false);
				SceneManager::Get().LoadScene(stringPath);
				ConsoleHelper::Log(LogType::Info, "Loaded a scene '%s'", stringPath.c_str());
			}
		}

		ImGui::EndDragDropTarget();
		return wPath;

	}
	return L"";
}

std::wstring DropHandler::DropFileEntity(GamerEngine::Entity& aEntity)
{

	bool hasFound;
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

				ModelAssetHandler::Get().LoadAnimationData(modelComp.GetModel()->GetName(), wPath);
				modelComp.GetModel()->PlayAnimation(wPath);
				std::filesystem::path modelName = modelComp.GetModel()->GetName();

				ConsoleHelper::Log(LogType::Info, "Added a Animation | Model %s got the animation %s", modelName.filename().string().c_str(), path.filename().string());
			}



		}
		ImGui::EndDragDropTarget();
			return wPath;
	}

	return L"";
}
