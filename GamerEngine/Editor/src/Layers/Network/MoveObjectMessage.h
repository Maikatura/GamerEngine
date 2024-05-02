#pragma once
#include "NetOverloads.h"
#include "Components/Components.hpp"



struct ObjectMoveMessage
{
	uint64_t EntityID;
	GamerEngine::TransformComponent Transform;

	friend std::ostream& operator<<(std::ostream& aOutstream, const ObjectMoveMessage& aMessage)
	{
		aOutstream << "Moved entity: " << aMessage.EntityID << " to ";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, ObjectMoveMessage& data)
{
	msg >> data.EntityID;
	msg >> data.Transform;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const ObjectMoveMessage& data)
{
	msg << data.Transform;
	msg << data.EntityID;
	msg << static_cast<char>(NetworkMessages::ObjectMove);
	return msg;
}

struct NetObjectMoveMessage
{
	uint64_t EntityID;
	GamerEngine::TransformComponent Transform;

	friend std::ostream& operator<<(std::ostream& aOutstream, const NetObjectMoveMessage& aMessage)
	{
		aOutstream << "Moved entity: " << aMessage.EntityID << " to ";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, NetObjectMoveMessage& data)
{
	msg >> data.EntityID;
	msg >> data.Transform;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const NetObjectMoveMessage& data)
{
	msg << data.Transform;
	msg << data.EntityID;
	msg << static_cast<char>(NetworkMessages::ServerObjectMove);
	return msg;
}