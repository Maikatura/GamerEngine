#pragma once
#include <commdlg.h>
#include <string>

#include "GraphicsEngine.h"

class FileDialog
{
public:

	static std::string OpenFile(const char* aFilter)
	{
		OPENFILENAMEA  ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = GraphicsEngine::Get()->GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = aFilter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();

	}

	static std::string SaveFile(const char* aFilter)
	{
		OPENFILENAMEA  ofn;
		CHAR szFile[260] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = GraphicsEngine::Get()->GetWindowHandle();
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = aFilter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if(GetSaveFileNameA(&ofn) == TRUE)
		{

			std::filesystem::path path = ofn.lpstrFile;

			return path.replace_extension("").string();
		}

		return std::string();

	}
};
