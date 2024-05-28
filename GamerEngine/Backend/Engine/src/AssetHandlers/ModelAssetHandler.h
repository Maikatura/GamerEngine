#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <Core/Model/Model.h>
#include <vector>
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "Utilites/Pointers.h"

namespace ofbx
{
	struct IScene;
}

class ModelAssetHandler
{
	std::vector<Ref<GamerEngine::Model>> myModelRegistry;

	std::vector<std::thread> myThreads;
	std::queue<std::function<void()>> myTaskQueue;
	std::mutex myQueueMutex;
	std::mutex myListMutex;
	std::condition_variable myTaskAvailable;
	bool myStop = false;

	bool InitUnitCube();
	bool InitUnitSphere();


	ofbx::IScene* LoadModelScene(const std::string& aPath);


public:


	static ModelAssetHandler& Get();

	void Clear();
	bool Init();

	void UnloadModel(const std::wstring& aFilePath);

	void ResetRenderedModels();

	bool LoadModelNewTesting(const std::wstring& aFilePath);

	bool LoadModelData(const std::wstring& aFilePath);
	bool LoadAnimationData(const std::wstring& aModelName, const std::wstring& someFilePath);

	//Ref<Model> GetModel(const std::wstring& aFilePath);
	Ref<GamerEngine::Model> GetModelInstance(const std::wstring& aModelName);


	void EnqueueLoadTask(std::function<void()> task)
	{
		{
			std::unique_lock<std::mutex> lock(myQueueMutex);
			myTaskQueue.push(task);
		}
		myTaskAvailable.notify_one();
	}


private:
	ModelAssetHandler(int numThreads)
	{
		for (int i = 0; i < numThreads; ++i)
		{
			myThreads.emplace_back(std::bind(&ModelAssetHandler::WorkerThread, this));
		}
	}

	~ModelAssetHandler()
	{
		myStop = true;
		myTaskAvailable.notify_all();
		for (std::thread& thread : myThreads)
		{
			thread.join();
		}
	}

	void WorkerThread()
	{
		while (true)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(myQueueMutex);
				myTaskAvailable.wait(lock, [this] { return myStop || !myTaskQueue.empty(); });
				if (myStop && myTaskQueue.empty())
				{
					return;
				}
				task = myTaskQueue.front();
				myTaskQueue.pop();
			}
			task();
		}
	}

	

private:
	

};

