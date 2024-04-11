#pragma once
#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MetaFileHandler
{

  

    std::map<std::string, std::string> metadata; // Store UUID and file path mappings

    std::filesystem::path myDirectory;
    std::filesystem::path myMetadataFilePath;

public:




    void Init(const std::filesystem::path& directory, const std::filesystem::path& metadataFilePath)
    {
        myDirectory = directory;
        myMetadataFilePath = metadataFilePath;

        if (std::filesystem::exists(myMetadataFilePath)) 
        {
            json metadataJson;
            std::ifstream metadataFile(myMetadataFilePath);
            metadataFile >> metadataJson;

            for (const auto& it : metadataJson.items()) 
            {
                metadata[it.key()] = it.value();
            }
        }
        else 
        {
            // If the metadata file doesn't exist, create the metadata directory
            std::filesystem::create_directory(myDirectory);
        }
    }

    std::string GenerateFileUUID()
	{
        
        return "uuidStr";
    }

    void CreateMetadataFile(const std::filesystem::path& jsonFilePath)
	{
        json metadataJson;
        for (const auto& entry : metadata) 
        {
            metadataJson[entry.first] = entry.second;
        }

        std::ofstream jsonFile(jsonFilePath);
        jsonFile << metadataJson.dump(4);
    }

    void UpdateMetadata(const std::filesystem::path& filePath)
	{
        // Generate a UUID for the file
        std::string fileUUID = GenerateFileUUID();
        metadata[fileUUID] = filePath.string();
    }

    void CheckFileChanges()
	{
        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(myDirectory))
        {
            if (std::filesystem::is_regular_file(entry.path())) 
            {
                // Generate a UUID for the file
                std::string fileUUID = GenerateFileUUID();

                if (metadata.find(fileUUID) == metadata.end()) 
                {
                    // New file, update metadata
                    UpdateMetadata(entry.path());
                    CreateMetadataFile(myMetadataFilePath);
                }
            }
        }
    }
};