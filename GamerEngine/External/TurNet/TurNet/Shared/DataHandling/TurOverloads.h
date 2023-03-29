#pragma once
#include "TurMessage.h"
#include "MessageData.h"
#include "MessageTypes.h"

namespace TurNet
{

	///////////////////////////////////////


	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, TurHeartbeatMessage& data)
	{
		msg >> data.IsAlive;
		return msg;
	}

	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const TurHeartbeatMessage& data)
	{
		msg << data.IsAlive;
		msg.Header.ID = NetworkDataTypes::Heartbeat;
		return msg;
	}

#pragma region Vectors
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, CommonUtilities::TVector2<float>& data)
//	{
//		msg >> data.x;
//		msg >> data.y;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const CommonUtilities::TVector2<float>& data)
//	{
//		msg << data.y;
//		msg << data.x;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, CommonUtilities::TVector3<float>& data)
//	{
//		msg >> data.x;
//		msg >> data.y;
//		msg >> data.z;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const CommonUtilities::TVector3<float>& data)
//	{
//		msg << static_cast<float>(data.z);
//		msg << static_cast<float>(data.y);
//		msg << static_cast<float>(data.x);
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, CommonUtilities::TVector4<float>& data)
//	{
//		msg >> data.x;
//		msg >> data.y;
//		msg >> data.z;
//		msg >> data.w;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const CommonUtilities::TVector4<float>& data)
//	{
//		msg << data.w;
//		msg << data.z;
//		msg << data.y;
//		msg << data.x;
//
//		return msg;
//	}
//#pragma endregion
//
//#pragma region Messages
//
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, MessageToErikMessage& data)
//	{
//		msg >> data.Name;
//		msg >> data.ReciveName;
//		msg >> data.Message;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const MessageToErikMessage& data)
//	{
//		msg << data.Message;
//		msg << data.ReciveName;
//		msg << data.Name;
//		msg << static_cast<char>(NetMessageType::MessageToErik);
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, MoveStateMessage& data)
//	{
//		msg >> data.Name;
//		msg >> data.Scene;
//		msg >> data.Position;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const MoveStateMessage& data)
//	{
//		msg << data.Position;
//		msg << data.Scene;
//		msg << data.Name;
//		msg << static_cast<char>(NetMessageType::MovingInScene);
//
//		return msg;
//	}
//
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, JoinSceneMessage& data)
//	{
//		msg >> data.Name;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const JoinSceneMessage& data)
//	{
//		msg << data.Name;
//
//		return msg;
//	}
//
//
//	inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, LeaveSceneMessage& data)
//	{
//		msg >> data.Name;
//
//		return msg;
//	}
//
//	inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const LeaveSceneMessage& data)
//	{
//		msg << data.Name;
//
//		return msg;
//	}

#pragma endregion
	
}
