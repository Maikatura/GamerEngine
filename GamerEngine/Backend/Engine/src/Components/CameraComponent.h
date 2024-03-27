#pragma once
#include "Camera.h"
#include "Math/Frustum.h"
#include "Math/MathTypes.hpp"

enum class VREye;
class TransformComponent;

class CameraComponent
{

	Vector3f myRotation;
	Vector2ui myRes;
	Vector3f myPosition;

public:

	GraphicsTransform myTransform;

	Matrix4x4f ProjectionLeft;
	Matrix4x4f ViewPosLeft;

	Matrix4x4f ProjectionRight;
	Matrix4x4f ViewPosRight;

	Matrix4x4f Projection;
	Matrix4x4f ViewProjection;
	Matrix4x4f ViewFlatProjection;
	CommonUtilities::Frustum myFrustum;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;

	bool Primary = true;
	bool EditorCam = false;

	CameraComponent();
	CameraComponent(const CameraComponent&) = default;

	void Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution = { 0,0 });
	void Resize(Vector2ui aResolution);
	float GetResScale();

	float GetHorizontalFoV();
	float GetVerticalFoV();
	Vector2ui GetResolution();


	CommonUtilities::Frustum GetFrustum() const
	{
		return myFrustum;
	}

	void SetCameraSpeed(float aCameraSpeed);
	float GetCameraSpeed();

	Vector3f GetPosition();
	Vector3f GetForward();

	Matrix4x4f GetCurrentViewProjectionMatrix(VREye anEye);
	Matrix4x4f GetHMDMatrixPoseEye(VREye anEye);
	Matrix4x4f GetHMDMatrixProjectionEye(VREye anEye);

	bool HasMoved();
	void SetHasMoved(bool aMoveValue);
	void BuildTransform(TransformComponent* aTransform);

	void SetupCameras();
	Matrix4x4f GetCurrentViewMatrix(VREye evr_eye);

private:
	bool myHasMoved;
	float myCameraSpeed;
	float myHorizontalFoV;
	float myVerticalFoV;
	

	Matrix4x4f viewProjectionMatrixVR;

};

