#pragma once
#include "Utilites/Pointers.h"

namespace YAML
{
	class Node;
}

struct ProjectData;

class ProjectSerializer
{
public:
	ProjectSerializer(Ref<ProjectData> aProjectData);

	bool Serialize(const std::filesystem::path& aFilepath);
	bool Deserialize(const std::filesystem::path& filepath);

private:
	Ref<ProjectData> myProjectData;

};

