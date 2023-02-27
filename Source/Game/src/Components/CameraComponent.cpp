#include "GraphicsEngine.pch.h"
#include "Components/CameraComponent.h"

#include "Components.hpp"

CameraComponent::CameraComponent()
{
	Initialize(90, 0.1f, 25000.0f, { 1920, 1080 });
}

void CameraComponent::Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution)
{
	assert(aNearPlane < aFarPlane);
	assert(aNearPlane >= 0.01f);

	if(aResolution.x == 0 || aResolution.y == 0)
	{
		return;
	}

	myFoV = aHorizontalFoV;

	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;

	Resize(aResolution);
}

void CameraComponent::Resize(Vector2ui aResolution)
{
	if (aResolution.x == 0 || aResolution.y == 0)
	{
		return;
	}
	myRes.x = aResolution.x;
	myRes.y = aResolution.y;

	const float hFoVRad = myFoV * (0.01745f);
	const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

	myVerticalFoV = vFoVRad;

	const float myXScale = 1 / std::tanf(hFoVRad * 0.5f);
	const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
	const float Q = myFarPlane / (myFarPlane - myNearPlane);

	Projection(1, 1) = myXScale;
	Projection(2, 2) = myYScale;
	Projection(3, 3) = Q;
	Projection(3, 4) = 1.0f / Q;
	Projection(4, 3) = -Q * myNearPlane;
	Projection(4, 4) = 1.0f;
}

float CameraComponent::GetResScale()
{
	return static_cast<float>(myRes.y) / static_cast<float>(myRes.x);
}

float CameraComponent::GetHorizontalFoV()
{
	return myFoV;
}

float CameraComponent::GetVerticalFoV()
{
	return myVerticalFoV;
}

Vector2ui CameraComponent::GetResolution()
{
	return myRes;
}

void CameraComponent::SetCameraSpeed(float aCameraSpeed)
{
	myCameraSpeed = aCameraSpeed;
}

float CameraComponent::GetCameraSpeed()
{
	return myCameraSpeed;
}

bool CameraComponent::HasMoved()
{
	return myHasMoved;
}

void CameraComponent::SetHasMoved(bool aMoveValue)
{
	myHasMoved = aMoveValue;
}

void CameraComponent::BuildTransform(TransformComponent* aTransform)
{
	ViewProjection = Matrix4x4f::BuildTransform(aTransform->Translation, aTransform->Rotation, aTransform->Scale);
}
