#pragma once

namespace TurNet
{
	enum class NetworkDataTypes : char
	{
		Connect,
		Disconnect,
		ClientConnect,
		ClientReconnect,
		ClientDisconnect,
		ClientDisconnected,
		Message,
		Heartbeat,
		Ping,

		Count
	};

	
}
