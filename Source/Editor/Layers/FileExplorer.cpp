#include "FileExplorer.h"
#include "ImGui/imgui.h"
#include <AssetHandlers/TextureAssetHandler.h>
#include <Model/Texture.h>
#include <d3d11.h>

#include "GraphicsEngine.h"
#include "Audio/Audio.h"
#include "ImGuiAdded/ImGuiConfirm.h"
#include "Scene/Scene.h"
#include <ImGui/imgui_stdlib.h>
#include <Types/FileExtensions.h>

#include "Render/SelectionData.h"

extern const std::filesystem::path AssetPath = "Assets";

FileExplorer::FileExplorer() : myCurrentDirectory(AssetPath)
{
	myConfirmCheck = false;
	myIsRenaming = false;
	myFileType.insert({ FileType::Error,		TextureAssetHandler::GetTexture(L"Editor\\icons\\error.dds")	});
	myFileType.insert({ FileType::Folder,		TextureAssetHandler::GetTexture(L"Editor\\icons\\folder.dds")	});
	myFileType.insert({ FileType::Shader,		TextureAssetHandler::GetTexture(L"Editor\\icons\\shader.dds")	});
	myFileType.insert({ FileType::Scene,		TextureAssetHandler::GetTexture(L"Editor\\icons\\scene.dds")	});
	myFileType.insert({ FileType::Audio,		TextureAssetHandler::GetTexture(L"Editor\\icons\\audio.dds")	});
	myFileType.insert({ FileType::Prefab,		TextureAssetHandler::GetTexture(L"Editor\\icons\\prefab.dds")	});
	myFileType.insert({ FileType::Model,		TextureAssetHandler::GetTexture(L"Editor\\icons\\fbx.dds")		});
	myFileType.insert({ FileType::Animation,	TextureAssetHandler::GetTexture(L"Editor\\icons\\anim.dds")		});
	myFileType.insert({ FileType::Text,		TextureAssetHandler::GetTexture(L"Editor\\icons\\text.dds")		});
	myFileType.insert({ FileType::DLL,		TextureAssetHandler::GetTexture(L"Editor\\icons\\dll.dds")		});

	myExtensionsMap.insert({	shaderExt,	FileType::Shader	});
	myExtensionsMap.insert({	textureExt,		FileType::Texture	});
	myExtensionsMap.insert({ dllExt,		FileType::DLL		});

	myExtensionsMap.insert({ textExt,		FileType::Text		});
	myExtensionsMap.insert({ jsonExt,		FileType::Text		});

	myExtensionsMap.insert({	fbxExt,		FileType::Model		});
	myExtensionsMap.insert({	animationExt,	FileType::Animation });
	myExtensionsMap.insert({	prefabExt,	FileType::Prefab	});

	myExtensionsMap.insert({	mp3Ext,		FileType::Audio		});
	myExtensionsMap.insert({	wavExt,		FileType::Audio		});
	myExtensionsMap.insert({	oggExt,		FileType::Audio		});

	myExtensionsMap.insert({	sceneExt,		FileType::Scene		});


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

	static float padding = 32.0f;
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
			if (!directoryEntry.is_directory())
			{
				mySelectedPath = directoryEntry.path();
			}
			SetInspectorValue(directoryEntry, fileType);
		}

		if(ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = myCurrentPath.c_str();

			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::ImageButton(icon, { 64, 64 });

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			if(!directoryEntry.is_directory())
			{
				mySelectedPath = directoryEntry.path();
			}
		}

		if(ImGui::BeginPopupContextItem())
		{
			myConfirmCheck = false;

			if(ImGui::MenuItem("Rename"))
			{
				if(!directoryEntry.is_directory())
				{
					mySelectedPath = directoryEntry.path();
					myRenameBuffer = mySelectedPath.filename().string();
					myIsRenaming = true;
				}
			}

			if(ImGui::MenuItem("Delete"))
			{
				myDeletePath = myCurrentPath;
				myConfirmCheck = true;
			}

			ImGui::EndPopup();
		}


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
			if(ImGui::IsKeyPressed(ImGuiKey_F2) && ImGui::IsWindowHovered())
			{
				if(!directoryEntry.is_directory())
				{
					myRenameBuffer = mySelectedPath.filename().string();
					myIsRenaming = true;
				}
			}
		
			ImGui::TextWrapped(filenameString.c_str());
		}
		ImGui::NextColumn();
		ImGui::PopID();
	}

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

	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		myIsRenaming = false;
	}

	if(ImGui::IsKeyPressed(ImGuiKey_Enter) || (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemHovered()))
	{
		if (myRenameBuffer != "")
		{
			std::filesystem::path test = myCurrentDirectory.string() + "\\" + myRenameBuffer;
			std::filesystem::rename(mySelectedPath, test);
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
	else if (aFileType == FileType::Texture)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, (void*)SelectIcon(aDirectory, aFileType).get());
	}
	else if(aFileType == FileType::Scene)
	{
		SelectionData::SetSelectedObject<Texture*>(aFileType, nullptr);
	}
	else if (aFileType == FileType::Text)
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
	if (out == myExtensionsMap.end())
	{
		return FileType::Error;
	}

	return out->second;
}
