#pragma once
#include <Renderer/Types/EnumTypes.h>


struct SelectedObejct
{
	FileType FileType;
	void* FileData;
	size_t Size;

	template<class T>
	T GetType()
	{
		return reinterpret_cast<T>(static_cast<T*>(FileData));
	}

	template<class T>
	T* GetPtrType()
	{
		return static_cast<T*>(FileData);
	}

	const bool IsNull() const
	{
		if (FileData == nullptr && FileType == FileType::Error)
		{
			return true;
		}
		return false;
	}
};