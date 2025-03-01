#pragma once
#include <string>
#include <random>
#include <locale>

namespace Helpers
{


	inline std::random_device rnd;
	inline std::default_random_engine rndEngine(rnd());

	inline float DegToRadFunction(float aDegree)
	{
		return aDegree * (3.141f / 180.0f);
	}

	inline float RadToDeg(float aRadian)
	{
		return static_cast<float>(aRadian * static_cast<float>(180.0f / 3.141f));
	}

	inline float RandomFlaotRange(float aMin, float aMax)
	{
		std::uniform_real_distribution<float> distr(aMin, aMax);
		return distr(rndEngine);
	}

	inline std::string ToLowerCase(const std::string& input)
	{
		std::string result;
		std::locale loc;

		for (char c : input)
		{
			result += std::tolower(c, loc);
		}

		return result;
	}

	inline std::wstring ToLowerCase(const std::wstring& input)
	{
		std::wstring result;
		std::locale loc;

		for (wchar_t c : input)
		{
			result += std::tolower(c, loc);
		}

		return result;
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

	template<class T>
	T RemoveExtension(T const& filename)
	{
		typename T::size_type const p(filename.find_last_of('.'));
		return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
	}

	inline std::wstring CleanModelName(const std::wstring& aModelName)
	{
		auto returnValue = aModelName.substr(aModelName.find_last_of(L"/\\") + 1);
		return RemoveExtension(returnValue);
	}


}
