#pragma once
#include <cstdint>
#include <ostream>
#include <vector>
#include "MessageData.h"
#include "NetworkDataTypes.h"

#include "TurHeader.h"

namespace TurNet
{

	enum class SyncMessage
	{
		Position,
		Rotation,

	};


	struct TurMessageHeader
	{
		NetworkDataTypes ID{};
		uint32_t MessageID;
		uint32_t Size = 0;
		ClientAddress Connection {};
	};

	inline std::random_device rd; // obtain a random number from hardware
	inline std::mt19937 gen(rd());
	inline std::uniform_int_distribution<> distr(-7777, 7777);

	struct TurMessage
	{
		TurMessageHeader Header{};
		std::vector<char> Body;

		TurMessage()
		{
			Header.ID = NetworkDataTypes::Message;
			Header.MessageID = static_cast<uint32_t>(distr(gen));
		}
		TurMessage(const TurMessage& someData) = default;
		~TurMessage() = default;

		size_t size()
		{
			return Body.size();
		}

		friend std::ostream& operator<<(std::ostream& aOutstream, const TurMessage& aMessage)
		{
			aOutstream << "ID: " << char(aMessage.Header.ID) << "Size:" << aMessage.Header.Size;
			return aOutstream;
		}

		template<typename DataType>
		friend TurMessage& operator<<(TurMessage& msg, const DataType& someData)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

			size_t i = msg.Body.size();

			msg.Body.resize(msg.Body.size() + sizeof(DataType));

			std::memcpy(msg.Body.data() + i, &someData, sizeof(DataType));

			msg.Header.Size = static_cast<int>(msg.size());

			return msg;
		}

		template<typename DataType>
		friend TurMessage& operator>>(TurMessage& msg, DataType& data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");
			size_t i = msg.size() - sizeof(DataType);

			std::memcpy(&data, msg.Body.data() + i, sizeof(DataType));

			msg.Body.resize(i);
			msg.Header.Size = static_cast<int>(msg.size());
			return msg;
		}

		
		friend TurMessage& operator<<(TurMessage& msg, const std::string& someData)
		{
			size_t i = msg.Body.size();
			msg.Body.resize(msg.Body.size() + someData.size());


			if (msg.Body.size() + sizeof(uint8_t) < 0x0010FFFF)
			{
				std::memcpy(msg.Body.data() + i, someData.c_str(), someData.size());
				msg.Header.Size = static_cast<int>(msg.size());

				msg << static_cast<uint8_t>(someData.size());
			}
			else
			{
				std::string wrong = "Something is wrong";
				std::memcpy(msg.Body.data() + i, wrong.c_str(), wrong.size());
				msg.Header.Size = static_cast<int>(wrong.size());

				msg << static_cast<uint8_t>(wrong.size());
			}

			return msg;
		}

		friend TurMessage& operator>>(TurMessage& msg, std::string& data)
		{
			uint8_t stringSize = 0;
			msg >> stringSize;

			data.resize(stringSize);

			size_t i = msg.size() - stringSize;

			std::memcpy((void*)data.data(), msg.Body.data() + i, stringSize);

			msg.Body.resize(i);

			msg.Header.Size = static_cast<int>(msg.size());

			return msg;
		}


		std::string BufferRaw()
		{
			std::string returnString;
			returnString = static_cast<char>(Header.ID);

			char messageID[4];
			std::memcpy(messageID, &Header.MessageID, 4);

			char size[4];
			std::memcpy(size, &Header.Size, 4);

			for(size_t i = 0; i < 4; i++)
			{
				returnString += messageID[i];
			}

			for(size_t i = 0; i < 4; i++)
			{
				returnString += size[i];
			}

			for(size_t i = 0; i < Body.size(); i++)
			{
				returnString += Body[i];
			}

			return returnString;
		}
	};


	inline std::string encrypt(const std::string& message, int key)
	{
		std::string encryptedMessage = "";
		for (char c : message) 
		{
			if (isalpha(c)) 
			{
				char base = isupper(c) ? 'A' : 'a';
				encryptedMessage += static_cast<char>((c - base + key) % 26 + base);
			}
			else 
			{
				encryptedMessage += c;
			}
		}
		return encryptedMessage;
	}

	inline std::string decrypt(const std::string& encryptedMessage, int key)
	{
		return encrypt(encryptedMessage, -key); // Decryption is just encryption with the inverse key
	}

	inline TurMessage& MessageEncrypt(const std::string& aEncryptKey, TurMessage& aNewMessage, const MessageData& someData)
	{

		int whereDataBegin = 1 + (sizeof(uint32_t) * 2);

		std::memcpy(&aNewMessage.Header.ID, &someData.Buffer[0], 1);
		std::memcpy(&aNewMessage.Header.MessageID, someData.Buffer + 1, 4);
		std::memcpy(&aNewMessage.Header.Size, someData.Buffer + 1 + sizeof(uint32_t), 4);

		size_t size = aNewMessage.Header.Size;
		aNewMessage.Body.resize(size);

		std::memcpy(aNewMessage.Body.data(), someData.Buffer + whereDataBegin, size);

		return aNewMessage;
	}

	inline TurMessage& MessageDecoder(TurMessage& aNewMessage, const MessageData& someData)
	{

		int whereDataBegin = 1 + (sizeof(uint32_t) * 2) ;

		std::memcpy(&aNewMessage.Header.ID, &someData.Buffer[0], 1);
		std::memcpy(&aNewMessage.Header.MessageID, someData.Buffer + 1, 4);
		std::memcpy(&aNewMessage.Header.Size, someData.Buffer + 1 + sizeof(uint32_t), 4);

		size_t size = aNewMessage.Header.Size;
		aNewMessage.Body.resize(size);

		std::memcpy(aNewMessage.Body.data(), someData.Buffer + whereDataBegin, size);

		return aNewMessage;
	}



}
