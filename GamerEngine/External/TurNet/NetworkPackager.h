#pragma once
#include <cstdint>

#include "TurNet/Shared/DataHandling/TurMessage.h"

#define NETWORK_PLAYLOAD_SIZE 512
namespace TurNet
{
	class NetworkPackager
	{
		static inline uint32_t myIDCounter = 0;
		static inline uint8_t myWorkBuffer[NETWORK_PLAYLOAD_SIZE * 2];
		static inline uint16_t myCursor = 0;
		static inline std::vector<TurMessage> myPackets = {};

		static void Package()
		{
			if (myCursor == 0)
			{
				return;
			}

			TurMessage packet;
			packet.Header.MessageID = myIDCounter++;
			packet.Header.mySource = 0; // CLIENT ID
			//memcpy()

		}
	};
}
