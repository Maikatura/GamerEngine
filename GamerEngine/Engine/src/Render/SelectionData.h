#pragma once
#include <memory>
#include "model/SelectedObject.hpp"
#include <Model/Entity.h>

class SelectionData
{
public:
	template<class T>
	static void SetSelectedObject(FileType aFileType, void* aData)
	{
		myEntity = { entt::null, nullptr };
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

	static void SetEntityObject(Entity aEntity)
	{
		myEntity = aEntity;
	}

	static Entity GetEntityObject()
	{
		return myEntity;
	}

	static const bool IsNull()
	{
		return mySelectedObject->IsNull();
	}


private:

	inline static Entity myEntity;
	inline static std::shared_ptr<SelectedObejct> mySelectedObject = nullptr;
};
