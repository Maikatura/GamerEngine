#pragma once
#include <queue>
#include "TurNet/Shared/DataHandling/TurMessage.h"

namespace TurNet
{
	class ServerMessageQueue
	{
		static inline std::queue<TurNet::TurMessage> myQueue;

	public:
		static TurNet::TurMessage Top();
		static void Push(TurNet::TurMessage aMessage);

		static bool Empty();
	};
}

