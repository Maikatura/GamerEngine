#pragma once
#include "TurHeader.h"

namespace TurNet
{

	inline bool CompareClients(sockaddr_in& aClientOne, sockaddr_in& aClientTwo)
	{
		return ((aClientOne.sin_addr.S_un.S_addr == aClientTwo.sin_addr.S_un.S_addr) && (aClientOne.sin_port == aClientTwo.sin_port));
	}

	
}
