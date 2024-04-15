#include "GraphicsEngine.pch.h"
#include "NetworkComponent.h"
#include "Core/Model/Entity.h"

Network::NetworkComponent::NetworkComponent()
{
	myFirstStart = true;
}

Network::NetworkComponent::NetworkComponent(GamerEngine::UUID aUUID)
{
	myServerId = aUUID;
	myFirstStart = true;
}

GamerEngine::UUID Network::NetworkComponent::GetID()
{
	return myServerId;
}

void Network::NetworkComponent::SetID(GamerEngine::UUID aUUID2)
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
