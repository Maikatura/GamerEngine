#pragma once
#include "Utilites/Pointers.h"


struct ProjectData
{
public:
	std::string ProjectName;

	std::filesystem::path StartScene;
	std::filesystem::path AssetPath;

};


class ProjectConfig
{
public:
	static ProjectConfig& Get()
	{
		static ProjectConfig instance;
		return instance;
	}



	bool Load(const std::filesystem::path& aPath);
	bool Save(const std::filesystem::path& aPath);

private:
	Ref<ProjectData> myProjectData;

};