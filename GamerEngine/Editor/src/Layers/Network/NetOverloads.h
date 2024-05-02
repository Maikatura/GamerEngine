#pragma once
#include "TurNet/Shared/DataHandling/TurMessage.h"
#include "Components/TransfromComponent.h"

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


inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, GamerEngine::TransformComponent& data)
{
	Vector3f pos;
	Vector3f rot;
	Vector3f sca;


	msg >> pos;
	msg >> rot;
	msg >> sca;

	data.SetPosition(pos);
	data.SetRotation(rot);
	data.SetScale(sca);

	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const GamerEngine::TransformComponent& data)
{
	msg << data.GetPosition();
	msg << data.GetRotation();
	msg << data.GetScale();
	return msg;
}