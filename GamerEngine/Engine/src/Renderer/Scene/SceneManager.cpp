#include "GraphicsEngine.pch.h"
#include "SceneManager.h"

#include "Renderer/GraphicsEngine.h"
#include "SceneSerializer.h"
#include "Components/CameraComponent.h"
#include "Components/Components.hpp"
#include "Components/Network/NetworkComponent.h"
#include "Renderer/Debugger/ConsoleHelper.h"
#include "Renderer/model/Entity.h"
#include "Renderer/Particles/ParticleEmitter.h"
#include "Renderer/Render/SelectionData.h"

bool SceneManager::IsHeadless()
{
	return myIsHeadless;
}

void SceneManager::SetHeadless(bool isHeadless)
{
	myIsHeadless = isHeadless;
}

void SceneManager::Initialize()
{
	/*if(myLoadThread)
	{
		delete myLoadThread;
		myLoadThread = nullptr;
	}

	if(mySaveThread)
	{
		delete mySaveThread;
		mySaveThread = nullptr;
	}*/

	myScene = std::make_shared<Scene>();
}

void SceneManager::LoadScene(const std::string& aFilepath)
{
	if (!myScene)
	{
		Initialize();
	}


	if (!aFilepath.empty())
	{
		SelectionData::SetEntityObject(Entity{ entt::null, nullptr });

		if(GraphicsEngine::Get())
		{
			GraphicsEngine::Get()->SetPauseState(true);
		}

		if(myLoadThread)
		{
			myLoadThread->join();
			delete myLoadThread;
			myLoadThread = nullptr;
		}

		if (GraphicsEngine::Get())
		{
			GraphicsEngine::Get()->StopUpdateThread();
		}

		myLoadThread = new std::thread([&, aFilepath]()
		{
			std::scoped_lock<std::mutex> lock(mySceneMutex);
			mySceneStatus = SceneStatus::Loading;

			if (!myScene)
			{
				std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
				SceneSerializer sceneLoad(newScene.get());
				if(sceneLoad.Deserialize(aFilepath, myIsHeadless))
				{
					//newScene->Initialize();
					mySwapScene = newScene;
					mySceneStatus = SceneStatus::NeedSwap;
				}
			}
			else
			{
				std::shared_ptr<Scene> newScene = std::make_shared<Scene>();
				SceneSerializer sceneLoad(newScene.get());
				if(sceneLoad.Deserialize(aFilepath, myIsHeadless))
				{
					//newScene->Initialize();
					mySwapScene = newScene;
					mySceneStatus = SceneStatus::NeedSwap;
				}

			}


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

std::shared_ptr<Scene> SceneManager::GetScene()
{
	if(!myScene)
	{
		Initialize();
	}

	if (myScene)
	{
		if (mySceneStatus == SceneStatus::Complete)
		{
			return myScene;
		}
	}

	return nullptr;
}

SceneStatus SceneManager::GetStatus()
{
	return mySceneStatus;
}

Entity SceneManager::ConstructEntity(entt::entity aEntityValue)
{
	return Entity(aEntityValue, myScene.get());
}

Entity SceneManager::CreateEntityType(int aEntityType, const UUID2& aUUID)
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

void SceneManager::SwapScene()
{
	if (mySceneStatus == SceneStatus::NeedSwap)
	{
		myScene = mySwapScene;
		myScene->SceneReady();
		mySwapScene = nullptr;
		mySceneStatus = SceneStatus::Complete;
		myScene->SetSceneStatus(SceneStatus::Complete);

		if(GraphicsEngine::Get())
		{
			GraphicsEngine::Get()->StartUpdateThread();
		}

		if(GraphicsEngine::Get())
		{
			GraphicsEngine::Get()->SetPauseState(false);
		}
	}
}
