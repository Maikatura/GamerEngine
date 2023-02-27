#include "GraphicsEngine.pch.h"
#include "SceneManager.h"

#include "Renderer/GraphicsEngine.h"
#include "SceneSerializer.h"
#include "Components/CameraComponent.h"
#include "Components/Components.hpp"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Renderer/model/Entity.h"
#include "Renderer/Particles/ParticleEmitter.h"

void SceneManager::Initialize()
{
	if(myLoadThread)
	{
		delete myLoadThread;
		myLoadThread = nullptr;
	}

	if(mySaveThread)
	{
		delete mySaveThread;
		mySaveThread = nullptr;
	}

	myScene = std::make_shared<Scene>();
	myScene->Initialize();
}

void SceneManager::ClearAndLoadNewScene(const std::string& aFilepath)
{
	if (!myScene)
	{
		Initialize();
	}

	

	if (!aFilepath.empty())
	{
		if(myLoadThread)
		{
			myLoadThread->join();
			delete myLoadThread;
			myLoadThread = nullptr;
		}

		myLoadThread = new std::thread([&, aFilepath]()
		{
			std::scoped_lock<std::mutex> lock(mySceneMutex);
			myScene->Clear();
			SceneSerializer sceneLoad(myScene.get());
			if (sceneLoad.Deserialize(aFilepath))
			{
				return true;
			}

			return false;
		});
	}
}

void SceneManager::SaveScene(const std::string& aFilepath)
{

	if(mySaveThread)
	{
		mySaveThread->join();
		delete mySaveThread;
		mySaveThread = nullptr;
	}

	mySaveThread = new std::thread([&, aFilepath]()
	{
		std::scoped_lock<std::mutex> lock(mySceneMutex);
		if(!myScene)
		{
			Initialize();
		}
		SceneSerializer saveScene(myScene.get());
		saveScene.Serialize(aFilepath);


		mySaveDone = true;
	});

	
}

void SceneManager::Update()
{
	
}

Scene* SceneManager::GetScene()
{
	if(!myScene)
	{
		Initialize();
	}
	return myScene.get();
}

Entity SceneManager::CreateEntityType(int aEntityType, UUID2 aUUID)
{
	Entity aEntity;

	if (aUUID == 0)
	{
		aEntity = SceneManager::GetScene()->CreateEntity("");
	}
	else
	{
		aEntity = SceneManager::GetScene()->CreateEntityWithUUID(aUUID, "");
	}

	if(aEntityType == 0)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "Client";
	}

	if (aEntityType == 1)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "EditorCamera";
		aEntity.AddComponent<CameraComponent>();
	}

	if(aEntityType == 2)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "Particle Emitter";
		aEntity.AddComponent<ParticleEmitter>();
	}

	if(aEntityType == 3)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "Directional Light";
		aEntity.AddComponent<DirectionalLightComponent>();
	}

	if(aEntityType == 4)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "Point Light";
		aEntity.AddComponent<PointLightComponent>();
	}

	if(aEntityType == 5)
	{
		ConsoleHelper::Log(LogType::Info, "This will be removed when add component when its been added");
		aEntity.GetComponent<TagComponent>().Tag = "Spot Light";
		aEntity.AddComponent<SpotLightComponent>();
	}

	return aEntity;
}
