#pragma once
#define DEFAULT_PORT 5000
#include <functional>
#include <mutex>
#include <thread>


#include "DataHandling/TurOverloads.h"
#include "Managment/ClientStatus.h"

namespace TurNet
{

	class TurNetworkBase
	{
	protected:
		int myReturnResult = 0;
		std::atomic<bool> myWorkerShouldRun;

		std::function<void(TurMessage&)> myMessageLoop;
		ClientStatus myStatus {};


	public:
		virtual bool Start();
		virtual void StartWorker();

		void SetReceiveMessageLoop(std::function<void(TurMessage&)> aMessageLoop);

		ClientStatus GetStatus() const { return myStatus; }

	protected:
		bool StartWinSock();
		virtual void WorkerThread() = 0;

		std::mutex myThreadLock;
		std::unique_ptr<std::thread> myWorkerThread;
	
	};
}

