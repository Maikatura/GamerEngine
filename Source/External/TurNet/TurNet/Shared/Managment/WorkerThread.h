#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>


namespace TurNet
{
	class WorkerThread
	{
		std::atomic<bool> done;
		std::unique_ptr<std::thread> myThread;

	public:
		
		void Start()
		{
			
		}

		void Stop()
		{
			if (myThread)
			{
				myThread->join();
				myThread.reset();
			}
		}

	};
	
}

