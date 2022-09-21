#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include <cassert>
#include <cmath>
#include <GraphicsEngine.h>
#include <StringCast.h>

#include "Time.hpp"
#define PI 3.14f

bool Camera::Initialize(float aHorizontalFoV, CommonUtilities::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane)
{

	assert(aNearPlane < aFarPlane);
	assert(aNearPlane >= 0.01f);

	myFoV = aHorizontalFoV;

	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;

	Resize(aResolution);

	return true;
}

void Camera::Resize(CommonUtilities::Vector2<unsigned int> aResolution)
{
	const float hFoVRad = myFoV * (PI / 180.f);
	const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

	const float myXScale = 1 / std::tanf(hFoVRad * 0.5f);
	const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
	const float Q = myFarPlane / (myFarPlane - myNearPlane);

	myProjection(1, 1) = myXScale;
	myProjection(2, 2) = myYScale;
	myProjection(3, 3) = Q;
	myProjection(3, 4) = 1.0f / Q;
	myProjection(4, 3) = -Q * myNearPlane;
	myProjection(4, 4) = 0.0f;
}

void Camera::SetPosition(CommonUtilities::Vector3<float> aPosition)
{
	myPos.x = aPosition.x;
	myPos.y = aPosition.y;
	myPos.z = aPosition.z;

	GetTransform().SetPosition({ myPos.x, myPos.y, myPos.z });
}

CommonUtilities::Vector3<float> Camera::GetPosition()
{
	return myPos;
}

CommonUtilities::Vector3<float> Camera::GetXAxis()
{
	auto myTransform = GetTransform().GetMatrix();
	return CommonUtilities::Vector3<float>(myTransform(1, 1), myTransform(1, 2), myTransform(1, 3));
}

CommonUtilities::Vector3<float> Camera::GetYAxis()
{
	auto myTransform = GetTransform().GetMatrix();
	return CommonUtilities::Vector3<float>(myTransform(2, 1), myTransform(2, 2), myTransform(2, 3));
}

CommonUtilities::Vector3<float> Camera::GetZAxis()
{
	auto myTransform = GetTransform().GetMatrix();
	return CommonUtilities::Vector3<float>(myTransform(3, 1), myTransform(3, 2), myTransform(3, 3));
}

void Camera::Update()
{
	float aDeltaTime = Time::GetDeltaTime();
	CommonUtilities::Vector3<float> movement = GetTransform().GetPosition();
	auto input = GraphicsEngine::Get()->GetInput();

	movement += input->ScrollDelta() * GetZAxis() * aDeltaTime * (mySpeed * 5.0f);
	GetTransform().ComposeTransform(movement, { myPitch, myYaw, 0 }, { 1.0f,1.0f ,1.0f });

	if(!input->IsMouseDown(VK_LBUTTON))
	{
		return;
	}

	if(input->IsKeyDown('W'))
	{
		movement += GetZAxis() * aDeltaTime * mySpeed;
	}
	if(input->IsKeyDown('S'))
	{
		movement += -1.0f * GetZAxis() * aDeltaTime * mySpeed;
	}
	if(input->IsKeyDown('D'))
	{
		auto Axis = GetXAxis();
		movement += (Axis * aDeltaTime * mySpeed);
	}

	if(input->IsKeyDown('A'))
	{
		movement += (-1.0f * GetXAxis() * aDeltaTime * mySpeed);
	}
	
	if(input->IsKeyDown(VK_SPACE))
	{
		movement += { 0.0f, 1.0f * aDeltaTime * mySpeed, 0.0f };
	}

	if(input->IsKeyDown(VK_CONTROL))
	{
		movement += { 0.0f, -1.0f * aDeltaTime * mySpeed, 0.0f };
	}

	

	myYaw += input->MouseDelta().x * aDeltaTime * myCameraSensitivity;
	myPitch += input->MouseDelta().y * aDeltaTime * myCameraSensitivity;
	SetRotation(myPitch, myYaw, 0);
	GetTransform().ComposeTransform(movement, { myPitch, myYaw, 0 }, { 1.0f,1.0f ,1.0f });

}
