#pragma once
#include <memory>
#include "Core/Model/SelectedObject.hpp"
#include <Core/Model/Entity.h>
#include "Utilites/Pointers.h"

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

	static Ref<SelectedObject> GetSelectedObject()
	{
		if(mySelectedObject == nullptr)
		{
			mySelectedObject = MakeRef<SelectedObject>(FileType::Error, nullptr);
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
	inline static Ref<SelectedObject> mySelectedObject = nullptr;
};
