#pragma once
#include <iostream>
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
	static ThreadPool& Get()
	{
		static ThreadPool instance(8);
		return instance;
	}

	void EnqueueTask(std::function<void()> task)
	{
		{
			std::unique_lock<std::mutex> lock(myQueueMutex);
			myTaskQueue.push(task);
		}
		myTaskAvailable.notify_one();
	}



	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

private:
	ThreadPool(int numThreads)
	{
		for (int i = 0; i < numThreads; ++i)
		{
			myThreads.emplace_back(std::bind(&ThreadPool::WorkerThread, this));
		}
	}

	~ThreadPool()
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

	std::vector<std::thread> myThreads;
	std::queue<std::function<void()>> myTaskQueue;
	std::mutex myQueueMutex;
	std::condition_variable myTaskAvailable;
	bool myStop = false;
};




