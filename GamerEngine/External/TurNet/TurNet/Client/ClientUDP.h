#pragma once
#include "TurHeader.h"

#include <string>

#include "TurNet/Shared/TurNetworkBase.h"
#include "TurNet/Shared/DataHandling/NetworkDataTypes.h"
#include "TurNet/Shared/DataHandling/TurOverloads.h"

namespace TurNet
{
	class ClientUDP : public TurNetworkBase
	{
		bool myIsConnected;

		SOCKET myConnectSocket;
		sockaddr_in myServer;

		std::string myServerIP;
		u_short myServerPort;



	public:
		ClientUDP();
		~ClientUDP();

		bool Connect(const std::string& aAddress, u_short aPort);

		bool Start() override;
		void Stop();
		void StartWorker() override;

		int SendToServer(TurNet::TurMessage& aMessage);

	private:

		int SendToServerRaw(char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize);
		void WorkerThread() override;
	};
}


