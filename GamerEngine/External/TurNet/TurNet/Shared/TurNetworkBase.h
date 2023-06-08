#pragma once
#define DEFAULT_PORT 5000
#include <functional>
#include <mutex>
#include <thread>

#include "DataHandling/TurOverloads.h"
#include "Managment/ClientStatus.h"

namespace TurNet
{


	struct MessageSuccess
	{
		const char* dataBuffer = nullptr;
		int dataSize{0};
		uint32_t myGuaranteedMessages {0};
		const sockaddr* serverAddr = nullptr;
		std::condition_variable ackReceivedCV {};
		bool ackReceived = false;
	};

	class TurNetworkBase
	{
	protected:

		int mySentBytes = 0;
		int myRecvBytesDiff = 0;
		int myRecvBytes = 0;
		int myRecvBytesOld = 0;

		int myReturnResult = 0;
		std::atomic<bool> myWorkerShouldRun;

		std::function<void(TurMessage&)> myMessageLoop;
		ClientStatus myStatus {};


	public:
		virtual bool Start();
		virtual void StartWorker();

		void SetReceiveMessageLoop(std::function<void(TurMessage&)> aMessageLoop);

		int GetSendBytes()
		{
			return mySentBytes;
		}

		int GetRecvBytes()
		{
			return myRecvBytes;
		}

		ClientStatus GetStatus() const { return myStatus; }

	protected:
		bool StartWinSock();
		virtual void WorkerThread() = 0;

		std::mutex myThreadLock;
		std::unique_ptr<std::thread> myWorkerThread;
		std::vector<std::thread> mySenderThreads;
		/*std::vector<MessageSuccess> myGuaranteedMessages;*/

		
		std::mutex myGuaranteedMessagesMutex;
		

	
	};
}

