#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct PlayerConnectSelfMessage
{
	int ClientID;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerConnectSelfMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " moved to new position";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerConnectSelfMessage& data)
{
	msg >> data.ClientID;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerConnectSelfMessage& data)
{
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerConnectSelf);
	return msg;
}


// OTHER CLIENTS

struct PlayerConnectMessage
{
	int ClientID;
	Vector3f Translation;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerConnectMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " connected";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerConnectMessage& data)
{
	msg >> data.ClientID;
	msg >> data.Translation;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerConnectMessage& data)
{
	msg << data.Translation;
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerConnect);
	return msg;
}
