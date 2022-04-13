#pragma once
#include <filesystem>

namespace GamerEngine
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path myCurrentDirectory;
		std::filesystem::path myAssetPath;
	};
}