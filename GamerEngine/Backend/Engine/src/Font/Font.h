#pragma once
#include <filesystem>

#include "Core/Model/Texture.h"
#include "Utilites/Pointers.h"


namespace GamerEngine
{

	struct MSDFData;

	class Font
	{

	public:
		Font(const std::filesystem::path& aFontPath);
		~Font();


		const MSDFData* GetMSDFData() { return myData; }
		Ref<Texture> GetAtlasTexture() const { return myAtlasTexture; }

	private:
		MSDFData* myData;
		Ref<Texture> myAtlasTexture;
	};
}
