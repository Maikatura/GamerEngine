#include "FileExplorer.h"
#include "ImGui/imgui.h"
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include <d3d11.h>

#include "GraphicsEngine.h"
#include "Audio/Audio.h"
#include "ImGui/imgui_internal.h"
#include "ImGuiAdded/ImGuiConfirm.h"
#include "Scene/Scene.h"

extern const std::filesystem::path AssetPath = "Assets";

FileExplorer::FileExplorer() : myCurrentDirectory(AssetPath)
{
	myConfirmCheck = false;
	myFileType.insert({ FileType::Error, TextureAssetHandler::GetTexture(L"Editor\\icons\\error.dds") });
	myFileType.insert({ FileType::Folder, TextureAssetHandler::GetTexture(L"Editor\\icons\\folder.dds") });
	myFileType.insert({ FileType::Shader, TextureAssetHandler::GetTexture(L"Editor\\icons\\shader.dds") });
	myFileType.insert({ FileType::Model, TextureAssetHandler::GetTexture(L"Editor\\icons\\model.dds") });
	myFileType.insert({ FileType::Audio, TextureAssetHandler::GetTexture(L"Editor\\icons\\audio.dds") });
	myFileType.insert({ FileType::Scene,TextureAssetHandler::GetTexture(L"Editor\\icons\\scene.dds") });

	myDeletePath = "";
}

bool FileExplorer::OnImGuiRender()
{

	ImGui::Begin(EditorNames::ContentBrowserName.c_str());
	


	if(myCurrentDirectory != std::filesystem::path(AssetPath))
	{
		if(ImGui::Button("<-"))
		{
			TextureAssetHandler::UnloadUnusedTextures();
			myCurrentDirectory = myCurrentDirectory.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if(columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for(auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
	{
		myCurrentPath = directoryEntry.path();
		std::string filenameString = myCurrentPath.filename().string();

		auto fileType = GetFileType(directoryEntry);
		auto icon = SelectIcon(directoryEntry, fileType)->GetSRV().Get();

		ImGui::PushID(filenameString.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if(ImGui::ImageButton(icon, {thumbnailSize, thumbnailSize}))
		{
			SetInspectorValue(directoryEntry, fileType);
		}

		
		
		if(ImGui::BeginPopupContextItem())
		{
			myConfirmCheck = false;

			if(ImGui::MenuItem("Create"))
			{

			}

			if(ImGui::MenuItem("Rename"))
			{

			}

			if(ImGui::MenuItem("Delete"))
			{
				myDeletePath = myCurrentPath;
				myConfirmCheck = true;
				auto& path = myCurrentPath;
				ImGui::Confirm("Do you want to delete file?", [path]() { std::filesystem::remove_all(path); }, []() {}, myConfirmCheck);

			}

			ImGui::EndPopup();
		}


		if(ImGui::BeginDragDropSource())
		{
			auto relativePath = std::filesystem::relative(myCurrentPath, AssetPath);
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));

			ImGui::ImageButton(icon, { 64, 64 });

			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if(directoryEntry.is_directory())
				myCurrentDirectory /= myCurrentPath.filename();

		}
		ImGui::TextWrapped(filenameString.c_str());
		ImGui::NextColumn();
		ImGui::PopID();
	}

	ImGui::Columns(1);

	
	

	ImGui::End();

	return true;
}

void FileExplorer::SetInspectorValue(std::filesystem::directory_entry aDirectory, FileType aFileType)
{
	if(aFileType == FileType::Audio)
	{
		myFilePath = myCurrentPath.string();
		GraphicsEngine::Get()->GetScene()->SetSelectedObject<std::string>(aFileType, (void*)myFilePath.c_str());
	}
	else if (aFileType == FileType::Texture)
	{
		GraphicsEngine::Get()->GetScene()->SetSelectedObject<Texture*>(aFileType, (void*)SelectIcon(aDirectory, aFileType).get());
	}
	else
	{
		GraphicsEngine::Get()->GetScene()->SetSelectedObject<Texture>(FileType::Error, nullptr);
	}
	
}


std::shared_ptr<Texture> FileExplorer::SelectIcon(std::filesystem::directory_entry aDirectory, FileType aFileType)
{

	if(aFileType == FileType::Texture)
	{
		return TextureAssetHandler::GetTexture(myCurrentPath);
	}
	else
	{
		return myFileType[aFileType];
	}
}

FileType FileExplorer::GetFileType(std::filesystem::directory_entry aDirectory)
{

	if(aDirectory.is_directory()) return FileType::Folder;

	std::filesystem::path fileExt = aDirectory.path().extension();
	if(fileExt == ".hlsl") return FileType::Shader;
	if(fileExt == ".fbx") return FileType::Model;
	if(fileExt == ".dds") return FileType::Texture;

	if(fileExt == ".mp3") return FileType::Audio;
	if(fileExt == ".wav") return FileType::Audio;
	if(fileExt == ".ogg") return FileType::Audio;

	if(fileExt == ".csf") return FileType::Scene;


	return FileType::Error;

}
