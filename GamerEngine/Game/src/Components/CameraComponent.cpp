#include "Components/CameraComponent.h"
#include "Components/TransfromComponent.h"
#include "Components.hpp"
#include "Math/MathTypes.hpp"
#include "openvr.h"
#include "VR/VRMath.h"
//#include <DirectXMath.h>

CameraComponent::CameraComponent()
{
	Initialize(90, 0.1f, 250000.0f, { DX11::Get().GetScreenSize().x, DX11::Get().GetScreenSize().y });
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

	myHorizontalFoV = hFoVRad;
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
	return myHorizontalFoV;
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

Vector3f CameraComponent::GetPosition()
{
	return myPosition;
}

Vector3f CameraComponent::GetForward()
{
	return myRotation.Forward();
}

Matrix4x4f CameraComponent::GetCurrentViewProjectionMatrix(VREye anEye)
{
	if (anEye == VREye::None)
	{
		return ViewFlatProjection;
	}

	return ViewProjection;
}

Matrix4x4f CameraComponent::GetHMDMatrixPoseEye(VREye anEye)
{
	if (DX11::Get().IsVrNull())
		return Matrix4x4f();

	Matrix4x4f matrixObj = DX11::Get().GetVRSystem().GetEyeToHeadTransform(anEye);
	return Matrix4x4f::GetFastInverse(matrixObj);
}


Matrix4x4f CameraComponent::GetHMDMatrixProjectionEye(VREye anEye)
{
	if (!DX11::Get().IsVrNull() || anEye == VREye::None)
		return Projection;

	Matrix4x4f matrixObj = DX11::Get().GetVRSystem().GetProjectionMatrix( anEye, myNearPlane, myFarPlane);
	return matrixObj;
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
	myPosition = aTransform->GetPosition();
	myRotation = CommonUtilities::Quat::FromEulers(ToRadians(aTransform->GetRotation()));

	myTransform = aTransform->AsGraphicsTransform();
	

#if ENABLE_VR
	CommonUtilities::Quaternionf rotation = DX11::Get().GetVRSystem().GetHMDPose().GetQuat();
	ViewProjection = ComposeFromTRS(aTransform->GetPosition(), rotation, aTransform->GetScale());
	ViewFlatProjection = ComposeFromTRS(aTransform->GetPosition(), myRotation, aTransform->GetScale());
#else
	ViewFlatProjection = aTransform->GetMatrix();
#endif
	

	myFrustum = CommonUtilities::CreateFrustumFromCamera(ComposeFromTRS(aTransform->GetPosition(), myRotation, {1,1,1}), myVerticalFoV * CommonUtilities::RadToDeg, myHorizontalFoV * CommonUtilities::RadToDeg, myNearPlane, myFarPlane);


	//ViewProjection = Matrix4x4f(1.0f);

	//ViewProjection = ViewProjection * rotationQuat;
	//ViewProjection.SetPosition({ aTransform->Translation.x,aTransform->Translation.y,aTransform->Translation.z, 1.0f });


	//myFrustum = CommonUtilities::CreateFrustumFromCamera(ViewProjection, myVerticalFoV, myHorizontalFoV, myNearPlane, myFarPlane);
}

void CameraComponent::SetupCameras()
{
	ProjectionLeft = GetHMDMatrixProjectionEye(VREye::Left);
	ProjectionRight = GetHMDMatrixProjectionEye(VREye::Right);
	ViewPosLeft = GetHMDMatrixPoseEye(VREye::Left);
	ViewPosRight = GetHMDMatrixPoseEye(VREye::Right);
}

Matrix4x4f CameraComponent::GetCurrentViewMatrix(VREye evr_eye)
{
	return ViewProjection;
}


