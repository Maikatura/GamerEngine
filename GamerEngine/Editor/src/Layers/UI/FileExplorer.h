#pragma once
#include <filesystem>
#include <unordered_map>
#include <Layers/Layer.h>
#include <Types/EnumTypes.h>

#include "Utilites/Pointers.h"

class Texture;

class FileExplorer : public Layer
{
public:

	FileExplorer();
	void ListFiles(std::filesystem::path path);
	void OnImGuiRender() override;
	
	void PopupMenu(const std::filesystem::directory_entry& aValue);
	void ListDirectoryRecursive(const std::filesystem::path& path);
	void LoopThroughFiles();

	void HoveredWithItem();


	void RenderRenameFrame(std::filesystem::directory_entry aDirectory);
	void SetInspectorValue(std::filesystem::directory_entry aDirectory, FileType aFileType);
	void ExplorerDragDropSourceSetter(Ref<Texture> aIconTexture);
	FileType GetFileType(std::filesystem::directory_entry aDirectory);
	Ref<Texture> SelectIcon(std::filesystem::directory_entry aDirectory, FileType aFileType);
	void HandleFile(std::filesystem::directory_entry aPath, FileType aFileType);


private:
	std::vector<std::filesystem::directory_entry> GetSortedDirectory() const;

	std::filesystem::path myCurrentPath;
	std::filesystem::path myCurrentDirectory;
	std::filesystem::path myDeletePath;
	std::filesystem::path mySelectedPath;

	std::string myRenameBuffer;
	std::string myFilePath;

	std::wstring myCopyPath;
	std::wstring myPath;

	std::unordered_map<std::string, FileType> myExtensionsMap;
	std::unordered_map<FileType,Ref<Texture>> myFileType;

	float padding = 32.0f;
	float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	bool myConfirmCheck;
	bool myIsRenaming;
};
