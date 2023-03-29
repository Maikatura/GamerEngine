#pragma once

#include "TurHeader.h"

#include <functional>
#include <thread>
#include <vector>



#include "TurNet/Shared/TurNetworkBase.h"
#include "TurNet/Shared/Data/ClientConnection.h"
#include "TurNet/Shared/DataHandling/TurOverloads.h"

namespace TurNet
{
	struct ServerConfig
	{
		float TimerValue = 60.0f;
		float TimeoutValue = 120.0f;
	};

	class ServerUDP : public TurNetworkBase
	{
	public:

		ServerUDP();
		virtual ~ServerUDP();

		bool Start() override;

		void StartWorker() override;
		int SendToClient(sockaddr_in aAddress, TurNet::TurMessage& aMessage);

		void Stop();
		

	private:

		int SendToClientRaw(sockaddr_in aAddress, char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize);
		void WorkerThread() override;
		void HeartbeatThread();

		ServerConfig myServerConfig;

		std::unique_ptr<std::thread> myHeartbeatThread;
		std::vector<ClientConnection> myClients;


		SOCKET myListenSocket = INVALID_SOCKET;
	};
		
}


