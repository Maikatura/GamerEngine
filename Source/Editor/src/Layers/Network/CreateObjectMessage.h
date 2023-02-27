#pragma once
#include "NetOverloads.h"

struct ObjectCreateMessage
{
	uint64_t EntityID;
	int EntityType;

	friend std::ostream& operator<<(std::ostream& aOutstream, const ObjectCreateMessage& aMessage)
	{
		aOutstream << "Created entity: " << aMessage.EntityID << " to ";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, ObjectCreateMessage& data)
{
	msg >> data.EntityID;
	msg >> data.EntityType;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const ObjectCreateMessage& data)
{
	msg << data.EntityType;
	msg << data.EntityID;
	msg << static_cast<char>(NetworkMessages::ObjectSpawn);
	return msg;
}
