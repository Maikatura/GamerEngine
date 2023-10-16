#pragma once
#include "Scene.h"
#include "Data/SharedRef.h"
#include <string>
#include <queue>
#include <mutex>





class SceneManager
{
	inline static std::shared_ptr<Scene> myScene;
	inline static std::shared_ptr<Scene> mySwapScene;
	inline static std::mutex mySceneMutex;

	inline static std::atomic<bool> mySaveDone;
	inline static std::atomic<bool> myLoadDone;

	inline static SceneStatus mySceneStatus;

	inline static bool myIsHeadless = false;

public:

	static bool IsHeadless();
	static void SetHeadless(bool isHeadless);

	static void Initialize();
	static void LoadScene(const std::string& aFilepath);
	static void SaveScene(const std::string& aFilepath);

	static void Update();
	static std::shared_ptr<Scene> GetScene();

	static SceneStatus GetStatus();

	static Entity ConstructEntity(entt::entity aEntityValue);

	static Entity CreateEntityType(int aEntityType, const UUID2& aUUID = 0);
	static void SwapScene();
};
