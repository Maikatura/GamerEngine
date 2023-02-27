#pragma once
#include <string>


class MetaFileHandler
{
private:
	std::string myMetaFilePath;


public:

	bool LoadMeta(const std::string& aFileName);

};
