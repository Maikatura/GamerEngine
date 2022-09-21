#pragma once
#include <vector>
#include <memory>
#include <Scene/Camera.h>
#include <Model/ModelInstance.h>
#include <Model/EnumTypes.h>
#include <model/SelectedObject.hpp>

class Scene
{
	std::vector<std::shared_ptr<ModelInstance>> mySceneObjects;
	std::shared_ptr<Camera> myMainCamera;
	std::shared_ptr<SelectedObejct> mySelectedObject;

public:

	bool Initialize();

	template<typename T>
	void AddSceneObject(std::shared_ptr<T> aSceneObject)
	{
		mySceneObjects.push_back(std::move(aSceneObject));
	}

	void Clear()
	{
		mySceneObjects.clear();
	}

	void SetCamera(std::shared_ptr<Camera> aCamera)
	{
		myMainCamera = aCamera;
	}

	std::shared_ptr<SelectedObejct> GetSelectedObject()
	{
		if(mySelectedObject == nullptr)
		{
			mySelectedObject = std::make_shared<SelectedObejct>(FileType::Error, nullptr);
		}

		return mySelectedObject;
	}

	template<class T>
	void SetSelectedObject(FileType aFileType, void* aData)
	{
		mySelectedObject->FileType = aFileType;
		mySelectedObject->FileData = (T*)aData;
		mySelectedObject->Size = sizeof(T);
	}

	std::shared_ptr<Camera>& GetCamera()
	{
		return myMainCamera;
	}

	void RemoveObject(std::shared_ptr<ModelInstance> aObjectToRemove)
	{
		std::swap(aObjectToRemove, *(mySceneObjects.end() - 1));
		mySceneObjects.pop_back();
	}

	std::vector<std::shared_ptr<ModelInstance>> CullModels(std::shared_ptr<Camera> aCamera)
	{

		return mySceneObjects;
	}

};

