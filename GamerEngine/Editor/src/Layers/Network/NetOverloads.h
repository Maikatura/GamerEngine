#pragma once
#include "Components/Components.hpp"
#include "TurNet/Shared/DataHandling/TurMessage.h"

enum class NetworkMessages : char
{
	None,
	ObjectSpawn,
	ObjectDelete,
	ObjectMove,

	ServerObjectMove,

	PlayerConnectSelf,
	PlayerDisconnectSelf,
	PlayerConnect,
	PlayerDisconnect,
	PlayerMove,

	PlayerChangeSkin,

	ServerClientMove,

	Count
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, Vector3f& data)
{
	msg >> data.x;
	msg >> data.y;
	msg >> data.z;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const Vector3f& data)
{
	msg << data.z;
	msg << data.y;
	msg << data.x;
	return msg;
}

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, TransformComponent& data)
{
	msg >> data.Scale;
	msg >> data.Rotation;
	msg >> data.Translation;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const TransformComponent& data)
{
	msg << data.Translation;
	msg << data.Rotation;
	msg << data.Scale;
	return msg;
}