#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"

struct PlayerMoveMessage
{
	uint64_t ClientID;
	std::chrono::steady_clock::time_point Time;
	float MoveSpeed;
	Vector3f Translation;

	friend std::ostream& operator<<(std::ostream& aOutstream, const PlayerMoveMessage& aMessage)
	{
		aOutstream << "Player: " << aMessage.ClientID << " moved to new position";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, PlayerMoveMessage& data)
{
	msg >> data.ClientID;
	msg >> data.Time;
	msg >> data.MoveSpeed;
	msg >> data.Translation;


	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const PlayerMoveMessage& data)
{
	msg << data.Translation;
	msg << data.MoveSpeed;
	msg << data.Time;
	msg << data.ClientID;
	msg << static_cast<char>(NetworkMessages::PlayerMove);
	return msg;
}