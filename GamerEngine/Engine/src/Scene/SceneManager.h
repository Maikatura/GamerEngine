#pragma once
#include "Scene.h"
#include "Data/SharedRef.h"
#include <string>
#include <queue>
#include <mutex>





class SceneManager
{

	
	Ref<Scene> myScene;
	Ref<Scene> mySwapScene;
	std::mutex mySceneMutex;

	std::atomic<bool> mySaveDone = true;
	std::atomic<bool> myLoadDone = true;

	SceneStatus mySceneStatus;

	bool myIsHeadless = false;

public:

	static SceneManager& Get()
	{
		static SceneManager instance;
		return instance;
	}

	bool IsHeadless();
	void SetHeadless(bool isHeadless);
	
	void Initialize();
	void LoadScene(const std::string& aFilepath);
	void SaveScene(const std::string& aFilepath);
	
	void Update();
	void Render();
	Ref<Scene> GetScene();
	
	bool IsReady();
	
	SceneStatus GetStatus();
	
	Entity ConstructEntity(entt::entity aEntityValue);
	
	Entity CreateEntityType(int aEntityType, const UUID2& aUUID = 0);
	void SwapScene();
};
