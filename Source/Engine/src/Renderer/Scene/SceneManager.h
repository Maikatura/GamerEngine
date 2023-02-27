#pragma once
#include "Scene.h"
#include "Data/SharedRef.h"
#include <string>
#include <queue>


class SceneManager
{
	inline static std::shared_ptr<Scene> myScene;
	inline static std::mutex mySceneMutex;

	inline static std::thread* mySaveThread;
	inline static std::atomic<bool> mySaveDone;

	inline static std::thread* myLoadThread;
	inline static std::atomic<bool> myLoadDone;

public:

	static void Initialize();
	static void ClearAndLoadNewScene(const std::string& aFilepath);
	static void SaveScene(const std::string& aFilepath);

	static void Update();
	static Scene* GetScene();

	static Entity CreateEntityType(int aEntityType, UUID2 aUUID = 0);
};
