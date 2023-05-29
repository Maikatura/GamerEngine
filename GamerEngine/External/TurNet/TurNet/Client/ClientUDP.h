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


		const int MAX_RETRIES = 3;  // Maximum number of retransmissions
		const int TIMEOUT_MS = 1000;  // Timeout for waiting for ACK in milliseconds

	public:
		ClientUDP();
		~ClientUDP();

		bool Connect(const std::string& aAddress, u_short aPort);

		bool Start() override;
		void Stop();
		void StartWorker() override;

		int SendToServer(TurNet::TurMessage& aMessage, bool aShouldGuaranteed = false);

	private:

		int SendToClientRawSuccess();
		int SendToServerRaw(char aDataBuffer[DEFAULT_BUFFER_SIZE], int aSize);
		void WorkerThread() override;
	};
}


