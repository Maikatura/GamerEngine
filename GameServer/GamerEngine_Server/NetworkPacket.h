#pragma once
#include "Packet.h"

class NetworkPacket 
{
public:
	void Recv(Packet& packet);
	void Send(Packet& packet);
};