#include "Editor.pch.h"
#include "ProjectSerializer.h"
#include "ProjectConfig.h"

ProjectSerializer::ProjectSerializer(Ref<ProjectData> aProjectData)
{
	myProjectData = aProjectData;
}

bool ProjectSerializer::Serialize(const std::filesystem::path& aFilepath)
{
	return true;
}

bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
{
	return true;
}
