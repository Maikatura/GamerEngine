#pragma once
#include "Component.h"
#include "Math/Frustum.h"

class TransformComponent;

class CameraComponent : public Component
{

public:
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


	bool HasMoved();
	void SetHasMoved(bool aMoveValue);
	void BuildTransform(TransformComponent* aTransform);

private:
	bool myHasMoved;
	float myCameraSpeed;
	float myHorizontalFoV;
	float myVerticalFoV;
	Vector2ui myRes;
	Vector3f myPosition;

};

