#include "pch.h"
#include <cstdint>
#include <filesystem>
#include "COMInitializer.h"
#include "WindowsUtils.h"

#include <shobjidl_core.h>

void CommonUtilities::ClientRectSize(HWND aHwnd, uint32_t& aOutWidth, uint32_t& aOutHeight)
{
	RECT rect{};
	GetClientRect(aHwnd, &rect);
	aOutWidth = rect.right - rect.left;
	aOutHeight = rect.bottom - rect.top;
}

void CommonUtilities::ClientRectToScreenRect(HWND aHwnd, RECT& aOutRect)
{
	MapWindowPoints(aHwnd, HWND_DESKTOP, reinterpret_cast<LPPOINT>(&aOutRect), 2);
}

void CommonUtilities::GetWindowSize(HWND aHwnd, POINT& aOutPoint)
{
	RECT windowRect;
	if (GetWindowRect(aHwnd, &windowRect))
	{
		aOutPoint.x = windowRect.right - windowRect.left;
		aOutPoint.y = windowRect.bottom - windowRect.top;
	}
	else
	{
		aOutPoint.x = -1;
		aOutPoint.y = -1;
	}
}

std::filesystem::path CommonUtilities::SaveFileDialog(LPCWSTR aExtension, std::vector<FileDialogFilter> aFilters, HWND aWindowHandle)
{
	std::filesystem::path result{};

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* fileSaveDialog;

		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&fileSaveDialog));
		if (SUCCEEDED(hr))
		{
			if (!aFilters.empty())
			{
				hr = fileSaveDialog->SetFileTypes(static_cast<UINT>(aFilters.size()), reinterpret_cast<const COMDLG_FILTERSPEC*>(aFilters.data()));
			}

			if (SUCCEEDED(hr)
				&& SUCCEEDED(fileSaveDialog->SetDefaultExtension(aExtension)))
			{
				hr = fileSaveDialog->Show(aWindowHandle);

				if (SUCCEEDED(hr))
				{
					IShellItem* shellItem;
					hr = fileSaveDialog->GetResult(&shellItem);
					if (SUCCEEDED(hr))
					{
						PWSTR saveFilePath;
						hr = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &saveFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							result = saveFilePath;
							CoTaskMemFree(saveFilePath);
						}
						shellItem->Release();
					}
				}
			}
			fileSaveDialog->Release();
		}
		CoUninitialize();
	}

	std::filesystem::path relativepath = std::filesystem::relative(result);

	if (relativepath.empty())
	{
		return result;
	}

	return relativepath;
}

bool CommonUtilities::IsFileReadonly(const std::filesystem::path& aPath)
{
	ULONG fileAttributes = GetFileAttributes(aPath.c_str());
	if (fileAttributes != INVALID_FILE_ATTRIBUTES)
	{
		return fileAttributes & FILE_ATTRIBUTE_READONLY;
	}

	return false;
}

bool CommonUtilities::MakeFileWritable(const std::filesystem::path& aPath)
{
	if (!std::filesystem::exists(aPath))
	{
		return false;
	}

	ULONG fileAttributes = GetFileAttributes(aPath.c_str());
	if (fileAttributes != INVALID_FILE_ATTRIBUTES)
	{
		if (SetFileAttributes(aPath.c_str(), fileAttributes & ~FILE_ATTRIBUTE_READONLY) != 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

std::filesystem::path CommonUtilities::OpenFileDialog(std::vector<FileDialogFilter> aFilters, HWND aWindowHandle)
{
	std::filesystem::path result{};
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* fileOpenDialog;
		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&fileOpenDialog));
		if (SUCCEEDED(hr))
		{
			if (!aFilters.empty())
			{
				hr = fileOpenDialog->SetFileTypes(static_cast<UINT>(aFilters.size()), reinterpret_cast<const COMDLG_FILTERSPEC*>(aFilters.data()));
			}

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = fileOpenDialog->Show(aWindowHandle);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* shellItem;
					hr = fileOpenDialog->GetResult(&shellItem);
					if (SUCCEEDED(hr))
					{
						PWSTR openedFilePath;
						hr = shellItem->GetDisplayName(SIGDN_FILESYSPATH, &openedFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							result = openedFilePath;
							CoTaskMemFree(openedFilePath);
						}
						shellItem->Release();
					}
				}
			}
			fileOpenDialog->Release();
		}
		CoUninitialize();
	}

	std::filesystem::path relativepath = std::filesystem::relative(result);

	if (relativepath.empty())
	{
		return result;
	}

	return relativepath;
}
