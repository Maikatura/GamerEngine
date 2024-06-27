#include "GraphicsEngine.pch.h"
#include "Camera.h"
#include "Camera.h"
#include "GraphicsEngine.h"
#include "Math/Ray.hpp"

void Camera::SetPerspective(float aHorizontalFoV, const Vector2ui& aResolution, float aNearClipPlane, float aFarClipPlane)
{
	
	myResolution = aResolution;
	myNearClipPlane = aNearClipPlane;
	myFarClipPlane = aFarClipPlane;
	myHFoV = aHorizontalFoV;

		myAspectRatio = static_cast<float>(myResolution.x) / static_cast<float>(myResolution.y);

	//Aspect ratio where vfov becomes 135 degrees
	//Prevents flipping of projection
	constexpr float minAspectRatio = 0.625f;
	if (myAspectRatio < minAspectRatio)
	{
		myResolution.x = static_cast<uint32_t>(static_cast<float>(myResolution.y) * minAspectRatio);
		myResolution.y = static_cast<uint32_t>(static_cast<float>(myResolution.x) / minAspectRatio);
		myAspectRatio = minAspectRatio;
	}


	float vFovRad = static_cast<float>(2.0) * std::atan(std::tan(myHFoV * CommonUtilities::DegToRad / static_cast<float>(2.0))) * (1.0f / myAspectRatio);
	myVFoV = vFovRad * CommonUtilities::RadToDeg;

	myFrustum = CommonUtilities::CreateFrustumFromCamera(myTransform.GetMatrix(), myVFoV, myHFoV, myNearClipPlane, myFarClipPlane);
	myProjection = Matrix4x4f::CreatePerspectiveHFoVResolution(myHFoV, myResolution, myNearClipPlane, myFarClipPlane);
}

void Camera::SetOrthographicPerspective(float aLeft, float aRight, float aBottom, float aTop, float aNear, float aFar)
{
	myAspectRatio = 1.f;
	myVFoV = 0;

	myFrustum = CommonUtilities::CreateFrustumFromCamera(myTransform.GetMatrix(), myVFoV, myHFoV, myNearClipPlane, myFarClipPlane);
	myProjection = Matrix4x4f::CreateOrthographicProjection(aLeft, aRight, aBottom, aTop, aNear, aFar);
}

Matrix4x4f Camera::GetViewMatrix() const
{
	return Matrix4x4f::AffineInverse(myTransform.GetMatrix());
}

Matrix4x4f Camera::GetWorldViewMatrix() const
{
	return myTransform.GetMatrix();
}

Vector3f Camera::ScreenToWorldPos(Vector2f aMousePos, float aZ) const
{
	CommonUtilities::Ray<float> ray = ScreenToWorldRay(aMousePos);

	float worldZ = aZ - GetTransform().Translation.Length();
	Vector3f worldPosition = ray.GetOrigin() + ray.GetDirection() * worldZ;
	return worldPosition;
}

CommonUtilities::Ray<float> Camera::ScreenToWorldRay(Vector2f aMousePos) const
{
	Vector2f posSS = aMousePos;

	float viewportWidth = static_cast<float>(myResolution.x);
	float viewportHeight = static_cast<float>(myResolution.y);

	Vector2f posNDC
	{
		(2.0f * posSS.x / viewportWidth) - 1.0f,
		(-2.0f * posSS.y / viewportHeight) + 1.0f
	};

	Matrix4x4f projectionMatrix = GetProjection();

	Vector2f rayView
	{
		posNDC.x / projectionMatrix(0),
		posNDC.y / projectionMatrix(6)
	};

	Vector4f viewRayOrigin{ 0.0f, 0.0f, 0.0f, 1.0f };
	Vector4f viewRayDirection{ rayView.x, rayView.y, 1.0f, 0.0f };

	Matrix4x4f inverseView = myTransform.GetMatrix();

	Vector3f worldRayOrigin = inverseView.TransformPoint(viewRayOrigin);
	Vector3f worldRayDirection = inverseView.TransformVector(viewRayDirection);

	CommonUtilities::Ray<float> worldRay;
	worldRay.InitWithOriginAndDirection(worldRayOrigin, worldRayDirection);

	return worldRay;
}

const CommonUtilities::Frustum& Camera::GetFrustum() const
{
	return myFrustum;
}
