#pragma once
#include <filesystem>
#include <unordered_map>
#include <Layers/Layer.h>
#include <Model/EnumTypes.h>
class Texture;



class FileExplorer : public Layer
{
public:

	FileExplorer();
	bool OnImGuiRender() override;


	void SetInspectorValue(std::filesystem::directory_entry aDirectory, FileType aFileType);
	std::shared_ptr<Texture> SelectIcon(std::filesystem::directory_entry aDirectory, FileType aFileType);
	FileType GetFileType(std::filesystem::directory_entry aDirectory);


private:

	std::filesystem::path myCurrentPath;
	std::filesystem::path myCurrentDirectory;
	std::filesystem::path myDeletePath;

	std::string myFilePath;

	std::unordered_map<FileType, std::shared_ptr<Texture>> myFileType;
	bool myConfirmCheck;
};
