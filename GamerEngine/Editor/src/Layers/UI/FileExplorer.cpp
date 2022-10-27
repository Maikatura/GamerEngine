#include "Editor.pch.h"
#include "FileExplorer.h"
#include "ImGui/imgui.h"
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include "GraphicsEngine.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Scene/Scene.h"
#include <ImGui/imgui_stdlib.h>
#include <Types/FileExtensions.h>
#include "Render/SelectionData.h"
#include "StringCast.h"
#include <fstream>

#include "Debugger/ConsoleHelper.h"

extern const std::filesystem::path AssetPath = "Assets";

FileExplorer::FileExplorer() : myCurrentDirectory(AssetPath)
{
	myConfirmCheck = false;
	myIsRenaming = false;
	myFileType.insert({ FileType::Error,		TextureAssetHandler::GetTexture(L"Editor\\icons\\error.dds") });
	myFileType.insert({ FileType::Folder,		TextureAssetHandler::GetTexture(L"Editor\\icons\\folder.dds") });
	myFileType.insert({ FileType::Shader,		TextureAssetHandler::GetTexture(L"Editor\\icons\\shader.dds") });
	myFileType.insert({ FileType::Scene,		TextureAssetHandler::GetTexture(L"Editor\\icons\\scene.dds") });
	myFileType.insert({ FileType::Audio,		TextureAssetHandler::GetTexture(L"Editor\\icons\\audio.dds") });
	myFileType.insert({ FileType::Prefab,		TextureAssetHandler::GetTexture(L"Editor\\icons\\prefab.dds") });
	myFileType.insert({ FileType::Model,		TextureAssetHandler::GetTexture(L"Editor\\icons\\fbx.dds") });
	myFileType.insert({ FileType::Animation,	TextureAssetHandler::GetTexture(L"Editor\\icons\\anim.dds") });
	myFileType.insert({ FileType::Text,		TextureAssetHandler::GetTexture(L"Editor\\icons\\text.dds") });
	myFileType.insert({ FileType::DLL,		TextureAssetHandler::GetTexture(L"Editor\\icons\\dll.dds") });

	myExtensionsMap.insert({ shaderExt,	FileType::Shader });
	myExtensionsMap.insert({ textureExt,		FileType::Texture });
	myExtensionsMap.insert({ dllExt,		FileType::DLL });

	myExtensionsMap.insert({ textExt,		FileType::Text });
	myExtensionsMap.insert({ jsonExt,		FileType::Text });

	myExtensionsMap.insert({ fbxExt,		FileType::Model });
	myExtensionsMap.insert({ animationExt,	FileType::Animation });
	myExtensionsMap.insert({ prefabExt,	FileType::Prefab });

	myExtensionsMap.insert({ mp3Ext,		FileType::Audio });
	myExtensionsMap.insert({ wavExt,		FileType::Audio });
	myExtensionsMap.insert({ oggExt,		FileType::Audio });

	myExtensionsMap.insert({ sceneExt,		FileType::Scene });

	myDeletePath = "";
}

bool FileExplorer::OnImGuiRender()
{

	ImGui::Begin(EditorNames::ContentBrowserName.c_str());

	HoveredWithItem();

	if(myCurrentDirectory != std::filesystem::path(AssetPath))
	{
		if(ImGui::Button("<-"))
		{
			TextureAssetHandler::UnloadUnusedTextures();
			myCurrentDirectory = myCurrentDirectory.parent_path();
		}
	}

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if(columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);
	
	LoopThroughFiles();

	ImGui::Columns(1);

	if(myConfirmCheck)
	{
		auto& path = mySelectedPath;
		ImGui::ImGuiCenterNextWindow();
		ImGui::Confirm("Do you want to delete file?", [path]() { std::filesystem::remove_all(path); }, []() {}, myConfirmCheck);
	}

	ImGui::End();

	return true;
}

void FileExplorer::LoopThroughFiles()
{
	auto sortedDirectories = GetSortedDirectory();
	for(auto& directoryEntry : sortedDirectories)
	{
		myCurrentPath = directoryEntry.path();
		std::string filenameString = myCurrentPath.filename().string();


		auto fileType = GetFileType(directoryEntry);
		auto icon = SelectIcon(directoryEntry, fileType);

		ImGui::PushID(filenameString.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if(ImGui::ImageButton(icon->GetSRV().Get(), { thumbnailSize, thumbnailSize }))
		{
			SetInspectorValue(directoryEntry, fileType);
			mySelectedPath = directoryEntry.path();
		}

		if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			mySelectedPath = directoryEntry.path();
		}

		if(ImGui::BeginPopupContextItem())
		{
			myConfirmCheck = false;

			if(ImGui::MenuItem("Rename"))
			{
				mySelectedPath = directoryEntry.path();
				myRenameBuffer = mySelectedPath.filename().string();
				myIsRenaming = true;
			}

			if(ImGui::MenuItem("Delete"))
			{
				myDeletePath = myCurrentPath;
				myConfirmCheck = true;
			}

			ImGui::EndPopup();
		}

		ExplorerDragDropSourceSetter(icon);


		ImGui::PopStyleColor();
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if(directoryEntry.is_directory())
				myCurrentDirectory /= myCurrentPath.filename();

		}

		if(myCurrentPath == mySelectedPath && myIsRenaming)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
			RenderRenameFrame(directoryEntry);
			ImGui::PopStyleVar();
		}
		else
		{
			if(ImGui::IsKeyPressed(ImGuiKey_F2) && ImGui::IsWindowFocused())
			{
				myRenameBuffer = mySelectedPath.filename().string();
				myIsRenaming = true;
			}

			ImGui::TextWrapped(filenameString.c_str());
		}
		ImGui::NextColumn();
		ImGui::PopID();
	}
}

