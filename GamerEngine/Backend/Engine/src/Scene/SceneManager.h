#pragma once
#include "Scene.h"
#include "Data/SharedRef.h"
#include <string>
#include <queue>
#include <mutex>





class SceneManager
{

	
	Ref<GamerEngine::Scene> myScene;
	Ref<GamerEngine::Scene> mySwapScene;
	std::mutex mySceneMutex;

	std::atomic<bool> mySaveDone = true;
	std::atomic<bool> myLoadDone = true;

	GamerEngine::SceneStatus mySceneStatus = GamerEngine::SceneStatus::None;

	bool myIsHeadless = false;

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
	
	void Initialize();
	void LoadScene(const std::string& aFilepath);
	void SaveScene(const std::string& aFilepath);
	
	void Update();
	void Render();
	Ref<GamerEngine::Scene> GetScene();
	
	bool IsReady();
	
	GamerEngine::SceneStatus GetStatus();
	
	GamerEngine::Entity ConstructEntity(entt::entity aEntityValue);
	
	GamerEngine::Entity CreateEntityType(int aEntityType, const UUID2& aUUID = 0);
	void SwapScene();
};
