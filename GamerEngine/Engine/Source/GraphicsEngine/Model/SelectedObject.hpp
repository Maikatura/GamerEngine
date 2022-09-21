#pragma once
#include <model/EnumTypes.h>


struct SelectedObejct
{
	FileType FileType;
	void* FileData;
	size_t Size;

	template<typename T>
	T* GetType()
	{
		return (T*)FileData;
	}
};