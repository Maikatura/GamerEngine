#include "GraphicsEngine.pch.h"
#include "Components/CameraComponent.h"

CameraComponent::CameraComponent()
{
	Initialize(90, 0.1f, 1500.0f, { 1920, 1080 });
}

void CameraComponent::Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution)
{
	assert(aNearPlane < aFarPlane);
	assert(aNearPlane >= 0.01f);

	myFoV = aHorizontalFoV;

	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;

	if(aResolution.x == 0 && aResolution.y == 0)
	{
		Resize(myRes);
	}
	else
	{
		Resize(aResolution);
	}
}

void CameraComponent::Resize(Vector2ui aResolution)
{
	myRes.x = aResolution.x;
	myRes.y = aResolution.y;

	const float hFoVRad = myFoV * (0.01745f);
	const float vFoVRad = 2 * std::atan(std::tan(hFoVRad / 2) * (static_cast<float>(aResolution.y) / static_cast<float>(aResolution.x)));

	const float myXScale = 1 / std::tanf(hFoVRad * 0.5f);
	const float myYScale = 1 / std::tanf(vFoVRad * 0.5f);
	const float Q = myFarPlane / (myFarPlane - myNearPlane);

	Projection(1, 1) = myXScale;
	Projection(2, 2) = myYScale;
	Projection(3, 3) = Q;
	Projection(3, 4) = 1.0f / Q;
	Projection(4, 3) = -Q * myNearPlane;
	Projection(4, 4) = 0.0f;
}

float CameraComponent::GetResScale()
{
	return static_cast<float>(myRes.y) / static_cast<float>(myRes.x);
}
