//#pragma once
//#include <iostream>
//#include <fstream>
//#include <map>
//#include <nlohmann/json.hpp>
//
//using json = nlohmann::json;
//
//class MetaAssetHandler {
//public:
//    // Constructor
//    MetaAssetHandler(const std::string& metadataDir) : metadataDir_(metadataDir) {}
//
//    // Load metadata for all assets in the metadata directory
//    void loadAllMetadata();
//
//    // Load metadata for a specific asset by UUID
//    void loadMetadataByUUID(const std::string& uuid);
//
//    // Get metadata for a specific asset by UUID
//    json getMetadataByUUID(const std::string& uuid) const;
//
//    // Print all loaded metadata
//    void printAllMetadata() const;
//
//private:
//    std::string metadataDir_; // Directory where metadata files are stored
//    std::map<std::string, json> metadata_; // Map of UUID to metadata
//
//    // Load metadata from a metadata file
//    void loadMetadataFromFile(const std::string& metafilePath);
//};