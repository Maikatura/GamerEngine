#include "GraphicsEngine.pch.h"
#include "Components/CameraComponent.h"

#include "Components.hpp"
#include "openvr_capi.h"

CameraComponent::CameraComponent()
{
	Initialize(90, 0.1f, 25000.0f, { DX11::m_nRenderWidth, DX11::m_nRenderHeight});
}

inline Matrix4x4f ComposeFromTRS(const Vector3f& aTranslation, const CommonUtilities::Quaternion<float>& aRotationQuat, const Vector3f& aScale)
{
	return Matrix4x4f::CreateScale(aScale)
		* aRotationQuat.GetRotationMatrix4x4()
		* Matrix4x4f::CreateTranslation(aTranslation);
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

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose)
{
	Matrix4x4f matrixObj;
	matrixObj(0) = matPose.m[0][0];
	matrixObj(1) = matPose.m[1][0];
	matrixObj(2) = matPose.m[2][0];
	matrixObj(3) = matPose.m[3][0];

	matrixObj(4) = matPose.m[0][1];
	matrixObj(5) = matPose.m[1][1];
	matrixObj(6) = matPose.m[2][1];
	matrixObj(7) = matPose.m[3][1];

	matrixObj(8) = matPose.m[0][2];
	matrixObj(9) = matPose.m[1][2];
	matrixObj(10) = matPose.m[2][2];
	matrixObj(11) = matPose.m[3][2];

	matrixObj(12) = matPose.m[0][3];
	matrixObj(13) = matPose.m[1][3];
	matrixObj(14) = matPose.m[2][3];
	matrixObj(15) = matPose.m[3][3];

	return matrixObj;
}

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4x4(const vr::HmdMatrix44_t& matPose)
{
	Matrix4x4f matrixObj;
	matrixObj(0) = matPose.m[0][0];
	matrixObj(1) = matPose.m[1][0];
	matrixObj(2) = matPose.m[2][0];
	matrixObj(3) = matPose.m[3][0];

	matrixObj(4) = matPose.m[0][1];
	matrixObj(5) = matPose.m[1][1];
	matrixObj(6) = matPose.m[2][1];
	matrixObj(7) = matPose.m[3][1];

	matrixObj(8) = matPose.m[0][2];
	matrixObj(9) = matPose.m[1][2];
	matrixObj(10) = matPose.m[2][2];
	matrixObj(11) = matPose.m[3][2];

	matrixObj(12) = matPose.m[0][3];
	matrixObj(13) = matPose.m[1][3];
	matrixObj(14) = matPose.m[2][3];
	matrixObj(15) = matPose.m[3][3];

	return matrixObj;
}

Matrix4x4f CameraComponent::GetCurrentViewProjectionMatrix(vr::Hmd_Eye anEye)
{
	//Matrix4x4f matMVP;


	//Matrix4x4f eyeToHeadMatrix = ;

	Matrix4x4f eyeToHeadTransform = ConvertSteamVRMatrixToMatrix4(vr::VRSystem()->GetEyeToHeadTransform(anEye));
	Matrix4x4f inverseEyeTransform = Matrix4x4f::GetFastInverse(eyeToHeadTransform);
	Matrix4x4f viewMatrix = inverseEyeTransform;

	// Get the projection matrix for the specified eye
	Matrix4x4f projectionMatrix = DX11::m_mat4HMDPose;

	viewProjectionMatrixVR = Matrix4x4f();
	viewProjectionMatrixVR = ComposeFromTRS(myPosition, projectionMatrix.GetQuat(), { 1, 1, 1 }) * viewMatrix;
	viewProjectionMatrixVR.SetPosition({ myPosition.x,myPosition.y,myPosition.z, 1.0f });

	return viewProjectionMatrixVR;
}

Matrix4x4f CameraComponent::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!DX11::m_pHMD)
		return Matrix4x4f();

	vr::HmdMatrix34_t mat = DX11::m_pHMD->GetEyeToHeadTransform(nEye);
	Matrix4x4f matrixObj;

	matrixObj(0) = mat.m[0][0];
	matrixObj(1) = mat.m[1][0];
	matrixObj(2) = mat.m[2][0];
	matrixObj(3) = mat.m[3][0];

	matrixObj(4) = mat.m[0][1];
	matrixObj(5) = mat.m[1][1];
	matrixObj(6) = mat.m[2][1];
	matrixObj(7) = mat.m[3][1];

	matrixObj(8) = mat.m[0][2];
	matrixObj(9) = mat.m[1][2];
	matrixObj(10) = mat.m[2][2];
	matrixObj(11) = mat.m[3][2];

	matrixObj(12) = mat.m[0][3];
	matrixObj(13) = mat.m[1][3];
	matrixObj(14) = mat.m[2][3];
	matrixObj(15) = mat.m[3][3];

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
	matrixObj(3) = mat.m[3][0];

	matrixObj(4) = mat.m[0][1];
	matrixObj(5) = mat.m[1][1];
	matrixObj(6) = mat.m[2][1];
	matrixObj(7) = mat.m[3][1];

	matrixObj(8) = mat.m[0][2];
	matrixObj(9) = mat.m[1][2];
	matrixObj(10) = mat.m[2][2];
	matrixObj(11) = mat.m[3][2];

	matrixObj(12) = mat.m[0][3];
	matrixObj(13) = mat.m[1][3];
	matrixObj(14) = mat.m[2][3];
	matrixObj(15) = mat.m[3][3];
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


	CommonUtilities::Quaternionf rotation = DX11::m_mat4HMDPose.GetQuat();

	//rotation.x = -rotation.x;
	//rotation.y = -rotation.y;
	////rotation.z = 0.0f;

	myPosition = aTransform->Translation;
	ViewProjection = ComposeFromTRS(aTransform->Translation, rotation, aTransform->Scale);
	//ViewProjection = ViewProjection * rotationQuat;


	//ViewProjection = Matrix4x4f(1.0f);

	//ViewProjection = ViewProjection * rotationQuat;
	//ViewProjection.SetPosition({ aTransform->Translation.x,aTransform->Translation.y,aTransform->Translation.z, 1.0f });


	//myFrustum = CommonUtilities::CreateFrustumFromCamera(ViewProjection, myVerticalFoV, myHorizontalFoV, myNearPlane, myFarPlane);
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


