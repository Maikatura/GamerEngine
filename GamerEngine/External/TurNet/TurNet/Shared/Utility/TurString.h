#pragma once
#include <string>
#include <random>
#include "WinSock2.h"

namespace TurNet
{
	inline std::string ToLower(const std::string& aString)
	{
		std::string returnString;
		for(int x = 0; x < aString.size(); x++)
		{
			returnString += tolower(aString[x]);
		}
		return returnString;
	}

	inline std::string ToUpper(const std::string& aString)
	{
		std::string returnString;
		for(int x = 0; x < aString.size(); x++)
		{
			returnString += toupper(aString[x]);
		}
		return returnString;
	}

	template<typename T, typename U>
	inline static T string_cast(const U& someString)
	{
		return nullptr;
	}

	template<>
	inline std::wstring string_cast<std::wstring>(const std::string& someString)
	{
		const int sLength = static_cast<int>(someString.length());
		const int len = MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, 0, 0);
		std::wstring result(len, L'\0');
		MultiByteToWideChar(CP_ACP, 0, someString.c_str(), sLength, &result[0], len);
		return result;
	}

	template<>
	inline std::string string_cast<std::string>(const std::wstring& someString)
	{
		const int sLength = static_cast<int>(someString.length());
		const int len = WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, 0, 0, 0, 0);
		std::string result(len, L'\0');
		WideCharToMultiByte(CP_ACP, 0, someString.c_str(), sLength, &result[0], len, 0, 0);
		return result;
	}
}
