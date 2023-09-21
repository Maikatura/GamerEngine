#include "GraphicsEngine.pch.h"
#include "MetaAssetHandler.h"
#include <filesystem>

void MetaAssetHandler::loadAllMetadata()
{

	// Clear any existing metadata
	metadata_.clear();

	// Iterate through metadata files in the directory
	for (const auto& entry : std::filesystem::directory_iterator(metadataDir_)) 
	{
		if (entry.is_regular_file() && entry.path().extension() == ".meta") 
		{
			loadMetadataFromFile(entry.path().string());
		}
	}

}

void MetaAssetHandler::loadMetadataByUUID(const std::string& uuid)
{
	std::string metafilePath = metadataDir_ + "/" + uuid + ".meta";
	loadMetadataFromFile(metafilePath);

}

json MetaAssetHandler::getMetadataByUUID(const std::string& uuid) const
{
	auto it = metadata_.find(uuid);
	if (it != metadata_.end())
	{
		return it->second;
	}
	return json(); // Return an empty JSON object if metadata is not found
}

void MetaAssetHandler::printAllMetadata() const
{
	for (const auto& [uuid, metadata] : metadata_)
	{
		std::cout << "UUID: " << uuid << std::endl;
		std::cout << "Metadata: " << metadata.dump(4) << std::endl;
	}
}

void MetaAssetHandler::loadMetadataFromFile(const std::string& metafilePath)
{

	std::ifstream metafile(metafilePath);
	if (metafile.is_open()) 
	{
		json metadata;
		metafile >> metadata;
		metafile.close();

		// Extract the UUID from the filename
		std::string uuid = std::filesystem::path(metafilePath).stem().string();
		metadata_[uuid] = metadata;
	}
	else 
	{
		std::cerr << "Failed to open metafile: " << metafilePath << std::endl;
	}

}
