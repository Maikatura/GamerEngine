#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct PlayerDisconnectMessage
{
	int ClientID;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerDisconnectMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " disconnected";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerDisconnectMessage& data)
{
	msg >> data.ClientID;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerDisconnectMessage& data)
{
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerDisconnect);
	return msg;
}