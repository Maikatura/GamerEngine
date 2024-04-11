#include "Editor.pch.h"
#include "ProjectConfig.h"
#include <yaml-cpp/yaml.h>

#include "ProjectSerializer.h"

bool ProjectConfig::Load(const std::filesystem::path& aPath)
{
	if (!std::filesystem::exists(aPath))
	{
		return false;
	}

	Ref<ProjectData> project = MakeRef<ProjectData>();
	ProjectSerializer projSerializer(project);
	if (projSerializer.Serialize(aPath))
	{
		myProjectData = project;
		return true;
	}

	return false;
}

bool ProjectConfig::Save(const std::filesystem::path& aPath)
{
	ProjectSerializer projSerializer(myProjectData);
	if(projSerializer.Deserialize(aPath))
	{
		return true;
	}

	return false;
}


