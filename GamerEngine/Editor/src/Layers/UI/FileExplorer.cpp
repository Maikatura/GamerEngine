#include "Editor.pch.h"
#include "FileExplorer.h"
#include "ImGui/imgui.h"
#include <AssetHandlers/TextureAssetHandler.h>
#include <Core/Model/Texture.h>
#include "GraphicsEngine.h"
#include "ImGuiAdded/ImGuiExtra.h"
#include <ImGui/imgui_stdlib.h>
#include <Types/FileExtensions.h>
#include "Core/Rendering/SelectionData.h"
#include "Utilites/StringCast.h"
#include "Debugger/ConsoleHelper.h"
#include "Scene/SceneManager.h"
#include <fstream>
#include <regex>

#include "Compilers/ShaderCompiler.h"
#include "Core/Rendering/PixelShader.h"

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

	myExtensionsMap.insert({ Helpers::ToLowerCase(shaderExt),	FileType::Shader });
	myExtensionsMap.insert({ Helpers::ToLowerCase(dllExt),		FileType::DLL });

	myExtensionsMap.insert({ Helpers::ToLowerCase(textureExt0),	FileType::Texture });
	myExtensionsMap.insert({ Helpers::ToLowerCase(textureExt1),	FileType::Texture });

	myExtensionsMap.insert({ Helpers::ToLowerCase(textExt),		FileType::Text });
	myExtensionsMap.insert({ Helpers::ToLowerCase(jsonExt),		FileType::Text });

	myExtensionsMap.insert({ Helpers::ToLowerCase(fbxExt),		FileType::Model });
	myExtensionsMap.insert({ Helpers::ToLowerCase(animationExt),	FileType::Animation });
	myExtensionsMap.insert({ Helpers::ToLowerCase(prefabExt),	FileType::Prefab });

	myExtensionsMap.insert({ Helpers::ToLowerCase(mp3Ext),		FileType::Audio });
	myExtensionsMap.insert({ Helpers::ToLowerCase(wavExt),		FileType::Audio });
	myExtensionsMap.insert({ Helpers::ToLowerCase(oggExt),		FileType::Audio });

	myExtensionsMap.insert({ Helpers::ToLowerCase(sceneExt),		FileType::Scene });

	myDeletePath = "";
}

void FileExplorer::ListFiles(std::filesystem::path path)
{
	for (const auto& entry : std::filesystem::directory_iterator(path)) 
	{
		if (entry.is_directory())
		{
			if (ImGui::TreeNode(entry.path().filename().string().c_str())) 
			{
				// Recursively list files in subdirectories
				ListFiles(entry.path().string());


				ImGui::TreePop();
			}

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				myCurrentDirectory = path;
			}
		}
		else 
		{
			//ImGui::Text("%s", entry.path().filename().string().c_str());
		}
	}
}

std::vector<std::string> GetDirectoryContents(const std::string& path, bool directoriesOnly = false) {
	std::vector<std::string> items;
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (directoriesOnly) {
			if (entry.is_directory()) {
				items.push_back(entry.path().filename().string());
			}
		}
		else {
			items.push_back(entry.path().filename().string());
		}
	}
	return items;
}

