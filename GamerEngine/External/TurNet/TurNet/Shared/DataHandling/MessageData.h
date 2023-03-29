#pragma once
#include <string>
#include <iostream>
#include <ws2tcpip.h>

#include "TurHeader.h"

#include "TurNet/Shared/Utility/TurString.h"

namespace TurNet
{
	#define DEFAULT_BUFFER_SIZE 512

	/**
	 * \ THIS WILL BE REMOVE WHEN MY NEW HEADER SYSTEM IS IMPLEMENTED
	 */

	struct ClientAddress
	{
		std::string Address;
		unsigned short Port;
		sockaddr_in RealAddress;

		void ToClient(sockaddr_in aAddress)
		{
			Address = TurNet::string_cast<std::string>(std::wstring(InetNtop(aAddress.sin_family, &aAddress.sin_addr, (PWSTR)Address.c_str(), DEFAULT_BUFFER_SIZE)));
			Port = aAddress.sin_port;
			RealAddress = aAddress;
		}

		sockaddr_in ToAddress()
		{
			return RealAddress;
		}
	};

	struct MessageData
	{
		ClientAddress Connection;
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


