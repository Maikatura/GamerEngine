#pragma once
#include <memory>

#include "model/SelectedObject.hpp"


class SelectionData
{
public:
	template<class T>
	static void SetSelectedObject(FileType aFileType, void* aData)
	{
		mySelectedObject->FileType = aFileType;
		mySelectedObject->FileData = (T*)aData;
		mySelectedObject->Size = sizeof(T);
	}

	static std::shared_ptr<SelectedObejct> GetSelectedObject()
	{
		if(mySelectedObject == nullptr)
		{
			mySelectedObject = std::make_shared<SelectedObejct>(FileType::Error, nullptr);
		}

		return mySelectedObject;
	}



private:
	inline static std::shared_ptr<SelectedObejct> mySelectedObject;
};
