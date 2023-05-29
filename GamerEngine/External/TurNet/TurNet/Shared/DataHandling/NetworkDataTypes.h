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
		MessageGuaranteed,
		MessageAcknowledge,
		SyncMessage,
		Heartbeat,
		Ping,


		Count
	};

	
}
