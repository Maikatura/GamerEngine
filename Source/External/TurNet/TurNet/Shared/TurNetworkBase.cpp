#include "TurNetworkBase.h"

#include <iostream>
#include <WS2tcpip.h>

bool TurNet::TurNetworkBase::Start()
{
	return false;
}

void TurNet::TurNetworkBase::StartWorker()
{
}

// This is what the loop worker thread will run for message types (Check Docs if you feel weird about it)
void TurNet::TurNetworkBase::SetReceiveMessageLoop(std::function<void(TurMessage&)> aMessageLoop)
{
	myMessageLoop = std::move(aMessageLoop);
}

bool TurNet::TurNetworkBase::StartWinSock()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if(wsOk != 0)
	{
		std::cout << "Can't init winsock\n";
		return false;
	}

	return true;
}