inline bool FileExists(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

void FileExplorer::HoveredWithItem()
{
	auto vectorPath = GraphicsEngine::Get()->GetDropPath();

	for(size_t i = 0; i < vectorPath.size(); i++)
	{
		myCopyPath = Helpers::string_cast<std::wstring>(vectorPath[i]);
		myPath = std::filesystem::absolute(myCurrentDirectory).wstring() + L"\\" + std::filesystem::path(vectorPath[i]).filename().wstring();

		std::filesystem::path path = Helpers::string_cast<std::string>(myPath);

		if (FileExists(path.string()))
		{
			std::string filename = Helpers::RemoveExtension(Helpers::string_cast<std::string>(myPath));
			std::wstring newFileName = std::filesystem::path(vectorPath[i]).replace_filename(filename + " - Copy" + std::filesystem::path(vectorPath[i]).extension().string()).wstring();
			while (FileExists(Helpers::string_cast<std::string>(newFileName)))
			{
				filename = Helpers::RemoveExtension(Helpers::string_cast<std::string>(newFileName));
				newFileName = std::filesystem::path(vectorPath[i]).replace_filename(filename + " Copy" + std::filesystem::path(vectorPath[i]).extension().string()).wstring();
			}

			
			CopyFile(myCopyPath.c_str(), newFileName.c_str(), TRUE);
		}
		else
		{
			CopyFile(myCopyPath.c_str(), myPath.c_str(), TRUE);
		}
	}
}

std::vector<std::filesystem::directory_entry> FileExplorer::GetSortedDirectory()
{
	std::vector<std::filesystem::directory_entry> sortedByDirectory;
	for(auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
	{
		if(directoryEntry.is_directory())
		{
			sortedByDirectory.push_back(directoryEntry);
		}
	}

	for(auto& directoryEntry : std::filesystem::directory_iterator(myCurrentDirectory))
	{
		if(!directoryEntry.is_directory())
		{
			sortedByDirectory.push_back(directoryEntry);
		}
	}

	return sortedByDirectory;
}

void FileExplorer::RenderRenameFrame(std::filesystem::directory_entry aDirectory)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.1f);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5.0f, 5.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 5.f, 5.f });

	ImGui::SetKeyboardFocusHere();
	if(ImGui::InputText("##rename", &myRenameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	if(ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		myIsRenaming = false;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_Enter) || (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()))
	{
		std::filesystem::path test = myCurrentDirectory.string() + "\\" + myRenameBuffer;
		if(myRenameBuffer != "" && !FileExists(test.string()))
		{
			std::filesystem::rename(mySelectedPath, test);
		}
		else
		{
			ConsoleHelper::Log(LogType::Error, "The file name exist or name was blank");
		}

		myIsRenaming = false;
	}
}

void FileExplorer::SetInspectorValue(std::filesystem::directory_entry aDirectory, FileType aFileType)
{
	if(aFileType == FileType::Audio)
	{
		myFilePath = myCurrentPath.string();
		SelectionData::SetSelectedObject<std::string>(aFileType, (void*)myFilePath.c_str());
	}
	else if(aFileType == FileType::Texture)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, (void*)SelectIcon(aDirectory, aFileType).get());
	}
	else if(aFileType == FileType::Scene)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, nullptr);
	}
	else if(aFileType == FileType::Text)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, nullptr);
	}
	else if(aFileType == FileType::Animation)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, nullptr);
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

	std::string fileExt = aDirectory.path().extension().string();

	auto out = myExtensionsMap.find(fileExt);
	if(out == myExtensionsMap.end())
	{
		return FileType::Error;
	}

	return out->second;
}

void FileExplorer::ExplorerDragDropSourceSetter(std::shared_ptr<Texture> aIcon)
{
	if(ImGui::BeginDragDropSource())
	{
		const wchar_t* itemPath = myCurrentPath.c_str();
		if(myExtensionsMap[myCurrentPath.extension().string()] == FileType::Scene)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[myCurrentPath.extension().string()] == FileType::Texture)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[myCurrentPath.extension().string()] == FileType::Model)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[myCurrentPath.extension().string()] == FileType::Animation)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		ImGui::ImageButton(aIcon->GetSRV().Get(), {64, 64});
		ImGui::EndDragDropSource();
	}
}
