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
		float myIconSize = 96.0f;
		float myIconPadding = 4.0f;

		Texture myFolder;

		std::filesystem::path myCurrentDirectory;
		std::filesystem::path myAssetPath;
	};
}