void FileExplorer::OnImGuiRender()
{

	BeginMenu();


	//static std::filesystem::path currentPath = AssetPath; // Keep track of the current path.
	//static std::string selectedFolder; // Currently selected folder.

	//
	//	ImGui::Columns(2, "AssetBrowser"); // Create two columns.

	//	// Column 1: Folders
	//	ImGui::Text("Folders");
	//	ImGui::Separator();
	//	std::vector<std::string> folders = GetDirectoryContents(AssetPath.string(), true);
	//	for (const auto& folder : folders) {
	//		if (ImGui::Selectable(folder.c_str(), selectedFolder == folder)) {
	//			selectedFolder = folder;
	//			currentPath = std::filesystem::path(AssetPath.string()) / folder;
	//		}
	//	}

	//	// Column 2: Files in the selected folder
	//	ImGui::NextColumn();
	//	ImGui::Text("Files");
	//	ImGui::Separator();
	//	if (!selectedFolder.empty()) {
	//		std::vector<std::string> files = GetDirectoryContents(currentPath.string());
	//		for (const auto& file : files) {
	//			ImGui::Text("%s", file.c_str());
	//		}
	//	}

	//	ImGui::Columns(1); // Reset to one column so subsequent content is not affected.
	
	


	if (ImGui::BeginPopupContextWindow("FILECREATOR") && !ImGui::IsItemHovered())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Scene"))
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

			if (ImGui::MenuItem("Shader"))
			{
				// Do something when Submenu 1 is clicked
			}

			if (ImGui::MenuItem("C# Script"))
			{
				// Do something when Submenu 1 is clicked
			}
			
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	HoveredWithItem();


	auto pathBacktrack = myCurrentDirectory;

	std::vector<std::filesystem::path> paths;
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

void FileExplorer::PopupMenu(const std::filesystem::directory_entry& aDirEntry)
{
	if(ImGui::BeginPopupContextItem())
	{
		myConfirmCheck = false;
		
		
		if(ImGui::MenuItem("Rename"))
		{
			mySelectedPath = aDirEntry.path();
			myRenameBuffer = mySelectedPath.filename().string();
			myIsRenaming = true;
		}

		if(ImGui::MenuItem("Delete"))
		{
			myDeletePath = myCurrentPath;
			myConfirmCheck = true;
		}

		if (mySelectedPath.extension() == ".hlsl")
		{
			if(ImGui::MenuItem("Compile"))
			{
				std::filesystem::path csoFile = mySelectedPath;
				csoFile.replace_extension("cso");
				PixelShader::Compile(std::filesystem::absolute(mySelectedPath).string(), "Shaders\\Custom\\" + csoFile.filename().string());
			}
		}

		ImGui::EndPopup();
	}
}

void FileExplorer::ListDirectoryRecursive(const std::filesystem::path& path)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			std::string folderName = entry.path().filename().string();
			if (ImGui::TreeNodeEx(folderName.c_str(), ImGuiTreeNodeFlags_NoAutoOpenOnLog  | ImGuiTreeNodeFlags_OpenOnArrow)) 
			{
				if (ImGui::IsItemClicked()) 
				{
					mySelectedPath = entry.path();
					myCurrentDirectory = entry.path();
				}
				// Recursively list subdirectories
				ListDirectoryRecursive(entry.path());
				ImGui::TreePop();
			}
		}
	}
}

void FileExplorer::LoopThroughFiles()
{

	ImGui::Columns(2, "AssetBrowser"); // Create two columns.

	// Begin the "Folders" child window with a fixed width.
	ImGui::BeginChild("Folders", ImVec2(0, 0), true);
	ListDirectoryRecursive(AssetPath);
	ImGui::EndChild();

	
	ImGui::NextColumn();
	// Begin the "Assets" child window with the calculated remaining width.
	ImGui::BeginChild("Assets", ImVec2(0, 0), true);


	const float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = static_cast<int>(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, nullptr, false);

	const auto sortedDirectories = GetSortedDirectory();
	for(auto& directoryEntry : sortedDirectories)
	{
		myCurrentPath = directoryEntry.path();
		std::string filenameString = myCurrentPath.filename().string();


		const auto fileType = GetFileType(directoryEntry);
		const auto icon = SelectIcon(directoryEntry, fileType);

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
		

		PopupMenu(directoryEntry);

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
				std::cout << "Swag" << '\n';

				
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

	ImGui::EndChild();

	ImGui::Columns(1);
}

inline bool FileExists(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

void FileExplorer::HoveredWithItem()
{
	const auto vectorPath = GraphicsEngine::Get()->GetDropPath();

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

std::vector<std::filesystem::directory_entry> FileExplorer::GetSortedDirectory() const
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

Ref<Texture> FileExplorer::SelectIcon(std::filesystem::directory_entry aDirectory, FileType aFileType)
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
		case FileType::Shader:
		{
			std::wstring widePath = aPath.path().wstring();
			ShellExecute(0, 0, widePath.c_str(), 0, 0 , SW_SHOW);
			break;
		}
			
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

	std::string fileExt = Helpers::ToLowerCase(aDirectory.path().extension().string());

	auto out = myExtensionsMap.find(fileExt);
	if(out == myExtensionsMap.end())
	{
		return FileType::Error;
	}

	return out->second;
}

void FileExplorer::ExplorerDragDropSourceSetter(Ref<Texture> aIcon)
{
	if(ImGui::BeginDragDropSource())
	{
		const wchar_t* itemPath = myCurrentPath.c_str();

		std::string lowerExtension = Helpers::ToLowerCase(myCurrentPath.extension().string());

		if(myExtensionsMap[lowerExtension] == FileType::Scene)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[lowerExtension] == FileType::Texture)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[lowerExtension] == FileType::Model)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		else if(myExtensionsMap[lowerExtension] == FileType::Animation)
		{
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
		}
		ImGui::ImageButton(aIcon->GetSRV().Get(), {64, 64});
		ImGui::EndDragDropSource();
	}
}
