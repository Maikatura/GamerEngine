#pragma once
#include <string>
#include "AudioManager.h"

#pragma optimize( "", off )
class Audio
{
public:
	void Play()
	{
		
	}

	void Load(std::string aPath)
	{
		Path = aPath;
	}

	std::string GetPath() { return Path; }

private:
	std::string Path;
};
#pragma optimize( "", on )