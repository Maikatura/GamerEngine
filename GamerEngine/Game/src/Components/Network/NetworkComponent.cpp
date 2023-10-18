#include "NetworkComponent.h"

#include "Renderer/Model/Entity.h"

Network::NetworkComponent::NetworkComponent()
{
	myFirstStart = true;
}

Network::NetworkComponent::NetworkComponent(UUID2 aUUID)
{
	myServerId = aUUID;
	myFirstStart = true;
}

UUID2 Network::NetworkComponent::GetID()
{
	return myServerId;
}

void Network::NetworkComponent::SetID(UUID2 aUUID2)
{
	myServerId = aUUID2;
}

bool Network::NetworkComponent::IsServer()
{
	return myIsServer;
}

void Network::NetworkComponent::SetServer(bool aIsServer)
{
	myIsServer = aIsServer;
}

void Network::NetworkComponent::OnUpdate()
{
	if (myEntity)
	{
		auto& transform = myEntity->GetComponent<TransformComponent>();

		if (myFirstStart)
		{
			myFirstStart = false;
			myNewTranslation = transform.GetPosition();
		}


		if (myShouldSmooth)
		{
			transform.SetPosition(Vector3f::Lerp(transform.GetPosition(), myNewTranslation, Time::GetDeltaTime()));
		}
		else
		{
			transform.SetPosition(myNewTranslation);
		}
	}
	
}

void Network::NetworkComponent::SetNewPosition(Vector3f aNewPosition)
{
	myNewTranslation = aNewPosition;
}

bool Network::NetworkComponent::ShouldSmooth()
{
	return myShouldSmooth;
}

void Network::NetworkComponent::SetShouldSmooth(bool shouldSmooth)
{
	myShouldSmooth = shouldSmooth;
}
