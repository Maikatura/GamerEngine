#pragma once
class CameraComponent
{
public:
	Matrix4x4f Projection;
	Matrix4x4f ViewProjection;

	float myNearPlane = 0.1f;
	float myFarPlane = 1500.0f;
	float myFoV = 90.0f;

	bool Primary = true;

	CameraComponent();
	CameraComponent(const CameraComponent&) = default;

	void Initialize(float aHorizontalFoV, float aNearPlane, float aFarPlane, Vector2ui aResolution = { 0,0 });
	void Resize(Vector2ui aResolution);
	float GetResScale();

	float GetVerticalFoV();
	Vector2ui GetResolution();

private:
	float myVerticalFoV;
	Vector2ui myRes;

};

