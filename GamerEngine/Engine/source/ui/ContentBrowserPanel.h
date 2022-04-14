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
		float myIconSize = 128.0f;
		float myIconPadding = 16.0f;

		Texture myFolderIcon;
		Texture myModelIcon;

		std::filesystem::path myCurrentDirectory;
		std::filesystem::path myAssetPath;
	};
}