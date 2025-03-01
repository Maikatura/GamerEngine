#include "GraphicsEngine.pch.h"
#include "SceneManager.h"
#include "GraphicsEngine.h"
#include "SceneSerializer.h"
#include "Components/AllComponents.h"
#include "Core/Rendering/Renderer.h"
#include "Debugger/ConsoleHelper.h"
#include "Particles/ParticleEmitter.h"
#include "Core/Rendering/SelectionData.h"
#include "Utilites/VisualProfiler.h"


void SceneManager::SetSceneState(SceneState aSceneState)
{
	GamerEngine::Renderer::Clear();

	if (aSceneState == SceneState::Play)
	{
		myRuntimeScene = GamerEngine::Scene::Copy(myScene);
		myRuntimeScene->OnRuntimeStart();
		myRuntimeScene->SetSceneStatus(GamerEngine::SceneStatus::Complete);
		myRuntimeScene->SceneReady(true);

		SelectionData::SetEntityObject(GamerEngine::Entity{ entt::null, nullptr });
	}
	else if (aSceneState == SceneState::Edit)
	{
		SelectionData::SetEntityObject(GamerEngine::Entity{ entt::null, nullptr });
		if (myRuntimeScene)
		{
			myRuntimeScene->OnRuntimeStop();
			myRuntimeScene = nullptr;
		}
	}

	mySceneState = aSceneState;
}

bool SceneManager::IsHeadless()
{
	return myIsHeadless;
}

void SceneManager::SetHeadless(bool isHeadless)
{
	myIsHeadless = isHeadless;
}

void SceneManager::Init()
{
	PROFILE_SCOPE("SceneManager::Init");
	myScene = MakeRef<GamerEngine::Scene>();
	mySceneStatus = GamerEngine::SceneStatus::Complete;
}

void SceneManager::LoadScene(const std::string& aFilepath)
{


	if (!myScene)
	{
		Init();
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
					Init();
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
			ModelAssetHandler::Get().ResetRenderedModels();

			switch (mySceneState)
			{

			case SceneState::Runtime:
				myScene->OnUpdate(aRunningState);
				break;
			case SceneState::Play:
				myRuntimeScene->OnUpdate(aRunningState);
				break;

			case SceneState::Edit:
				myScene->OnUpdate(aRunningState);
				break;
			}
		}
	}
}

void SceneManager::Render()
{
	if (mySaveDone && myLoadDone && mySceneStatus == GamerEngine::SceneStatus::Complete)
	{
		if (myScene)
		{
			switch (mySceneState)
			{
			case SceneState::Runtime:
				myScene->OnRender();
				break;
			case SceneState::Play:
				myRuntimeScene->OnRender();
				break;

			case SceneState::Edit:
				myScene->OnRender();
				break;
			}

		}
	}
	
}

Ref<GamerEngine::Scene> SceneManager::GetScene()
{
	if(!myScene)
	{
		Init();
	}

	if (myScene)
	{
		if (mySceneStatus == GamerEngine::SceneStatus::Complete)
		{
			if (mySceneState == SceneState::Play)
			{
				return myRuntimeScene;
			}

			return myScene;
		}
	}

	return nullptr;
}

void SceneManager::OnRuntimeStart()
{
	if (myScene)
	{
		mySceneState = SceneState::Play;
	}
}

void SceneManager::OnRuntimeStop()
{
	if (myScene)
	{
		mySceneState = SceneState::Edit;
	}
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
	return GamerEngine::Entity(aEntityValue, (mySceneState == SceneState::Play) ? myRuntimeScene.get() : myScene.get());
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
		if (mySceneState == SceneState::Runtime)
		{
			if (myScene)
			{
				myScene->OnRuntimeStop();
			}
		}

		myScene = std::move(mySwapScene);

		if (mySceneState == SceneState::Runtime)
		{
			

			//myScene = GamerEngine::Scene::Copy(myScene);
			myScene->OnRuntimeStart();
			myScene->SetSceneStatus(GamerEngine::SceneStatus::Complete);
			myScene->SceneReady(true);
		}


		myScene->Initialize();

		if (GraphicsEngine::Get()) 
		{
			GraphicsEngine::Get()->SetPauseState(false);
			
		}

		if (myScene)
		{

			myScene->SetSceneStatus(GamerEngine::SceneStatus::Complete);
			myScene->SceneReady(true);
		}

		mySceneStatus = GamerEngine::SceneStatus::Complete;
	}

}

