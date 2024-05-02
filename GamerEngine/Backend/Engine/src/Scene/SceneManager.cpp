#include "GraphicsEngine.pch.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "SceneSerializer.h"
#include "Components/AllComponents.h"
#include "Debugger/ConsoleHelper.h"
#include "Particles/ParticleEmitter.h"
#include "Core/Rendering/SelectionData.h"


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
	myScene = MakeRef<GamerEngine::Scene>();
	mySceneStatus = GamerEngine::SceneStatus::Complete;
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
			
			SelectionData::SetEntityObject(GamerEngine::Entity{ entt::null, nullptr });

			if(GraphicsEngine::Get())
			{
				GraphicsEngine::Get()->SetPauseState(true);
				//GraphicsEngine::Get()->StopUpdateThread();
			}

			
				myLoadDone = false;

				// Move the old scene into a temporary smart pointer
				Ref<GamerEngine::Scene> oldScene = myScene;

				ThreadPool::Get().EnqueueTask([&, aFilepath]() 
					{
					mySceneStatus = GamerEngine::SceneStatus::Loading;

					Ref<GamerEngine::Scene> newScene = MakeRef<GamerEngine::Scene>();

					SceneSerializer sceneLoad(newScene.get());
					if (sceneLoad.Deserialize(aFilepath, myIsHeadless)) 
					{
						// Swap the scenes only if the new scene is loaded successfully
						mySwapScene = newScene;
						mySceneStatus = GamerEngine::SceneStatus::NeedSwap;

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

void SceneManager::Update(bool aRunningState)
{
	if (mySaveDone && myLoadDone && mySceneStatus == GamerEngine::SceneStatus::Complete)
	{
		if (myScene)
		{
			myScene->OnUpdate(aRunningState);
		}
	}
	
}

void SceneManager::Render()
{
	if (mySaveDone && myLoadDone && mySceneStatus == GamerEngine::SceneStatus::Complete)
	{
		if (myScene)
		{
			myScene->OnRender();
		}
	}
	
}

Ref<GamerEngine::Scene> SceneManager::GetScene()
{
	if(!myScene)
	{
		Initialize();
	}

	if (myScene)
	{
		if (mySceneStatus == GamerEngine::SceneStatus::Complete)
		{
			return myScene;
		}
	}

	return nullptr;
}

bool SceneManager::IsReady()
{
	return (mySceneStatus == GamerEngine::SceneStatus::Complete && mySaveDone && myLoadDone);
}

GamerEngine::SceneStatus SceneManager::GetStatus()
{
	return mySceneStatus;
}

GamerEngine::Entity SceneManager::ConstructEntity(entt::entity aEntityValue)
{
	return GamerEngine::Entity(aEntityValue, myScene.get());
}

GamerEngine::Entity SceneManager::CreateEntityType(int aEntityType, const GamerEngine::UUID& aUUID)
{
	GamerEngine::Entity aEntity;

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
		auto& camera = aEntity.AddComponent<GamerEngine::CameraComponent>();
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
	if (mySceneStatus == GamerEngine::SceneStatus::NeedSwap && mySaveDone && myLoadDone)
	{
		
		myScene = std::move(mySwapScene);

		if (GraphicsEngine::Get()) 
		{
			GraphicsEngine::Get()->SetPauseState(false);
			myScene->OnRuntimeStop();
		}

		if (myScene)
		{
			myScene->SetSceneStatus(GamerEngine::SceneStatus::Complete);
			myScene->SceneReady(true);
			myScene->OnRuntimeStart();
		}

		mySceneStatus = GamerEngine::SceneStatus::Complete;
	}

}

