#include "RandomMoverComponent.h"
#include "Components/Network/NetworkComponent.h"
#include <random>

#include "Network/NetworkSettings.h"
#include "TurNet/Server/ServerMessageQueue.h"
#include "../../Editor/src/Layers/Network/MoveObjectMessage.h"

RandomMoverComponent::RandomMoverComponent()
{
	mySendTimer = 5.0f;
}

void RandomMoverComponent::OnCreate()
{
	if (HasComponent<RandomMoverData>())
	{
		myMoverData = &GetComponent<RandomMoverData>();
	}
	else
	{
		myMoverData = &AddComponent<RandomMoverData>();
	}
}

void RandomMoverComponent::OnUpdate()
{
	auto& transform = GetComponent<TransformComponent>();

	transform.Translation = Vector3f(transform.Translation.x, transform.Translation.y, transform.Translation.z + 1400.0f * Time::GetDeltaTime());


	/*if (!HasComponent<Network::NetworkComponent>())
	{
		return;
	}*/

	//auto& netComp = GetComponent<Network::NetworkComponent>();

	//

	//auto& transform = GetComponent<TransformComponent>();

	//if(NetworkSettings::IsClient)
	//{
	//	return;
	//}

	//if (mySendTimer <= 0.0f)
	//{
	//	std::random_device randomdevice; // obtain a random number from hardware
	//	std::mt19937 gene(randomdevice());

	//	std::uniform_int_distribution<> disx(static_cast<int>(myMoverData->myMinArea.x), static_cast<int>(myMoverData->myMaxArea.x));
	//	float xPos = static_cast<float>(disx(gene));

	//	std::uniform_int_distribution<> disy(static_cast<int>(myMoverData->myMinArea.y), static_cast<int>(myMoverData->myMaxArea.y));
	//	float yPos = static_cast<float>(disy(gene));

	//	std::uniform_int_distribution<> disz(static_cast<int>(myMoverData->myMinArea.z), static_cast<int>(myMoverData->myMaxArea.z));
	//	float zPos = static_cast<float>(disz(gene));

	//	transform.Translation = Vector3f(xPos, yPos, zPos);
	//	mySendTimer = 5.0f;

	//	TurNet::TurMessage message;
	//	NetObjectMoveMessage objMove;
	//	objMove.EntityID = netComp.GetID();
	//	objMove.Transform = transform;


	//	message << objMove;


	//	TurNet::ServerMessageQueue::Push(message);
	//}
	//else
	//{
	//	mySendTimer -= Time::GetDeltaTime();
	//}

}

float RandomMoverComponent::GetSpeed()
{
	return myMoverData->mySpeed;
}

void RandomMoverComponent::SetSpeed(float aSpeed)
{
	myMoverData->mySpeed = aSpeed;
}

void RandomMoverComponent::SetMinArea(Vector3f aMinArea)
{
	myMoverData->myMinArea = aMinArea;
}

void RandomMoverComponent::SetMaxArea(Vector3f aMaxArea)
{
	myMoverData->myMaxArea = aMaxArea;
}

Vector3f RandomMoverComponent::GetMinArea()
{
	return myMoverData->myMinArea;
}

Vector3f RandomMoverComponent::GetMaxArea()
{
	return myMoverData->myMaxArea;
}