#pragma once
#include <string>


namespace TurNet
{
	std::string ToLower(const std::string& aString)
	{
		std::string returnString;
		for(int x = 0; x < aString.size(); x++)
		{
			returnString += tolower(aString[x]);
		}
		return returnString;
	}

	std::string ToUpper(const std::string& aString)
	{
		std::string returnString;
		for(int x = 0; x < aString.size(); x++)
		{
			returnString += toupper(aString[x]);
		}
		return returnString;
	}
}
