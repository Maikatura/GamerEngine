#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include <cassert>
#include <cmath>
#define PI 3.14f

bool Camera::Initialize(float aHorizontalFoV, CommonUtilities::Vector2<unsigned int> aResolution, float aNearPlane, float aFarPlane)
{
	
	assert(aNearPlane < aFarPlane);
	assert(aNearPlane >= 0.01f);
	

	myNearPlane = aNearPlane;
	myFarPlane = aFarPlane;
	

	const float hFoVRad = aHorizontalFoV * (PI / 180.f);
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

    return true;
}
