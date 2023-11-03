#pragma once
#include <filesystem>


namespace Engine
{

	struct MSDFData;

	class Font
	{

	public:
		Font(const std::filesystem::path& aFontPath);
		~Font();

	private:
		MSDFData* myData;
	};
}
