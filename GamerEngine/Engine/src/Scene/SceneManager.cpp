#include "GraphicsEngine.pch.h"
#include "SceneManager.h"

#include "GraphicsEngine.h"
#include "SceneSerializer.h"
#include "Components/CameraComponent.h"
#include "Components/Components.hpp"
#include "Components/Network/NetworkComponent.h"
#include "Debugger/ConsoleHelper.h"
#include "model/Entity.h"
#include "Particles/ParticleEmitter.h"
#include "Render/SelectionData.h"
#include "Utilites/COMInitializer.h"

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
	myScene = std::make_shared<Scene>();
	mySceneStatus = SceneStatus::Complete;
}

void SceneManager::LoadScene(const std::string& aFilepath)
{
	if (!myScene)
	{
		Initialize();
	}

	if (IsReady())
	{
		if (!aFilepath.empty())
		{
			//myScene->SceneReady(false);
			
			SelectionData::SetEntityObject(Entity{ entt::null, nullptr });

			if(GraphicsEngine::Get())
			{
				GraphicsEngine::Get()->SetPauseState(true);
				GraphicsEngine::Get()->StopUpdateThread();
			}

			
				myLoadDone = false;

				// Move the old scene into a temporary smart pointer
				std::shared_ptr<Scene> oldScene = myScene;

				ThreadPool::Get().EnqueueTask([&, aFilepath]() 
					{
					mySceneStatus = SceneStatus::Loading;

					std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

					SceneSerializer sceneLoad(newScene.get());
					if (sceneLoad.Deserialize(aFilepath, myIsHeadless)) 
					{
						// Swap the scenes only if the new scene is loaded successfully
						mySwapScene = newScene;
						mySceneStatus = SceneStatus::NeedSwap;

						// At this point, oldScene will automatically be destroyed
					}

					myLoadDone = true;
					});
			}
		
	}
}

void SceneManager::SaveScene(const std::string& aFilepath)
{
	if (IsReady())
	{
		mySaveDone = false;

		ThreadPool::Get().EnqueueTask([&, aFilepath]()
			{
				if (!myScene)
				{
					Initialize();
				}
				SceneSerializer saveScene(myScene.get());
				saveScene.Serialize(aFilepath);
				mySaveDone = true;
			});
	}

	
}

void SceneManager::Update()
{
	if (mySaveDone && myLoadDone && mySceneStatus == SceneStatus::Complete)
	{
		if (myScene)
		{
			myScene->OnUpdate();
		}

		
	}
	
}

void SceneManager::Render()
{
	if (mySaveDone && myLoadDone && mySceneStatus == SceneStatus::Complete)
	{
		if (myScene)
		{
			myScene->OnRender();
		}
	}
	
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

bool SceneManager::IsReady()
{
	return (mySceneStatus == SceneStatus::Complete && mySaveDone && myLoadDone);
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
		auto& camera = aEntity.AddComponent<CameraComponent>();
		camera.EditorCam = true;
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
	if (mySceneStatus == SceneStatus::NeedSwap && mySaveDone && myLoadDone) 
	{
		
		myScene = std::move(mySwapScene);

		if (GraphicsEngine::Get()) 
		{
			GraphicsEngine::Get()->SetPauseState(false);
		}

		if (myScene)
		{
			myScene->SetSceneStatus(SceneStatus::Complete);
			myScene->SceneReady(true);
		}

		mySceneStatus = SceneStatus::Complete;
	}

}

