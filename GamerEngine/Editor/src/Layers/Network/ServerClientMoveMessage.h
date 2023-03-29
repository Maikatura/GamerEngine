#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct ServerClientMoveMessage
{
	int ClientID;
	

	friend std::ostream& operator<<(std::ostream& aOutstream, const ServerClientMoveMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " moved to new position";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, ServerClientMoveMessage& data)
{
	msg >> data.ClientID;

	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const ServerClientMoveMessage& data)
{
	
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::ServerClientMove);
	return msg;
}