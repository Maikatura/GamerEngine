#include "Editor.pch.h"
#include <Handlers/MetaFileHandler.h>
#include <yaml-cpp/yaml.h>

bool MetaFileHandler::LoadMeta(const std::string& aFileName)
{
	if (aFileName.empty())
	{
		return false;
	}

	YAML::Node data;
	try
	{
		data = YAML::LoadFile(aFileName.c_str());
	}
	catch(YAML::ParserException e)
	{
		return false;
	}

	return true;
}
