#pragma once
#include <vector>
#include <memory>
#include <Model/SceneObject.h>
#include "Camera.h"
#include <Model/Model.h>

class Scene
{
	std::vector<std::shared_ptr<Model>> mySceneObjects;
	std::shared_ptr<Camera> myMainCamera;
	
public:

	bool Initialize();

	template<typename T>
	void AddGameObject(std::shared_ptr<T> aSceneObject) 
	{
		mySceneObjects.push_back(std::move(aSceneObject));
	}

	

	void SetCamera(std::shared_ptr<Camera> aCamera) 
	{
		myMainCamera = aCamera;
	}
	
	std::shared_ptr<Camera>& GetCamera() 
	{
		return myMainCamera;
	}

	std::vector<std::shared_ptr<Model>> CullModels(std::shared_ptr<Camera> aCamera)
	{
		
		return mySceneObjects;
	}
	
};

