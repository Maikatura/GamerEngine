#pragma once
#include <filesystem>

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();
	~ContentBrowserPanel();

	void OnImGuiRender();

private:
	std::filesystem::path myFilePath;
};