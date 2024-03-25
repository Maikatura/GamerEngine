#include "RandomMoverComponent.h"
#include "Components/Network/NetworkComponent.h"
#include <random>

#include "Time.hpp"
#include "Network/NetworkSettings.h"
#include "TurNet/Server/ServerMessageQueue.h"
#include "../../Editor/src/Layers/Network/MoveObjectMessage.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"

RandomMoverComponent::RandomMoverComponent()
{
	mySendTimer = 5.0f;


	myVelocity.x = static_cast<float>(rand() % 100);
	myVelocity.y = static_cast<float>(rand() % 100);
	myVelocity.z = static_cast<float>(rand() % 100);
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

	const float radius = 125.0f;

	if (transform.GetPosition().x < -radius)
	{
		transform.GetPosition().x = radius;
	}
	else if (transform.GetPosition().x > radius)
	{
		transform.GetPosition().x = -radius;
	}

	if (transform.GetPosition().y < -radius)
	{
		transform.GetPosition().y = radius;
	}
	else if (transform.GetPosition().y > radius)
	{
		transform.GetPosition().y = -radius;
	}

	if (transform.GetPosition().z < -radius)
	{
		transform.GetPosition().z = radius;
	}
	else if (transform.GetPosition().z > radius)
	{
		transform.GetPosition().z = -radius;
	}

	transform.GetPosition() += myVelocity * Time::GetDeltaTime();


	
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