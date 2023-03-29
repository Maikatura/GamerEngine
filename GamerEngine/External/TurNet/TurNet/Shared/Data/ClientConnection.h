#pragma once

#include "TurHeader.h"

namespace TurNet
{
	class ClientConnection
	{
	public:
		bool IsAlive;
		bool WaitingTimeout;
		bool HasSendMessage;
		float Timer{};
		float TimeoutTimer{};
		ClientAddress Address{};

		ClientConnection()
		{
			Timer = 0.0f;
			TimeoutTimer = 0.0f;
			IsAlive = true;
			HasSendMessage = false;
			WaitingTimeout = false;
		}

		void ResetTimers(float aTimerValue, float aTimeOutValue)
		{
			Timer += aTimerValue;
			TimeoutTimer += aTimeOutValue;
			HasSendMessage = false;
			WaitingTimeout = false;
		}
	};
}