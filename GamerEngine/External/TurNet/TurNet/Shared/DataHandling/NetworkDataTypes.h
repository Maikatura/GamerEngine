#pragma once

namespace TurNet
{
	enum class NetworkDataTypes : char
	{
		None,
		Connect,
		Disconnect,
		ClientConnect,
		ClientReconnect,
		ClientDisconnect,
		ClientDisconnected,

		Message,
		SyncMessage,
		Heartbeat,
		Ping,


		Count
	};

	
}
