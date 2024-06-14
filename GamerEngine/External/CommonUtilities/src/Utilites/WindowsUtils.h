#pragma once
#pragma once
#include <cstdint>
#include <filesystem>
#include "COMInitializer.h"


namespace CommonUtilities
{
	extern void ClientRectSize(HWND aHwnd, uint32_t& aOutWidth, uint32_t& aOutHeight);

	extern void ClientRectToScreenRect(HWND aHwnd, RECT& aOutRect);

	extern void GetWindowSize(HWND aHwnd, POINT& aOutPoint);

	struct FileDialogFilter
	{
		const wchar_t* FilterName;
		const wchar_t* FilterPattern;
	};

	extern std::filesystem::path OpenFileDialog(std::vector<FileDialogFilter> aFilters, HWND aWindowHandle = NULL);
	extern std::filesystem::path SaveFileDialog(LPCWSTR aExtension, std::vector<FileDialogFilter> aFilters, HWND aWindowHandle = NULL);

	extern bool IsFileReadonly(const std::filesystem::path& aPath);
	extern bool MakeFileWritable(const std::filesystem::path& aPath);
}
