#pragma once
#include "Scene.h"
#include "Data/SharedRef.h"
#include <string>
#include <queue>
#include <mutex>



enum class SceneState
{
	Play,
	Edit,
	Runtime,
	Count
};

class SceneManager
{
	
	
	Ref<GamerEngine::Scene> myScene;
	Ref<GamerEngine::Scene> myRuntimeScene;
	Ref<GamerEngine::Scene> mySwapScene;
	std::mutex mySceneMutex;

	std::atomic<bool> mySaveDone = true;
	std::atomic<bool> myLoadDone = true;

	GamerEngine::SceneStatus mySceneStatus = GamerEngine::SceneStatus::None;

	SceneState mySceneState = SceneState::Edit;
	SceneState myOldSceneState = SceneState::Edit;

	bool myIsHeadless = false;
	bool myIsInRuntime = false;

	inline static Ref<SceneManager> myInstance;
	
public:

	static SceneManager& Get()
	{
		if (!myInstance)
		{
			myInstance = MakeRef<SceneManager>();
		}
		
		return *myInstance;
	}

	bool IsHeadless();
	void SetHeadless(bool isHeadless);
	
	void Init();
	void LoadScene(const std::string& aFilepath);
	void SaveScene(const std::string& aFilepath);
	
	void Update(bool aRunningState = true);
	void Render();
	Ref<GamerEngine::Scene> GetScene();

	void OnRuntimeStart();
	void OnRuntimeStop();

	bool IsReady();

	void SetSceneState(SceneState aSceneState);
	GamerEngine::SceneStatus GetStatus();
	
	GamerEngine::Entity ConstructEntity(entt::entity aEntityValue);
	
	GamerEngine::Entity CreateEntityType(int aEntityType, const GamerEngine::UUID& aUUID = 0);
	void SwapScene();
};
