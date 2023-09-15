#include "GraphicsEngine.pch.h"
#include "Components/CameraComponent.h"

#include "Components.hpp"
#include "openvr_capi.h"
//#include <DirectXMath.h>

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
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		} 
	}

	return matrixObj;
}

inline Matrix4x4f ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix44_t& matPose)
{
	Matrix4x4f matrixObj;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			matrixObj(row + 1, col + 1) = matPose.m[col][row]; // Transpose the matrix during conversion
		}
	}

	return matrixObj;
}

Matrix4x4f CameraComponent::GetCurrentViewProjectionMatrix(VR_Eyes anEye)
{
	//Matrix4x4f matMVP;
	/*if (anEye == VR_Eyes::None)
	{
		return ;
	}*/


	//Matrix4x4f eyeToHeadMatrix = ;

	Matrix4x4f eyeToHeadTransform = ConvertSteamVRMatrixToMatrix4(vr::VRSystem()->GetEyeToHeadTransform(VR_Eyes::Left == anEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right));
	Matrix4x4f inverseEyeTransform = eyeToHeadTransform;
	Matrix4x4f viewMatrix = inverseEyeTransform;

	// Get the projection matrix for the specified eye
	Matrix4x4f projectionMatrix = DX11::m_mat4HMDPose;

	viewProjectionMatrixVR = Matrix4x4f();
	viewProjectionMatrixVR = ComposeFromTRS({0,0,0}, projectionMatrix.GetQuat(), {1, 1, 1}) * viewMatrix;
	viewProjectionMatrixVR.SetPosition({ myPosition.x,myPosition.y,myPosition.z, 1.0f });

	return ViewProjection;
}

Matrix4x4f CameraComponent::GetHMDMatrixPoseEye(VR_Eyes anEye)
{
	if (!DX11::m_pHMD)
		return Matrix4x4f();

	Matrix4x4f matrixObj = ConvertSteamVRMatrixToMatrix4(DX11::m_pHMD->GetEyeToHeadTransform(VR_Eyes::Left == anEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right));


	return Matrix4x4f::GetFastInverse(matrixObj);
}

inline Matrix4x4f ConvertXMMatrixToMyMatrix(const DirectX::XMMATRIX& xmMatrix)
{
	DirectX::XMFLOAT4X4 float4x4;
	DirectX::XMStoreFloat4x4(&float4x4, xmMatrix);

	Matrix4x4f output;

	// Copy the components to your custom 1D array matrix
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			output[i * 4 + j] = float4x4.m[i][j];
		}
	}

	return output;
}

Matrix4x4f CameraComponent::GetHMDMatrixProjectionEye(VR_Eyes anEye)
{
	if (!DX11::m_pHMD)
		return Matrix4x4f();

	if (anEye == VR_Eyes::None)
	{
		//Matrix4x4f matrix = ConvertXMMatrixToMyMatrix(DirectX::XMMatrixOrthographicLH(1280, 720, myNearPlane, myFarPlane));
		//return matrix;
	}

	// TODO check here
	Matrix4x4f matrixObj = ConvertSteamVRMatrixToMatrix4(DX11::m_pHMD->GetProjectionMatrix(VR_Eyes::Left == anEye ? vr::Hmd_Eye::Eye_Left : vr::Hmd_Eye::Eye_Right, myNearPlane, myFarPlane));

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


	myFrustum = CommonUtilities::CreateFrustumFromCamera(ViewProjection, myVerticalFoV, myHorizontalFoV, myNearPlane, myFarPlane);
}

void CameraComponent::SetupCameras()
{
	ProjectionLeft = GetHMDMatrixProjectionEye(VR_Eyes::Left);
	ProjectionRight = GetHMDMatrixProjectionEye(VR_Eyes::Right);
	ViewPosLeft = GetHMDMatrixPoseEye(VR_Eyes::Left);
	ViewPosRight = GetHMDMatrixPoseEye(VR_Eyes::Right);
}

Matrix4x4f CameraComponent::GetCurrentViewMatrix(vr::Hmd_Eye evr_eye)
{
	return ViewProjection;
}


