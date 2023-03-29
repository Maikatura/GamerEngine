#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct PlayerSkinChangeMessage
{
	uint64_t ClientID;
	short SkinID;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerSkinChangeMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " moved to new position";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerSkinChangeMessage& data)
{
	msg >> data.ClientID;
	msg >> data.SkinID;

	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerSkinChangeMessage& data)
{
	msg << data.SkinID;
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerChangeSkin);
	return msg;
}