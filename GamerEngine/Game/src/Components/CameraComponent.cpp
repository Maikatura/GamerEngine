#include "GraphicsEngine.pch.h"
#include "Components/CameraComponent.h"

#include "Components.hpp"
#include "openvr_capi.h"

CameraComponent::CameraComponent()
{
	Initialize(90, 0.1f, 25000.0f, { DX11::m_nRenderWidth, DX11::m_nRenderHeight});
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

Vector3f CameraComponent::GetPosition()
{
	return myPosition;
}

Matrix4x4f CameraComponent::GetCurrentViewProjectionMatrix(vr::Hmd_Eye anEye)
{
	Matrix4x4f matMVP;

	if (anEye == vr::Eye_Left)
	{
		matMVP = ProjectionLeft * ViewPosLeft * DX11::m_mat4HMDPose;
	}
	else if (anEye == vr::Eye_Right)
	{
		matMVP = ProjectionRight * ViewPosRight * DX11::m_mat4HMDPose;
	}

	return matMVP;
}

Matrix4x4f CameraComponent::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!DX11::m_pHMD)
		return Matrix4x4f();

	vr::HmdMatrix34_t matEyeRight = DX11::m_pHMD->GetEyeToHeadTransform(nEye);
	Matrix4x4f matrixObj;

	matrixObj(0) = matEyeRight.m[0][0];
	matrixObj(1) = matEyeRight.m[1][0];
	matrixObj(2) = matEyeRight.m[2][0];
	matrixObj(3) = 0.0f;

	matrixObj(4) = matEyeRight.m[0][1];
	matrixObj(5) = matEyeRight.m[1][1];
	matrixObj(6) = matEyeRight.m[2][1];
	matrixObj(7) = 0.0f;

	matrixObj(8) = matEyeRight.m[0][2];
	matrixObj(9) = matEyeRight.m[1][2];
	matrixObj(10) = matEyeRight.m[2][2];
	matrixObj(11) = 0.0f;

	matrixObj(12) = matEyeRight.m[0][3];
	matrixObj(13) = matEyeRight.m[1][3];
	matrixObj(14) = matEyeRight.m[2][3];
	matrixObj(15) = 1.0f;

	return Matrix4x4f::GetFastInverse(matrixObj);
}

Matrix4x4f CameraComponent::GetHMDMatrixProjectionEye(vr::Hmd_Eye anEye)
{
	if (!DX11::m_pHMD)
		return Matrix4x4f();
	// TODO check here
	vr::HmdMatrix44_t mat = DX11::m_pHMD->GetProjectionMatrix(anEye, myNearPlane, myFarPlane);

	Matrix4x4f matrixObj;

	matrixObj(0) = mat.m[0][0];
	matrixObj(1) = mat.m[1][0];
	matrixObj(2) = mat.m[2][0];
	matrixObj(3) = 0.0f;

	matrixObj(4) = mat.m[0][1];
	matrixObj(5) = mat.m[1][1];
	matrixObj(6) = mat.m[2][1];
	matrixObj(7) = 0.0f;

	matrixObj(8) = mat.m[0][2];
	matrixObj(9) = mat.m[1][2];
	matrixObj(10) = mat.m[2][2];
	matrixObj(11) = 0.0f;

	matrixObj(12) = mat.m[0][3];
	matrixObj(13) = mat.m[1][3];
	matrixObj(14) = mat.m[2][3];
	matrixObj(15) = 1.0f;
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
	myPosition = aTransform->Translation;
	ViewProjection = Matrix4x4f::BuildTransform(aTransform->Translation, aTransform->Rotation, aTransform->Scale);
	myFrustum = CommonUtilities::CreateFrustumFromCamera(ViewProjection, myVerticalFoV, myHorizontalFoV, myNearPlane, myFarPlane);
}

void CameraComponent::SetupCameras()
{
	ProjectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
	ProjectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
	ViewPosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	ViewPosRight = GetHMDMatrixPoseEye(vr::Eye_Right);
}

Matrix4x4f CameraComponent::GetCurrentViewMatrix(vr::Hmd_Eye evr_eye)
{
	return ViewProjection;
}


