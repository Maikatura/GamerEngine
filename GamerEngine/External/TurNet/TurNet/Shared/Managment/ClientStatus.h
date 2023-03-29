#pragma once

namespace TurNet
{
	enum class Status
	{
		Connected,
		Disconnected,
		Connecting,
		Disconnecting,
		Started,
		Reconnecting,
		Count

	};

	struct ClientStatus
	{
		Status Status;
	};
}
