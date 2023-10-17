#include "Editor.pch.h"
#include "FileExplorer.h"
#include "ImGui/imgui.h"
#include <Renderer/AssetHandlers/TextureAssetHandler.h>
#include <Renderer/Model/Texture.h>
#include "Renderer/GraphicsEngine.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include "Renderer/Scene/Scene.h"
#include <ImGui/imgui_stdlib.h>
#include <Renderer/Types/FileExtensions.h>
#include "Renderer/Render/SelectionData.h"
#include "StringCast.h"
#include <fstream>

#include "Components/CameraController.h"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Renderer/Scene/SceneManager.h"
#include "Renderer/Scene/SceneSerializer.h"

extern const std::filesystem::path AssetPath = "Assets";

FileExplorer::FileExplorer() : Layer("File Explorer"), myCurrentDirectory(AssetPath)
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
	myExtensionsMap.insert({ dllExt,		FileType::DLL });

	myExtensionsMap.insert({ textureExt0,	FileType::Texture });
	myExtensionsMap.insert({ textureExt1,	FileType::Texture });

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

void FileExplorer::OnImGuiRender()
{

	BeginMenu();

	if(ImGui::BeginPopupContextWindow("FILECREATOR") && !ImGui::IsItemHovered())
	{
		if(ImGui::MenuItem("Create Scene"))
		{
			
			/*SharedRef<Scene> scene = MakeSharedRef<Scene>();

			auto camera = scene->CreateEntity("Camera");

			auto& cameraComp = camera.AddComponent<CameraComponent>();
			camera.AddComponent<CameraControllerData>();
			camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

			cameraComp.myFarPlane = 25000.0f;
			cameraComp.myNearPlane = 0.01f;
			cameraComp.myFoV = 90.0f;
			cameraComp.Primary = true;*/

			/*SceneSerializer saveNewScene(scene);
			saveNewScene.Serialize(myCurrentDirectory.string() + "\\" + "New Scene.csf");*/
		}

		ImGui::EndPopup();
	}

	HoveredWithItem();


	auto pathBacktrack = myCurrentDirectory;

	std::vector< std::filesystem::path> paths;
	while(pathBacktrack != std::filesystem::path(AssetPath).parent_path())
	{
		paths.push_back(pathBacktrack);
		pathBacktrack = pathBacktrack.parent_path();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	for (int i = static_cast<int>(paths.size() - 1); i >= 0; i--)
	{
		std::string currentPath = paths[i].filename().string();
		if(ImGui::Button(currentPath.c_str()))
		{
			TextureAssetHandler::UnloadUnusedTextures();
			myCurrentDirectory = paths[i];
		}

		ImGui::SameLine();
		ImGui::Text("/");
		ImGui::SameLine();
	}
	ImGui::PopStyleVar();
	ImGui::NewLine();

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


	EndMenu();
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
			{
				myCurrentDirectory /= myCurrentPath.filename();
			}
			else
			{
				HandleFile(directoryEntry, fileType);
				std::cout << "Swag" << std::endl;
			}
			


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
	ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.1f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 5.0f, 5.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 5.f, 5.f });

	ImGui::SetKeyboardFocusHere();
	if(ImGui::InputText("##rename", &myRenameBuffer, ImGuiInputTextFlags_EnterReturnsTrue))
	{

	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar(3);

	if(ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		myIsRenaming = false;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_Enter) || (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()))
	{
		std::filesystem::path renameFile = myCurrentDirectory.string() + "\\" + myRenameBuffer;
		if(myRenameBuffer != "" && !FileExists(renameFile.string()))
		{
			std::filesystem::rename(mySelectedPath, renameFile);
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
	else if(aFileType == FileType::Model)
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

void FileExplorer::HandleFile(std::filesystem::directory_entry aPath, FileType aFileType)
{
	switch (aFileType) {
		case FileType::Error: break;
		case FileType::Folder: break;
		case FileType::FBX: break;
		case FileType::Shader: break;
		case FileType::Model: break;
		case FileType::Texture: break;
		case FileType::Audio: break;
		case FileType::Scene:
		{
			SceneManager::Get().LoadScene(aPath.path().string());
			break;
		}
		case FileType::Animation: break;
		case FileType::SceneObject: break;
		case FileType::Prefab: break;
		case FileType::DLL: break;
		case FileType::Text: break;
		case FileType::Node: break;
		case FileType::COUNT: break;
	default: ;
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
