#pragma once
#include "Component.h"
#include "openvr.h"
#include "Math/Frustum.h"

class TransformComponent;

class CameraComponent : public Component
{

public:


	Matrix4x4f ProjectionLeft;
	Matrix4x4f ViewPosLeft;

	Matrix4x4f ProjectionRight;
	Matrix4x4f ViewPosRight;

	Matrix4x4f Projection;
	Matrix4x4f ViewProjection;
	CommonUtilities::Frustum myFrustum;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;

	bool Primary = true;

	CameraComponent();
	CameraComponent(const CameraComponent&) = default;

	void Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution = { 0,0 });
	void Resize(Vector2ui aResolution);
	float GetResScale();

	float GetHorizontalFoV();
	float GetVerticalFoV();
	Vector2ui GetResolution();

	void SetCameraSpeed(float aCameraSpeed);
	float GetCameraSpeed();

	Vector3f GetPosition();

	Matrix4x4f GetCurrentViewProjectionMatrix(vr::Hmd_Eye evr_eye);
	Matrix4x4f GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	Matrix4x4f GetHMDMatrixProjectionEye(vr::Hmd_Eye anEye);

	bool HasMoved();
	void SetHasMoved(bool aMoveValue);
	void BuildTransform(TransformComponent* aTransform);

	void SetupCameras();
	Matrix4x4f GetCurrentViewMatrix(vr::Hmd_Eye evr_eye);

private:
	bool myHasMoved;
	float myCameraSpeed;
	float myHorizontalFoV;
	float myVerticalFoV;
	Vector2ui myRes;
	Vector3f myPosition;

};

