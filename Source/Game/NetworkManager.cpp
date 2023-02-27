#include "NetworkManager.h"

#include "Components/Components.hpp"
#include "Renderer/Scene/SceneManager.h"

#include "Renderer/Model/Entity.h"
#include "TurNet/Shared/DataHandling/TurMessage.h"

enum class NetworkMessages : char
{
	None,
	ObjectSpawn,
	ObjectMove,
	Count
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, Vector3f& data)
{
	msg >> data.x;
	msg >> data.y;
	msg >> data.z;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const Vector3f& data)
{
	msg << data.z;
	msg << data.y;
	msg << data.x;
	return msg;
}

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, TransformComponent& data)
{
	msg >> data.Scale;
	msg >> data.Rotation;
	msg >> data.Translation;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const TransformComponent& data)
{
	msg << data.Translation;
	msg << data.Rotation;
	msg << data.Scale;
	return msg;
}

struct ObjectCreateMessage
{
	uint64_t EntityID;
	int EntityType;

	friend std::ostream& operator<<(std::ostream& aOutstream, const ObjectCreateMessage& aMessage)
	{
		aOutstream << "Created entity: " << aMessage.EntityID << " to ";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, ObjectCreateMessage& data)
{
	msg >> data.EntityID;
	msg >> data.EntityType;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const ObjectCreateMessage& data)
{
	msg << data.EntityType;
	msg << data.EntityID;
	msg << static_cast<char>(NetworkMessages::ObjectSpawn);
	return msg;
}

struct ObjectMoveMessage
{
	uint64_t EntityID;
	TransformComponent Transform;

	friend std::ostream& operator<<(std::ostream& aOutstream, const ObjectMoveMessage& aMessage)
	{
		aOutstream << "Moved entity: " << aMessage.EntityID << " to ";
		return aOutstream;
	}
};

inline TurNet::TurMessage& operator>>(TurNet::TurMessage& msg, ObjectMoveMessage& data)
{
	msg >> data.EntityID;
	msg >> data.Transform;
	return msg;
}

inline TurNet::TurMessage& operator<<(TurNet::TurMessage& msg, const ObjectMoveMessage& data)
{
	msg << data.Transform;
	msg << data.EntityID;
	msg << static_cast<char>(NetworkMessages::ObjectMove);
	return msg;
}


void NetworkClient::Start()
{
	if(!myClient.Start())
	{
		std::cout << "Failed to create socket" << std::endl;
	}


	std::function<void(TurNet::TurMessage&)> messageLoop = [](TurNet::TurMessage& someData)
	{
		char typeBase;
		someData >> typeBase;
		NetworkMessages messageType = static_cast<NetworkMessages>(typeBase);

		switch(messageType)
		{
			case NetworkMessages::ObjectMove:
			{
				ObjectMoveMessage moveMsg;
				someData >> moveMsg;

				std::cout << "Got Something from server";

				auto view = SceneManager::GetScene()->GetRegistry().view<IDComponent, TransformComponent>();
				for(auto entity : view)
				{
					if(!SceneManager::GetScene()->GetRegistry().valid(entity))
					{
						return;
					}

					auto& ID = view.get<IDComponent>(entity);

					if(ID.ID == moveMsg.EntityID)
					{
						auto& transform = view.get<TransformComponent>(entity);
						transform = moveMsg.Transform;
						std::cout << moveMsg << std::endl;
					}
				}
				break;
			}
			case NetworkMessages::ObjectSpawn:
			{
				ObjectCreateMessage createMsg;
				someData >> createMsg;

				std::cout << "Got Something from server" << std::endl;

				SceneManager::CreateEntityType(createMsg.EntityType, createMsg.EntityID);

				break;
			}
		}
	};

	myClient.SetReceiveMessageLoop(messageLoop);
	myClient.Connect(myHost, DEFAULT_PORT);

	TurNet::TurMessage connectMessage;
	connectMessage.Header.ID = TurNet::NetworkDataTypes::Connect;

	myClient.SendToServer(connectMessage);

	Started = true;
}
