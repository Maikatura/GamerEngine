#pragma once

#include <WS2tcpip.h>

class ClientConnection
{
public:
	bool IsAlive;
	bool WaitingTimeout;
	sockaddr_in Address{};
	float Timer{};
	float TimeoutTimer{};
	bool HasSendMessage;

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