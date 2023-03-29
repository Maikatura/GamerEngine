#pragma once
#include <string>
#include <iostream>
#include "TurHeader.h"


namespace TurNet
{
#define DEFAULT_BUFFER_SIZE 512

	/**
	 * \ THIS WILL BE REMOVE WHEN MY NEW HEADER SYSTEM IS IMPLEMENTED
	 */

	struct MessageData
	{
		sockaddr_in Connection;
		int BufferReceiveType;
		char Buffer[DEFAULT_BUFFER_SIZE]{};
		int BufferLength = DEFAULT_BUFFER_SIZE;
	};


	struct TurMessageMessage
	{
		std::string Username;
		std::string Message;

		friend std::ostream& operator<<(std::ostream& aOutstream, const TurMessageMessage& aMessage)
		{
			aOutstream << aMessage.Username << " says: " << aMessage.Message;
			return aOutstream;
		}
	};

	struct TurHeartbeatMessage
	{
		bool IsAlive;

		friend std::ostream& operator<<(std::ostream& aOutstream, const TurHeartbeatMessage& aMessage)
		{
			aOutstream << "Client is status is " << (aMessage.IsAlive ? "alive" : "dead");
			return aOutstream;
		}
	};

	struct TurJoinMessage
	{
		std::string Username;

		friend std::ostream& operator<<(std::ostream& aOutstream, const TurJoinMessage& aMessage)
		{
			aOutstream << aMessage.Username << " joined the chat.";
			return aOutstream;
		}
	};

	struct TurLeaveMessage
	{
		std::string Username;
		friend std::ostream& operator<<(std::ostream& aOutstream, const TurLeaveMessage& aMessage)
		{
			aOutstream << aMessage.Username << " left... the chat";
			return aOutstream;
		}
	};

}


