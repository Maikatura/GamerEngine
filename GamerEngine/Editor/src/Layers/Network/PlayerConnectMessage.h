#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct PlayerConnectSelfMessage
{
	uint64_t ClientID;
	std::string WelcomeMessage;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerConnectSelfMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " moved to new position";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerConnectSelfMessage& data)
{
	msg >> data.ClientID;
	msg >> data.WelcomeMessage;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerConnectSelfMessage& data)
{
	msg << data.WelcomeMessage;
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerConnectSelf);
	return msg;
}


// OTHER CLIENTS

struct PlayerConnectMessage
{
	uint64_t ClientID;
	Vector3f Translation;
	bool IsServer;

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
	msg >> data.IsServer;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerConnectMessage& data)
{
	msg << data.IsServer;
	msg << data.Translation;
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerConnect);
	return msg;
}
