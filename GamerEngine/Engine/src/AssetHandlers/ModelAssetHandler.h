#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <Model/Model.h>
#include <vector>
#include "Model/ModelInstance.h"
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace ofbx
{
	struct IScene;
}

const int InputLayoutSize = 18;

class ModelAssetHandler
{
	std::unordered_map<std::wstring, std::shared_ptr<ModelInstance>> myModelRegistry;

	std::vector<std::thread> myThreads;
	std::queue<std::function<void()>> myTaskQueue;
	std::mutex myQueueMutex;
	std::condition_variable myTaskAvailable;
	bool myStop = false;

	bool InitUnitCube();
	bool InitUnitSphere();


	ofbx::IScene* LoadModelScene(const std::string& aPath);


public:


	static ModelAssetHandler& Get();

	void Clear();
	bool Initialize();

	void UnloadModel(const std::wstring& aFilePath);

	void ResetRenderedModels();

	bool LoadModelNewTesting(const std::wstring& aFilePath);

	bool LoadModelData(const std::wstring& aFilePath);
	bool LoadAnimationData(const std::wstring& aModelName, const std::wstring& someFilePath);

	std::shared_ptr<Model> GetModel(const std::wstring& aFilePath);
	std::shared_ptr<ModelInstance> GetModelInstance(const std::wstring& aModelName);


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
	std::array<D3D11_INPUT_ELEMENT_DESC, InputLayoutSize> CreateLayout();

};

