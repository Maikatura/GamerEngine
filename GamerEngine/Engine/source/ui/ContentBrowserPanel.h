#pragma once
#include <filesystem>
#include "render/Texture.h"

namespace GamerEngine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		void OnImGuiRender();



	private:

		unsigned int GetIconID(const std::string& aExtension);
		float myIconSize = 96.0f;
		float myIconPadding = 4.0f;

		Texture myFolder;
		Texture myIcon;

		std::filesystem::path myCurrentDirectory;
		std::filesystem::path myAssetPath;
	};
}